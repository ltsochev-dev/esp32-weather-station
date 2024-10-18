// Client library for the OpenWeatherMap data-point server
// https://openweathermap.org/

// The API server uses https, so a client library with secure support is needed

// Created by Bodmer 9/4/2020
// This is a beta test version and is subject to change!
// Insecure mode added by ADAMSIN12

// See license.txt in root folder of library

#define MAX_ICON_INDEX 11 // Maximum for weather icon index
#define ICON_RAIN 1       // Index for the rain icon bitmap (bmp file)
#define NO_VALUE 11       // for precipType default (none)

#ifndef OpenWeather_h
#define OpenWeather_h

#include "User_Setup.h"
#include "Data_Point_Set.h"


/***************************************************************************************
** Description:   JSON interface class
***************************************************************************************/
class OW_Weather: public JsonListener {

  public:
    // Sketch calls this forecast request, it returns true if no parse errors encountered
    // ESP8266 only: setting secure to false will invoke an insecure connection
    bool getForecast(OW_current *current, OW_hourly *hourly, OW_daily  *daily,
                     String api_key, String base_url, String latitude, String longitude,
                     String units, String language, bool secure, bool freeVersion, bool fullSet);

    // Called by library (or user sketch), sends a GET request to a https (secure) url
    bool parseRequest(String url); // and parses response, returns true if no parse errors

    // Called by library (or user sketch), sends a GET request to a http (insecure) url
    bool parseRequestSecure(String* url); 
    bool parseRequestInsecure(String* url);

    float    lat = 0;
    float    lon = 0;
    String   timezone = "";

  private: // Streaming parser callback functions, allow tracking and decisions

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

    void fullDataSet(const char *value);    // Populate structure with full data set
    void freeDataSet(const char *value);    // Populate structure with data of free plan
    void partialDataSet(const char *value); // Populate structure with minimal data set


  private: // Variables used internal to library

    uint16_t hourly_index;   // index into the OW_hourly structure's data arrays
    uint16_t daily_index;    // index into the OW_daily structure's data arrays

    // The value storage structures are created and deleted by the sketch and
    // a pointer passed via the library getForecast() call the value() function
    // is then used to populate the structs with values
    OW_current  *current;  // pointer provided by sketch to the OW_current struct
    OW_hourly   *hourly;   // pointer provided by sketch to the OW_hourly struct
    OW_daily    *daily;    // pointer provided by sketch to the OW_daily struct

    String      valuePath;  // object (i.e. sequential key) path (like a "file path")
                            // taken to the name:value pair in the form "hourly/data"
                            // so values can be pulled from the correct array.
                            // Needed since different objects contain "data" arrays.

    String data_set;        // A copy of the last object name at the head of an array
                            // short equivalent to path.

    bool     parseOK;       // true if the parse been completed
                            // (does not mean data values gathered are good!)

    bool     fullSet = false;       // Set true for partial data set acquisition
    bool     freeSet = true;       // Set true if you use the free version of OpenWeather

    String   currentParent; // Current object e.g. "daily"
    uint16_t objectLevel;   // Object level, increments for new object, decrements at end
    String   currentKey;    // Name key of the name:value pair e.g "temperature"
    String   currentSet;    // Name key of the data set
    String   arrayPath;     // Path to name:value pair e.g.  "daily/data"
    uint16_t arrayIndex;    // Array index e.g. 5 for day 5 forecast, qualify with arrayPath
    uint16_t arrayLevel;    // Array level

    bool     Secure = true; // Link security setting secure (https) or insecure (http)
    uint16_t port;          // 
};

/***************************************************************************************
***************************************************************************************/
#endif
