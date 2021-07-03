#include "Config.h"

Config::Config(const char * spiffPath, fs::SPIFFSFS & spiffs)
    : path(spiffPath), fsys(spiffs)
{
    store = new conf_store_t;
}

Config::~Config()
{
    delete store;
}

bool Config::begin()
{
    char c = 0;
    parseOK = false;

    JSON_Decoder parser;
    parser.setListener(this);

    File file = fsys.open(path, "r");
    if (!file) {
        Serial.println("Failed to open settings file. Check your SPIFFS contents!");
        return false;
    }

    while(file.available() > 0) {
        c = file.read();
        parser.parse(c);
    }

    parser.reset();

    file.close();

    return parseOK;
}

const conf_store_t * Config::getData() const
{
    return store;
}

void Config::startDocument()
{
    currentParent = currentKey =   currentSet = "";
    objectLevel = 0;
    valuePath = "";
    arrayIndex = 0;
    arrayLevel = 0;
    parseOK = true;
}

void Config::endDocument()
{
    currentParent = currentKey = "";
    objectLevel = 0;
    valuePath = "";
    arrayIndex = 0;
    arrayLevel = 0;
}

void Config::startObject()
{
    if (arrayIndex == 0 && objectLevel == 1) {
        currentParent = currentKey;
    }

    currentSet = currentKey;

    objectLevel++;
}

void Config::endObject()
{
    if (arrayLevel == 0) {
        currentParent = "";
    }

    if (arrayLevel == 1  && objectLevel == 2) {
        arrayIndex++;
    }

    objectLevel--;
}

void Config::startArray()
{
    arrayLevel++;
    valuePath = currentParent + "/" + currentKey; // aka = current Object, e.g. "daily:data"
}

void Config::endArray()
{
    if (arrayLevel > 0) arrayLevel--;
    if (arrayLevel == 0) arrayIndex = 0;
    valuePath = "";
}

void Config::key(const char *key)
{
    currentKey = key;
}

void Config::value(const char *value)
{
    if (!partialSet) fullDataSet(value);
    else partialDataSet(value);
}

void Config::whitespace(char c)
{
    c = c; // Avoid warning
}

void Config::error( const char *message )
{
    Serial.print("\nParse error message: ");
    Serial.print(message);
    parseOK = false;
}

void Config::fullDataSet(const char *value)
{
    String val = value;

    if (currentParent == "clock") {
        data_set = "clock";
        if (currentKey == "gmt_offset") store->clock.gmtOffset = (int)val.toInt();
        if (currentKey == "daylight_offset") store->clock.daylightOffset = (int)val.toInt();
        if (currentKey == "ntp_servers") store->clock.ntpServers.push_back(val);

        return;
    }

    if (currentParent == "location") {
        data_set = "location";
        if (currentKey == "latitide") store->location.latitude = val.toFloat();
        if (currentKey == "longitude") store->location.longitude = val.toFloat();
        if (currentKey == "location") store->location.location = val;
        if (currentKey == "ow_key") store->location.owApiKey = val;

        return;
    }

    if (currentParent == "wifi") {
        data_set = "wifi";
        if (currentKey == "id") store->wifi.ssId = val;
        if (currentKey == "pass") store->wifi.ssIdPass = val;

        return;
    }
}

void Config::partialDataSet(const char *value)
{
    String val = value;

    if (currentParent == "clock") {
        data_set = "clock";
        if (currentKey == "gmt_offset") store->clock.gmtOffset = (int)val.toInt();
        else
        if (currentKey == "daylight_offset") store->clock.daylightOffset = (int)val.toInt();
        else
        if (currentKey == "ntp_servers") store->clock.ntpServers.push_back(val);

        return;
    }

    if (currentParent == "location") {
        data_set = "location";
        if (currentKey == "latitide") store->location.latitude = val.toFloat();
        else
        if (currentKey == "longitude") store->location.longitude = val.toFloat();
        else
        if (currentKey == "location") store->location.location = val;
        else
        if (currentKey == "ow_key") store->location.owApiKey = val;

        return;
    }

    if (currentParent == "wifi") {
        data_set = "wifi";
        if (currentKey == "id") store->wifi.ssId = val;
        else
        if (currentKey == "pass") store->wifi.ssIdPass = val;

        return;
    }
}

String Config::getWifiSSID() const
{
    return store->wifi.ssId;
}

String Config::getWifiPassword() const
{
    return store->wifi.ssIdPass;
}