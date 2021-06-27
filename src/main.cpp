#define LILYGO_T5_V213
#define SERIAL_BAUD_RATE 115200
#define uS_TO_S_FACTOR 1000000   // Conversion factor for micro seconds to seconds 
#define TIME_TO_SLEEP 60         // Time ESP32 will go to sleep (in seconds) 
#define UPDATE_WEATHER_PROID  60 // One hour update cycle
#define TEMPADCPIN 34

#include <Arduino.h>
#include <boards.h>
#include <GxEPD.h>
#include <GxGDEM0213B74/GxGDEM0213B74.h>  // 2.13" b/w  form GoodDisplay 4-color
#include <U8g2_for_Adafruit_GFX.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <time.h>
#include "IMG_0001.h"
#include "thermometer.h"
#include "weather.h"
#include "Clock.h"
#include "DisplayService.h"

extern "C" {
    #include "box.h"
}

GxIO_Class io(SPI, EPD_CS, EPD_DC, EPD_RSET);
GxEPD_Class display(io, EPD_RSET, EPD_BUSY);
U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;
uint16_t msgIncr = 0;
RTC_DATA_ATTR int bootCount = 0;
void(* resetFunc) (void) = 0;  //declare reset funciton @ address 0
Thermometer tm(display, TEMPADCPIN);
Clock clock(7200, 3600, "pool.ntp.org");

const char * ssid = "Krisi";
const char * ssidPass = "krisi9404194775";


/* OpenWeather settings */
Weather ow("42.1627557", "24.7487006", "Plovdiv Bulgaria", "36d9d56c52e94add5faac575aef78dcf");

void showBoot();
void wifiStart();
void syncTime();
void updateThermometer();
void updateWeather();
void updateClock();
String strTime(time_t);

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println();
    Serial.println("setup");

    delay(1000);

    display.init(SERIAL_BAUD_RATE);
    display.setRotation(3);
    
    u8g2Fonts.begin(display);
    u8g2Fonts.setFontMode(1);                   // use u8g2 transparent mode (this is default)
    u8g2Fonts.setFontDirection(0);              // left to right (this is default)
    u8g2Fonts.setForegroundColor(GxEPD_BLACK);  // apply Adafruit GFX color
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);  // apply Adafruit GFX color

    display.fillScreen(GxEPD_WHITE);

    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER
    || esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) {
        Serial.print("Boot count: ");
        Serial.println(bootCount);

        if (++bootCount >= UPDATE_WEATHER_PROID) {
            bootCount = 0;
            wifiStart();
            updateWeather();
        }

        syncTime();
        updateClock();
        updateThermometer();
    } else {
        // First boot
        showBoot();
        wifiStart();
        syncTime();

        updateWeather();
        updateClock();
        updateThermometer();
    }

    esp_sleep_enable_ext0_wakeup(GPIO_NUM_39, 0);
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

    Serial.println("Going to sleep now.");

    display.powerDown();

    Serial.println("Done");

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

void updateWeather() {
    Serial.print("\nRequesting weather information from OpenWeather... ");

    ow.load();

    Serial.println("Weather from Open Weather\n");

    display.setRotation(3);

    OW_current * current = ow.getCurrentForecast();

    display.eraseDisplay();

    // Draw Forecast
    uint16_t y_main = 12;
    u8g2Fonts.setFont( u8g2_font_helvB12_tf  );
    u8g2Fonts.setCursor(0, y_main);
    u8g2Fonts.print(current->main);

    // Draw Current Temp
    uint16_t y_temp = y_main + 16;
    u8g2Fonts.setCursor(0, y_temp);
    if (strcmp(ow.getUnits().c_str(), "imperial") == 0) {
        u8g2Fonts.print(String(current->temp, 1) + "°F");
    } else {
        u8g2Fonts.print(String(current->temp, 1) + "°C");
    }

    // Draw Current Humidity
    uint16_t y_humid = y_temp + 15;
    u8g2Fonts.setCursor(0, y_humid);
    u8g2Fonts.printf("%02d%%", current->humidity);  //printf works too

    // Draw last update time at bottom of screen
    u8g2Fonts.setFont( u8g2_font_finderskeepers_tr  );
    u8g2Fonts.setCursor(0, display.height() - 10);
    u8g2Fonts.print("Last update: " + strTime(current->dt));
    u8g2Fonts.setCursor(200, display.height() - 10);
    u8g2Fonts.print(ow.getLocation());

    display.update();
}

void wifiStart() {
    WiFi.begin(ssid, ssidPass);

    Serial.print("Connecting to: ");
    Serial.println(ssid);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("Wifi connected.");
}

void printLocalTime() {
    Serial.println(clock.getTime(), "%A, %B %d %Y %H:%M:%S");
}

void updateClock() {
    struct tm timeInfo;

    if (!getLocalTime(&timeInfo)) {
        Serial.println("Failed obtaining current time");
        return;
    }

    char buff[20];
    strftime(buff, sizeof(buff), "%H:%M", &timeInfo);

    display.setRotation(3);

    box box;
    box.x = display.width() - 90;
    box.y = 0;
    box.w = 90;
    box.h = 30;
    box.background = GxEPD_WHITE;
    uint16_t y = box.y + 30;

    display.fillRect(box.x, box.y, box.w, box.h, box.background);
    u8g2Fonts.setFont(u8g2_font_helvB24_tr);
    u8g2Fonts.setCursor(box.x, y);
    u8g2Fonts.print(buff);

    display.updateWindow(box.x, box.y, box.w, box.h, true);
    display.update();

    // @todo DisplayManager който да събира цялата информация от всички обекти
    // и да има update() метод, който метод да блъска всичко в дисплея и накрая
    // да извиква само веднъж display.update()
}

void updateThermometer() {
    ulong start = millis();

    tm.begin();
    float curTemp = tm.getReading();

    Serial.print("Current room temperature: ");
    Serial.print(curTemp);
    Serial.print(" degrees celsius. Temp measured in ");
    Serial.print(millis() - start);
    Serial.println("ms.");
}

void syncTime() {
    clock.begin();
    printLocalTime();
}

String strTime(time_t unixTime) {
  unixTime += gmtOffset_sec;
  return ctime(&unixTime);
}