#ifndef LTSOCHEV_DISPLAYDATA_H
#define LTSOCHEV_DISPLAYDATA_H

#include <Arduino.h>
#include <OpenWeather.h>

struct Forecast {
    char weather[50];
    float temp;
    unsigned char humidity;
};

typedef struct Forecast forecast_t;

struct DisplayData {
    float roomTemp;
    time_t unixTime;
    forecast_t forecasts[5];
};

typedef struct DisplayData displaydata_t;

#endif