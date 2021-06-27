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

struct tm * Clock::getTime()
{
    getLocalTime(&timeInfo);

    return &timeInfo;
}

const char * Clock::getTimeString()
{
    getTime();

    char buff[20];

    strftime(buff, sizeof(buff), "%H:%M", &timeInfo);

    String str(buff);

    return str.c_str();
}