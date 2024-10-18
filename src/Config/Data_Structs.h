#pragma once
#include <Arduino.h>
#include <vector>

typedef struct clock_conf_t {
    int gmtOffset, daylightOffset;
    std::vector<String> ntpServers;
} clock_conf_t;

typedef struct location_conf_t {
    float latitude, longitude;
    String location;
} location_conf_t;

typedef struct wifi_conf_t {
    String ssId, ssIdPass;
} wifi_conf_t;

typedef struct openweather_conf_t {
    String owApiKey;
    String baseURL;
    bool isfreeVersion;
    bool isfullDataSet;
    bool isSecureConnection;
} openweather_conf_t;

typedef struct conf_store_t {
    clock_conf_t clock;
    location_conf_t location;
    wifi_conf_t wifi;
    openweather_conf_t openweather;
} conf_store_t;