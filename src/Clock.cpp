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

void Clock::refresh()
{
    Serial.print("Refreshing clock....");
    setTimeZone(-gmtOffset_sec, daylightOffset_sec);
    Serial.println("Done.");
    // setTimeZone(-gmtOffset_sec, daylightOffset_sec);
    // getTime();
}

void Clock::setTimeZone(long offset, int daylight)
{
    char cst[17] = {0};
    char cdt[17] = "DST";
    char tz[33] = {0};

    if(offset % 3600){
        sprintf(cst, "UTC%ld:%02u:%02u", offset / 3600, abs((offset % 3600) / 60), abs(offset % 60));
    } else {
        sprintf(cst, "UTC%ld", offset / 3600);
    }
    if(daylight != 3600){
        long tz_dst = offset - daylight;
        if(tz_dst % 3600){
            sprintf(cdt, "DST%ld:%02u:%02u", tz_dst / 3600, abs((tz_dst % 3600) / 60), abs(tz_dst % 60));
        } else {
            sprintf(cdt, "DST%ld", tz_dst / 3600);
        }
    }
    sprintf(tz, "%s%s", cst, cdt);
    setenv("TZ", tz, 1);
    tzset();
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