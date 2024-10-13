#ifndef LTSOCHEV_CONFIG_H
#define LTSOCHEV_CONFIG_H

#include <JSON_Listener.h>
#include <JSON_Decoder.h>
#include <SPIFFS.h>
#include "Data_Structs.h"

class Config : JsonListener
{
public:
    Config(const char*, fs::SPIFFSFS&);
    ~Config();
    bool begin();
    const conf_store_t * getData() const;
    String getWifiSSID() const;
    String getWifiPassword() const;
    location_conf_t getLocationConfig() const;
    openweather_conf_t getOpenWeatherConfig() const;
private:
    void startDocument(); // JSON document has started, typically starts once
                          // Initialises variables used, e.g. sets objectLayer = 0
                          // and arrayIndex =0
    void endDocument();   // JSON document has ended, typically ends once

    void startObject();   // Called every time an Object start detected
                          // may be called multiple times as object layers entered
                          // Used to increment objectLayer
    void endObject();     // Called every time an object ends
                          // Used to decrement objectLayer and zero arrayIndex

    void startArray();    // An array of name:value pairs entered
    void endArray();      // Array member ended, increments arrayIndex

    void key(const char *key);            // The current "object" or "name for a name:value pair"
    void value(const char *value);        // String value from name:value pair e.g. "1.23" or "rain"

    void whitespace(char c);              // Whitespace character in JSON - not used

    void error( const char *message );    // Error message is sent to serial port
private:
    String path;
    conf_store_t * store;
    fs::SPIFFSFS & fsys;

    String      valuePath;  // object (i.e. sequential key) path (like a "file path")
                            // taken to the name:value pair in the form "hourly/data"
                            // so values can be pulled from the correct array.
                            // Needed since different objects contain "data" arrays.

    String   currentParent; // Current object e.g. "daily"
    uint16_t objectLevel;   // Object level, increments for new object, decrements at end
    String   currentKey;    // Name key of the name:value pair e.g "temperature"
    String   currentSet;    // Name key of the data set
    String   arrayPath;     // Path to name:value pair e.g.  "daily/data"
    uint16_t arrayIndex;    // Array index e.g. 5 for day 5 forecast, qualify with arrayPath
    uint16_t arrayLevel;    // Array level

    String data_set;        // A copy of the last object name at the head of an array
                            // short equivalent to path.
    bool parseOK;
};

#endif