#include "thermometer.h"
#include "esp_adc_cal.h"

Thermometer::Thermometer(GxEPD &display, int16_t pin) :
    _display(display), _adc(pin)
{
}

void Thermometer::begin()
{
    pinMode(_adc, INPUT);

    getReading();
}

float Thermometer::getReading()
{
    int rawReading = analogRead(_adc);

    uint32_t Voltage = readAdc(rawReading);

    lastReading = Voltage / 10;

    return lastReading;
}

float Thermometer::getLastReading()
{
    return lastReading;
}

uint32_t Thermometer::readAdc(int ADC_raw)
{
    esp_adc_cal_characteristics_t adc_chars;

    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);

    return esp_adc_cal_raw_to_voltage(ADC_raw, &adc_chars);
}