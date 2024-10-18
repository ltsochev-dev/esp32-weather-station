#ifndef LTSOCHEV_WEATHER_H
#define LTSOCHEV_WEATHER_H

#include <Arduino.h>
#include <JSON_Decoder.h>
#include <OpenWeather.h>

class Weather
{
public:
    Weather();
    Weather(String lat, String lng, String loc, String apiKey);
    ~Weather();
    void setApiKey(String key);
    void setIsSecureConnection(bool isSecure);
    void setBaseURL(String key);
    void setIsFreeVersion(bool isOWFreeVersion);
    bool getIsFreeVersion();
    void setIsFullDataSet(bool isFullDataSet);
    void setUnits(String unit);
    void setLanguage(String lng);
    void setLocation(String lat, String lng, String loc);
    String getUnits();
    String getLocation();
    void load();
    OW_current * getCurrentForecast();
    OW_hourly * getHourlyForecast();
    OW_daily * getDailyForecast();
protected:
    String latitude;
    String longitude;
    String location;
    String openWeatherKey;
    String baseURL;
    bool freeVersion;
    bool secure;
    bool fullSet;
    String units = "metric";  // or "imperial"
    String language = "en";   // See notes tab
private:
    OW_Weather ow;
    OW_current * current;// = new OW_current;
    OW_hourly * hourly;// = new OW_hourly;
    OW_daily  * daily;// = new OW_daily;
};

#endif