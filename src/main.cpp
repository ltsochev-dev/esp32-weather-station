#define LILYGO_T5_V213
#define SERIAL_BAUD_RATE 115200
#define uS_TO_S_FACTOR 1000000   // Conversion factor for micro seconds to seconds 
#define TIME_TO_SLEEP 60         // Time ESP32 will go to sleep (in seconds) 
#define UPDATE_WEATHER_PROID  60 // One hour update cycle
#define TEMPADCPIN 36
#define CONFIG_LITTLEFS_CACHE_SIZE 512

#include <Arduino.h>
#include <boards.h>
#include <GxEPD.h>
#include <GxGDEM0213B74/GxGDEM0213B74.h>  // 2.13" b/w  form GoodDisplay 4-color
#include <U8g2_for_Adafruit_GFX.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>
#include <WiFi.h>
#include "Config/Config.h"
#include "IMG_0001.h"
#include "thermometer.h"
#include "weather.h"
#include "Clock.h"
#include "DisplayService.h"
#include "DisplayData.h"

void(* resetFunc) (void) = 0;  //declare reset funciton @ address 0
GxIO_Class io(SPI, EPD_CS, EPD_DC, EPD_RSET);
GxEPD_Class display(io, EPD_RSET, EPD_BUSY);
RTC_DATA_ATTR int bootCount = 0;
RTC_DATA_ATTR displaydata_t displayDataStore;

Config config("/settings.json", SPIFFS);
Clock timeClock(7200, 3600, "pool.ntp.org");
Weather ow;
Thermometer tm(display, TEMPADCPIN);
DisplayService displayManager(display, ow, timeClock, tm);

void showBoot();
void loadConfig();
void wifiStart();
void dumpDisplayData();
void printFsData();

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println();
    Serial.println("setup");

    delay(1000);

    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occured while mounting the SPIFFS");
        return;
    }

    Serial.println("Attempting to load config");

    loadConfig();

    location_conf_t weatherConf = config.getWeatherInfo();
    ow.setApiKey(weatherConf.owApiKey);
    ow.setLocation(String(weatherConf.latitude), String(weatherConf.longitude), weatherConf.location);

    dumpDisplayData();

    display.init(SERIAL_BAUD_RATE);
    displayManager.begin();
    
    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER
    || esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) {
        Serial.print("Boot count: ");
        Serial.println(bootCount);

        if (++bootCount >= UPDATE_WEATHER_PROID) {
            bootCount = 0;
            wifiStart();
            displayManager.fullUpdate();
        } else {
            displayManager.unserialize(displayDataStore);
            timeClock.refresh();
            displayManager.update();
        }

    } else {
        // First boot
        showBoot();
        printFsData();
        wifiStart();

        timeClock.begin();
        display.eraseDisplay();
        displayManager.fullUpdate();
    }

    displayManager.serialize(displayDataStore);

    esp_sleep_enable_ext0_wakeup(GPIO_NUM_39, 0);
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

    Serial.println("Going to sleep now.");

    esp_deep_sleep_start();
}

void loop() {
    // Do nothing
}

void showBoot() {
    display.setRotation(2);
    // thanks to bytecrusher: http://forum.arduino.cc/index.php?topic=487007.msg3367378#msg3367378
    uint16_t x = (display.width() / 2) - (LOGO_WIDTH / 2);
    uint16_t y = (display.height() / 2) - (LOGO_HEIGHT / 2);
    display.fillScreen(GxEPD_WHITE);
    display.drawExampleBitmap(gImage_IMG_0001, x, y, LOGO_WIDTH, LOGO_HEIGHT, GxEPD_BLACK);
    display.update();
    delay(500);
}

void wifiStart() {
    String ssid = config.getWifiSSID();
    String ssidPass = config.getWifiPassword();

    ssid.trim();
    ssidPass.trim();

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), ssidPass.c_str());

    Serial.print("Connecting to: ");
    Serial.print(ssid);
    Serial.print(" with password: ");
    Serial.println(ssidPass);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("Wifi connected.");
}

void dumpDisplayData() {
    Serial.println("Dumping serialized display data...");

    Serial.print("Main weather: ");
    Serial.println(displayDataStore.forecasts[0].weather);

    Serial.print("Unix time: ");
    Serial.println(displayDataStore.unixTime);

    Serial.print("Room temp: ");
    Serial.println(displayDataStore.roomTemp);

    Serial.println("Forecasts");

    for (int i = 0; i < 5; i++) {
        forecast_t forecast = displayDataStore.forecasts[i];

        Serial.print("Forecast #");
        Serial.print(i);
        Serial.print(" main: ");
        Serial.print(forecast.weather);
        Serial.print(" -- ");
        Serial.println(forecast.temp);
    }

    Serial.println("Data dumped successfully...");

    delay(500);
}

void printFsData() {
    unsigned int totalBytes = SPIFFS.totalBytes();
    unsigned int usedBytes = SPIFFS.usedBytes();

    Serial.println("===== File system info =====");
 
    Serial.print("Total space:      ");
    Serial.print(totalBytes);
    Serial.println("byte");
 
    Serial.print("Total space used: ");
    Serial.print(usedBytes);
    Serial.println("byte");
 
    Serial.println();
}

void loadConfig()
{
    Serial.println("Attempting to parse config...");

    if (!config.begin()) {
        Serial.println("Error parsing config json.");
        return;
    }

    Serial.println("Done.");

    delay(500);
}