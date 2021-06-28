#include "Clock.h"

Clock::Clock(long gmtOffset, long daylightOffset, String ntpServ)
    : gmtOffset_sec(gmtOffset), daylightOffset_sec(daylightOffset), ntpServer(ntpServ)
{
}

void Clock::begin()
{
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer.c_str());
    getTime();
}

time_t Clock::getUnixtime()
{
    time_t now;
    
    getTime();

    time(&now);

    return now;
}

const struct tm & Clock::getTime()
{
    getLocalTime(&timeInfo);

    return timeInfo;
}

struct tm & Clock::getTimeRef()
{
    getLocalTime(&timeInfo);

    return timeInfo;
}

String Clock::getTimeString()
{
    getTime();

    char buff[20];

    strftime(buff, sizeof(buff), "%H:%M", &timeInfo);

    return String(buff);
}

long Clock::getGmtOffset() const 
{
    return gmtOffset_sec;
}