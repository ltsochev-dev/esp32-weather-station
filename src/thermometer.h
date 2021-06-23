#ifndef LTSOCHEV_THERMOMETER_H
#define LTSOCHEV_THERMOMETER_H

#include <Arduino.h>
#include <GxEPD.h>

class Thermometer
{
public:
    Thermometer(GxEPD& display, int16_t pin);
    void begin();
    void draw();
    float getReading();
    float getLastReading();
private:
    uint32_t readAdc(int ADC_raw);
    GxEPD &_display;
    int16_t _adc;
    float lastReading = 0;
};

#endif