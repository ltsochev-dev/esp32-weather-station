#include "DisplayService.h"

DisplayService::DisplayService(GxEPD &display, Weather &weather, Clock &clock, Thermometer &thermo)
    : _display(display), _weather(weather), _clock(clock), _thermometer(thermo)
{
}

void DisplayService::begin()
{
    u8g2Fonts.begin(_display);
    u8g2Fonts.setFontMode(1);                   // use u8g2 transparent mode (this is default)
    u8g2Fonts.setFontDirection(0);              // left to right (this is default)
    u8g2Fonts.setForegroundColor(GxEPD_BLACK);  // apply Adafruit GFX color
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);  // apply Adafruit GFX color
}

void DisplayService::fullUpdate()
{
    loadSources();

    _display.fillScreen(GxEPD_WHITE);
    _display.update();

    _display.setRotation(3);

    drawClock();
    drawCurrentWeather();
    drawRoomTemperature();
    drawForecast();

    _display.update();
}

void DisplayService::update()
{
    _display.setRotation(3);

    drawClock();
    drawCurrentWeather();
    drawRoomTemperature();
    drawForecast();

    _display.update();
}

void DisplayService::loadSources()
{
    Serial.print("Obtaining weather data...");
    _weather.load();
    Serial.println("Done.");

    Serial.print("Obtaining thermometer data...");
    _thermometer.getReading();
    Serial.println("Done.");
}

void DisplayService::drawCurrentWeather()
{
    OW_current * current = _weather.getCurrentForecast();

    _display.setRotation(3);

    // Draw forecast
    uint16_t y_main = 12;
    u8g2Fonts.setFont(u8g2_font_helvB12_tf);
    u8g2Fonts.setCursor(0, y_main);
    u8g2Fonts.print(current->main);

    // Draw current temp
    y_main += 16;
    u8g2Fonts.setCursor(0, y_main);
    u8g2Fonts.print(String(current->temp, 1) + "°C");

    // Draw humidity
    y_main += 15;
    u8g2Fonts.setCursor(0, y_main);
    u8g2Fonts.printf("%02d%%", current->humidity);

    // Draw last update time at bottom of screen
    u8g2Fonts.setFont( u8g2_font_finderskeepers_tr  );
    u8g2Fonts.setCursor(0, _display.height() - 10);
    u8g2Fonts.print("Last update: " + strTime(current->dt));
}

void DisplayService::drawRoomTemperature()
{
    _display.setRotation(3);

    float temp = _thermometer.getReading();

    uint16_t y_main = 65;
    u8g2Fonts.setFont(u8g2_font_helvB12_tf);
    u8g2Fonts.setCursor(0, y_main);
    u8g2Fonts.print("Room:");
    u8g2Fonts.print(String(temp) + "°C");
}

void DisplayService::drawClock()
{
    _display.setRotation(3);

    box box = createBox(
        _display.width() - 90,
        30, 90, 30, GxEPD_WHITE
    );

    _display.fillRect(box.x, 0, box.w, box.h, box.background);
    
    u8g2Fonts.setFont(u8g2_font_helvB24_tr);
    u8g2Fonts.setCursor(box.x, box.y);
    u8g2Fonts.print(_clock.getTimeString());
}

void DisplayService::drawForecast()
{
    // Each method has to set its own rotation
    _display.setRotation(3);

    // This method draws 4 boxes
    uint16_t box_x = 0;
    uint16_t box_w = floor(_display.width() / 4);
    uint16_t box_h = 45;
    uint16_t box_y =  _display.height() - box_h;

    auto forecast = _weather.getHourlyForecast();

    for (int i = 0; i < 4; i++) {
        box_x = box_w * i;

        _display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
        
        u8g2Fonts.setFont(u8g2_font_helvB12_tf);

        // Draw title
        u8g2Fonts.setCursor(box_x + 2, box_y + 12);
        u8g2Fonts.print(forecast->main[i]);

        // Draw temperature
        u8g2Fonts.setCursor(box_x + 2, box_y + 30);
        u8g2Fonts.print(String(forecast->temp[i], 1) + "°C");

        // Draw humidity
        u8g2Fonts.setCursor(box_x + 2, box_y + 43);
        u8g2Fonts.printf("%02d%%", forecast->humidity[i]);

        uint16_t ax, ay, bx, by, cx, cy, dx, dy;

        ax = box_x;
        ay = box_y;
        bx = box_x + box_w;
        by = box_y;
        dx = box_x;
        dy = _display.height() - 1;
        cx = box_x + box_w;
        cy = _display.height() - 1;

        // Top border
        _display.drawLine(ax, ay, bx, by, GxEPD_BLACK);

        // Right border
        _display.drawLine(bx, by, cx, cy, GxEPD_BLACK);

        // Bottom border
        _display.drawLine(dx, dy, cx, cy, GxEPD_BLACK);
        
        // Left border
        _display.drawLine(ax, ay, dx, dy, GxEPD_BLACK);
    }
}

void DisplayService::serialize(displaydata_t & store)
{
    forecast_t currentForecast;
    OW_current * current = _weather.getCurrentForecast();
    
    time(&store.unixTime);

    store.roomTemp = _thermometer.getLastReading();

    strToForecast(current->main, currentForecast);
    currentForecast.temp = current->temp;
    currentForecast.humidity = current->humidity;

    store.forecasts[0] = currentForecast;

    OW_hourly * hourly = _weather.getHourlyForecast();

    for (int i = 0; i < 4; i++) {
        forecast_t hourlyForecast;
        // hourlyForecast.main = hourly->main[i];
        strToForecast(hourly->main[i], hourlyForecast);
        hourlyForecast.temp = hourly->temp[i];
        hourlyForecast.humidity = hourly->humidity[i];

        store.forecasts[(i + 1)] = hourlyForecast;
    }

    Serial.println("Serialization complete.");
}

void DisplayService::unserialize(displaydata_t & store)
{
    Serial.print("Attempting to deserialize...");

    OW_current * current = _weather.getCurrentForecast();
    OW_hourly * hourly = _weather.getHourlyForecast();

    // current->main = store.forecasts[0].main;
    current->main = String(store.forecasts[0].weather);
    current->temp = store.forecasts[0].temp;
    current->humidity = store.forecasts[0].humidity;

    for (int i = 1; i <= 4; i++) {
        // hourly->main[(i - 1)] = store.forecasts[i].main;
        hourly->main[(i - 1)] = String(store.forecasts[i].weather);
        hourly->temp[(i - 1)] = store.forecasts[i].temp;
        hourly->humidity[(i - 1)] = store.forecasts[i].humidity;
    }

    Serial.println("Done.");
}

struct box DisplayService::createBox(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t background)
{
    box box;

    box.x = x;
    box.y = y;
    box.w = w;
    box.h = h;
    box.background = background;

    return box;
}

String DisplayService::strTime(time_t unixTime)
{
    unixTime += _clock.getGmtOffset();
    return ctime(&unixTime);
}

void DisplayService::strToForecast(String & src, forecast_t & dest)
{
    memset(dest.weather, '\0', sizeof(dest.weather));
    strncpy(dest.weather, src.c_str(), sizeof(dest.weather));
}