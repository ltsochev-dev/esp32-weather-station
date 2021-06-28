#ifndef LTSOCHEV_DISPLAYMANGER_H
#define LTSOCHEV_DISPLAYMANGER_H

#include <GxEPD.h>
#include <U8g2_for_Adafruit_GFX.h>
#include <time.h>
#include <sys/time.h>
#include "thermometer.h"
#include "weather.h"
#include "Clock.h"
#include "box.h"
#include "DisplayData.h"

class DisplayService
{
public:
    DisplayService(GxEPD&, Weather&, Clock&, Thermometer&);
    void begin();
    void fullUpdate();
    void update();
    void serialize(displaydata_t&);
    void unserialize(displaydata_t&);
private:
    void loadSources();
    void drawClock();
    void drawCurrentWeather();
    void drawRoomTemperature();
    void drawForecast();
    struct box createBox(uint16_t,uint16_t,uint16_t,uint16_t, uint16_t);
    String strTime(time_t);
    void strToForecast(String&, forecast_t&);
private:
    GxEPD &_display;
    Weather &_weather;
    Clock &_clock;
    Thermometer &_thermometer;
    U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;
};

#endif