#ifndef LTSOCHEV_CLOCK_H
#define LTSOCHEV_CLOCK_H

#include <Arduino.h>
#include <time.h>
#include <NTPClient.h>

class Clock
{
public:
    Clock(long gmtOffset, long daylightOffset, String ntpServ);
    void begin();
    struct tm * getTime();
    const char * getTimeString();
private:
    struct tm timeInfo;
    String ntpServer = "pool.ntp.org";
    long gmtOffset_sec = 7200;
    int daylightOffset_sec = 3600;
};

#endif