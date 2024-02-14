#include <stdio.h>
#include <stdlib.h>
#include "timelib.h"

#define NumSecOra 3600
#define NumSecZi 86400
#define NumSecMin 60
#define UnixYear 1970
#define unu 1
#define doi 2
#define patru 4
#define sase 6
#define noua 9
#define unspe 11
#define doispe 12
#define treizeci 30
#define doi 2
#define doiopt 28
#define doinoua 29
#define treiunu 31
#define doipatru 24
#define osuta 100
#define patrusute 400
#define an 365
#define anbisect 366
#define ui unsigned int

TTime convertUnixTimestampToTime(unsigned int timestamp) {
    TTime result;
    result.sec = 0;
    result.min = 0;
    result.hour = 0;

    result.hour = (timestamp / NumSecOra) % doipatru;
    result.min = (timestamp / NumSecMin) % NumSecMin;
    result.sec = timestamp % NumSecMin;

    return result;
}

int daysInMonth(unsigned int month, unsigned int year) {
    switch (month) {
        case patru: case sase: case noua: case unspe: return treizeci;
        case doi: return doiopt;
        default: return treiunu;
    }
}

TDate convertUnixTimestampToDateWithoutLeapYears(unsigned int timestamp) {
    TDate result;
    result.day = unu;
    result.month = unu;
    result.year = UnixYear;
    unsigned int days = timestamp / NumSecZi;

    while (days > 0) {
        result.day++;
        if (result.day > daysInMonth(result.month, result.year)) {
            result.day = unu;
            result.month++;
            if (result.month > doispe) {
                result.month = unu;
                result.year++;
            }
        }
        days--;
    }

    return result;
}

TDate convertUnixTimestampToDate(unsigned int timestamp) {
    TDate result;
    result.day = unu;
    result.month = unu;
    result.year = UnixYear;
    unsigned int days = timestamp / NumSecZi;

    while (days) {
        int daysInMonth = 0;

        switch (result.month) {
            case patru: case sase: case noua: case unspe:
                daysInMonth = treizeci;
                break;
            case doi:
                if ((result.year % patru == 0 && result.year % osuta != 0) || (result.year % patrusute == 0)) {
                    daysInMonth = doinoua;
                } else {
                    daysInMonth = doiopt;
                }
                break;
            default:
                daysInMonth = treiunu;
                break;
        }

        if (result.day == daysInMonth) {
            result.day = unu;
            if (result.month == doispe) {
                result.month = unu;
                result.year++;
            } else {
                result.month++;
            }
        } else {
            result.day++;
        }

        days--;
    }

    return result;
}

TDateTimeTZ convertUnixTimestampToDateTimeTZ(unsigned int timestamp, TTimezone *timezones, int timezone_index) {
    TDateTimeTZ dateTime;

    unsigned int new_timestamp = timestamp + timezones[timezone_index].utc_hour_difference * NumSecOra;
    dateTime.date = convertUnixTimestampToDate(new_timestamp);
    dateTime.time = convertUnixTimestampToTime(new_timestamp % NumSecZi);

    dateTime.tz = &timezones[timezone_index];

    return dateTime;
}

unsigned int convertDateTimeTZToUnixTimestamp(TDateTimeTZ x) {
    unsigned int timestamp_sec = (ui)x.time.sec + (ui)x.time.min * NumSecMin + (ui)x.time.hour * NumSecOra;

    unsigned int timestamp_day = 0;
    for (int month = unu; month < (unsigned int)x.date.month; ++month) {
        switch (month) {
            case patru: case sase: case noua: case unspe:
                timestamp_day += treizeci;
                break;
            case 2:
                if ((x.date.year % patru == 0 && x.date.year % osuta != 0) || (x.date.year % patrusute == 0))
                    timestamp_day += doinoua;
                else
                    timestamp_day += doiopt;
                break;
            default:
                timestamp_day += treiunu;
        }
    }
    for (int year = UnixYear; year < x.date.year; ++year) {
        timestamp_day += (year % patru == 0 && year % osuta != 0) || (year % patrusute == 0) ? anbisect : an;
    }
    timestamp_day += (x.date.day - unu);

    unsigned int timestamp_utc_difference = x.tz->utc_hour_difference * NumSecOra;
    unsigned int unix_timestamp = timestamp_sec + timestamp_day * NumSecZi - timestamp_utc_difference;
    return unix_timestamp;
}

void printDateTimeTZ(TDateTimeTZ datetimetz) {
    const char *months[] = {
        "ianuarie", "februarie", "martie", "aprilie",
        "mai", "iunie", "iulie", "august",
        "septembrie", "octombrie", "noiembrie", "decembrie"
    };
    printf("%02d %s %d, ", datetimetz.date.day, months[datetimetz.date.month - 1], datetimetz.date.year);
    printf("%02d:%02d:%02d ", datetimetz.time.hour, datetimetz.time.min, datetimetz.time.sec);

    if (datetimetz.tz->utc_hour_difference < 0) {
        printf("%s (UTC%d)", datetimetz.tz->name, datetimetz.tz->utc_hour_difference);
    } else {
        printf("%s (UTC+%d)", datetimetz.tz->name, datetimetz.tz->utc_hour_difference);
    }
}
