// The structures below are the repository for the data values extracted from the
// JSON message. The structures are popolated with the extracted data by the "value()"
// member function in the main OpenWeather.cpp file.

// Some structs contain arrays so watch out for memory consumption. You can
// request a subset of the full weather report but this library grabs all values with
// one GET request to avoid exceeding the 1000 free request count per day (count reset
// at 00:00 UTC). 1000 per day means ~40 per hour. As the weather forcast changes slowly
// the example requests the forecast every 15 minutes, so adapting to reduce memory
// by requesting current, daily, hourly etc forescasts individually can be done.

// The content is zero or "" when first created.

/***************************************************************************************
** Description:   Structure for current weather
***************************************************************************************/
typedef struct OW_current {

  // current
  uint32_t dt = 0;
  uint32_t sunrise = 0;
  uint32_t sunset = 0;
  float    temp = 0;
  float    feels_like = 0;
  float    pressure = 0;
  uint8_t  humidity = 0;
  float    dew_point = 0;
  uint8_t  clouds = 0;
  float    uvi = 0;
  uint32_t visibility = 0;
  float    wind_speed = 0;
  float    wind_gust = 0;
  uint16_t wind_deg = 0;
  float    rain = 0;
  float    snow = 0;

  // current.weather
  uint16_t id = 0;
  String   main;
  String   description;
  String   icon;

} OW_current;

/***************************************************************************************
** Description:   Structure for hourly weather
***************************************************************************************/
typedef struct OW_hourly {

  // hourly
  uint32_t dt[MAX_HOURS] = { 0 };
  float    temp[MAX_HOURS] = { 0 };
  float    feels_like[MAX_HOURS] = { 0 };
  float    pressure[MAX_HOURS] = { 0 };
  uint8_t  humidity[MAX_HOURS] = { 0 };
  float    dew_point[MAX_HOURS] = { 0 };
  uint8_t  clouds[MAX_HOURS] = { 0 };
  float    wind_speed[MAX_HOURS] = { 0 };
  float    wind_gust[MAX_HOURS] = { 0 };
  uint16_t wind_deg[MAX_HOURS] = { 0 };
  float    rain[MAX_HOURS] = { 0 };
  float    snow[MAX_HOURS] = { 0 };

  // hourly.weather
  uint16_t id[MAX_HOURS] = { 0 };
  String   main[MAX_HOURS];
  String   description[MAX_HOURS];
  String   icon[MAX_HOURS];
  float    pop[MAX_HOURS];
} OW_hourly;

/***************************************************************************************
** Description:   Structure for daily weather
***************************************************************************************/
typedef struct OW_daily {

  // daily
  uint32_t dt[MAX_DAYS] = { 0 };  // dt
  uint32_t sunrise[MAX_DAYS] = { 0 };
  uint32_t sunset[MAX_DAYS] = { 0 };
  
  // daily.temp
  float    temp_morn[MAX_DAYS] = { 0 };
  float    temp_day[MAX_DAYS] = { 0 };
  float    temp_eve[MAX_DAYS] = { 0 };
  float    temp_night[MAX_DAYS] = { 0 };
  float    temp_min[MAX_DAYS] = { 0 };
  float    temp_max[MAX_DAYS] = { 0 };

  // daily.feels_like
  float    feels_like_morn[MAX_DAYS] = { 0 };
  float    feels_like_day[MAX_DAYS] = { 0 };
  float    feels_like_eve[MAX_DAYS] = { 0 };
  float    feels_like_night[MAX_DAYS] = { 0 };

  // daily
  float    pressure[MAX_DAYS] = { 0 };
  uint8_t  humidity[MAX_DAYS] = { 0 };
  float    dew_point[MAX_DAYS] = { 0 };
  float    wind_speed[MAX_DAYS] = { 0 };
  float    wind_gust[MAX_DAYS] = { 0 };
  uint16_t wind_deg[MAX_DAYS] = { 0 };
  uint8_t  clouds[MAX_DAYS] = { 0 };
  float    uvi[MAX_DAYS] = { 0 };
  uint32_t visibility[MAX_DAYS] = { 0 };

  float    rain[MAX_DAYS] = { 0 };
  float    snow[MAX_DAYS] = { 0 };

  // hourly.weather
  uint16_t id[MAX_DAYS] = { 0 };
  String   main[MAX_DAYS];
  String   description[MAX_DAYS];
  String   icon[MAX_DAYS];
  float    pop[MAX_DAYS];

} OW_daily;


// Structures for minimal set of data points for TFT_eSPI examples to reduce RAM needs
/*
typedef struct OW_current {

  //float    lat = 0;
  //float    lon = 0;
  //String   timezone;

  // current
  uint32_t dt = 0;
  uint32_t sunrise = 0;
  uint32_t sunset = 0;
  float    temp = 0;
  //float    feels_like = 0;
  float    pressure = 0;
  uint8_t  humidity = 0;
  //float    dew_point = 0;
  uint8_t  clouds = 0;
  //uint8_t  uvi = 0;
  //uint32_t visibility = 0;
  float    wind_speed = 0;
  //float    wind_gust = 0;
  uint16_t wind_deg = 0;
  //float    rain = 0;
  //float    snow = 0;

  // current.weather
  uint16_t id = 0;
  String   main;
  //String   description;
  //String   icon;

} OW_current;


typedef struct OW_hourly {

} OW_hourly;

typedef struct OW_daily {

  // daily
  uint32_t dt[MAX_DAYS] = { 0 };  // dt
  //uint32_t sunrise = 0;
  //uint32_t sunset = 0;
  
  // daily.temp
  //float    temp_morn[MAX_DAYS] = { 0 };
  //float    temp_day[MAX_DAYS] = { 0 };
  //float    temp_eve[MAX_DAYS] = { 0 };
  //float    temp_night[MAX_DAYS] = { 0 };
  float    temp_min[MAX_DAYS] = { 0 };
  float    temp_max[MAX_DAYS] = { 0 };

  // daily.feels_like
  //float    feels_like_morn[MAX_DAYS] = { 0 };
  //float    feels_like_day[MAX_DAYS] = { 0 };
  //float    feels_like_eve[MAX_DAYS] = { 0 };
  //float    feels_like_night[MAX_DAYS] = { 0 };

  // daily
  //float    pressure[MAX_DAYS] = { 0 };
  //uint8_t  humidity[MAX_DAYS] = { 0 };
  //float    dew_point[MAX_DAYS] = { 0 };
  //float    wind_speed[MAX_DAYS] = { 0 };
  //float    wind_gust[MAX_DAYS] = { 0 };
  //uint16_t wind_deg[MAX_DAYS] = { 0 };
  //uint8_t  clouds[MAX_DAYS] = { 0 };
  //uint8_t  uvi[MAX_DAYS] = { 0 };
  //uint32_t visibility[MAX_DAYS] = { 0 };

  //float    rain[MAX_DAYS] = { 0 };
  //float    snow[MAX_DAYS] = { 0 };

  // hourly.weather
  uint16_t id[MAX_DAYS] = { 0 };
  //String   main[MAX_DAYS];
  //String   description[MAX_DAYS];
  //String   icon[MAX_DAYS];

} OW_daily;
*/