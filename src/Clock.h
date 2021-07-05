#ifndef LTSOCHEV_CLOCK_H
#define LTSOCHEV_CLOCK_H

#include <Arduino.h>
#include "time.h"

class Clock
{
public:
    Clock(long gmtOffset, long daylightOffset, String ntpServ);
    void begin();
    void refresh();
    const struct tm & getTime();
    struct tm & getTimeRef();
    String getTimeString();
    long getGmtOffset() const;
    time_t getUnixtime();
private:
    static void setTimeZone(long offset, int daylight);
private:
    struct tm timeInfo;
    long gmtOffset_sec = 7200;
    int daylightOffset_sec = 3600;
    String ntpServer = "pool.ntp.org";
};

#endif