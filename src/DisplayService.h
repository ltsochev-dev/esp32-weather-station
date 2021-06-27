#ifndef LTSOCHEV_DISPLAYMANGER_H
#define LTSOCHEV_DISPLAYMANGER_H

#include <GxEPD.h>
#include "thermometer.h"
#include "weather.h"
#include "Clock.h"
#include "box.h"

class DisplayService
{
public:
    DisplayService(GxEPD&, Weather&, Clock&, Thermometer&);
    void fullUpdate();
    void update();
private:
    void loadSources();
    void drawClock();
    void drawCurrentWeather();
    void drawRoomTemperature();
    void drawForecast();
private:
    GxEPD &_display;
    Weather &_weather;
    Clock &_clock;
    Thermometer &_thermometer;
};

#endif