#include <weather.h>

Weather::Weather(String lat, String lng, String loc, String apiKey)
    : latitude(lat.c_str()), longitude(lng.c_str()), location(loc.c_str()), openWeatherKey(apiKey.c_str())
{
    current = new OW_current;
    hourly = new OW_hourly;
    daily = new OW_daily;
}

Weather::Weather()
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

void Weather::setBaseURL(String key)
{
    baseURL = key;
}

void Weather::setIsFreeVersion(bool isOWFreeVersion)
{
    freeVersion = isOWFreeVersion;
}

bool Weather::getIsFreeVersion()
{
    return freeVersion;
}

void Weather::setLocation(String lat, String lng, String loc)
{
    latitude = lat;
    longitude = lng;
    location = loc;
}

void Weather::setIsSecureConnection(bool isSecure)
{
    secure = isSecure;
}

void Weather::setIsFullDataSet(bool isFullDataSet)
{
    fullSet = isFullDataSet;
}

void Weather::load()
{
    if (!ow.getForecast(current, hourly, daily, openWeatherKey, baseURL, latitude, longitude, units, language, secure, freeVersion, fullSet)) {
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