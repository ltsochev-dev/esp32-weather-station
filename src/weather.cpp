#include <weather.h>

Weather::Weather(String lat, String lng, String loc, String apiKey)
    : latitude(lat.c_str()), longitude(lng.c_str()), location(loc.c_str()), openWeatherKey(apiKey.c_str())
{
    current = new OW_current;
    hourly = new OW_hourly;
    daily = new OW_daily;
}

Weather::~Weather()
{
    delete current;
    delete hourly;
    delete daily;
}

void Weather::setApiKey(String key)
{
    openWeatherKey = key;
}

void Weather::load()
{
    if (!ow.getForecast(current, hourly, daily, openWeatherKey, latitude, longitude, units, language)) {
        current->main = "Error";
    }
}

void Weather::setUnits(String unit)
{
    units = unit;
}

void Weather::setLanguage(String lng)
{
    language = lng;
}

String Weather::getUnits()
{
    return units;
}

String Weather::getLocation()
{
    return location;
}

OW_current * Weather::getCurrentForecast()
{
    return current;
}

OW_hourly * Weather::getHourlyForecast()
{
    return hourly;
}

OW_daily * Weather::getDailyForecast()
{
    return daily;
}