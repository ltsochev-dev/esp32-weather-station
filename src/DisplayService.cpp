#include "DisplayService.h"

DisplayService::DisplayService(GxEPD &display, Weather &weather, Clock &clock, Thermometer &thermo)
    : _display(display), _weather(weather), _clock(clock), _thermometer(thermo)
{
}

void DisplayService::fullUpdate()
{
    loadSources();

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
    Serial.print("Obtaining proper time...");
    _clock.getTime();
    Serial.println("Done.");
    
    Serial.print("Obtaining weather data...");
    _weather.load();
    Serial.println("Done.");

    Serial.print("Obtaining thermometer data...");
    _thermometer.getReading();
    Serial.println("Done.");
}

void DisplayService::drawCurrentWeather()
{
    // @todo
}

void DisplayService::drawRoomTemperature()
{
    // @todo
}

void DisplayService::drawClock()
{
    // @todo
}

void DisplayService::drawForecast()
{
    // This method draws 4 boxes
    uint16_t box_w = floor(_display.width() / 4);
    uint16_t box_h = 55;
    uint16_t box_y =  _display.height() - box_h;

    auto forecast = _weather.getHourlyForecast();

    for (int i = 1; i <= 4; i++) {
        uint16_t box_x = box_w * i;

        _display.fillRect(box_x, box_y, box_w, box_h, GxEPD_BLACK);
        _display.setCursor(box_x, box_y + 20);
        _display.print(forecast->main[i]);
        _display.setCursor(box_x, box_y + 35);
        _display.print(forecast->temp[i]);
    }
}