#ifndef DATE_H
#define DATE_H

#include <time.h>
#include "String.h"

namespace yak
{
    class Date
    {
    public:
        enum Mode { Local, UTC };

        Date();
        Date(time_t time, Mode mode = UTC);

        bool isEpoch() const { return !mTime; }

        int date(Mode mode = UTC) const; // day of month, 1-31
        int day(Mode mode = UTC) const; // day of week, 0-6
        int year(Mode mode = UTC) const; // year, four digits
        int hours(Mode mode = UTC) const; // hour, 0-23
        int minutes(Mode mode = UTC) const; // minutes, 0-59
        int month(Mode mode = UTC) const; // month, 0-11
        int seconds(Mode mode = UTC) const; // seconds, 0-59. Does this need Mode?

        time_t time(Mode mode = UTC) const;
        void setTime(time_t time, Mode mode = UTC);

        enum TimeFormat {
            DateTimeFmt,
            TimeFmt,
            DateFmt
        };

        static Date Now() { return Date(::time(nullptr)); };
        
        String toString(TimeFormat fmt = DateTimeFmt){
            const char *format = 0;
            switch (fmt) {
            case DateTimeFmt:
                format = "%Y-%m-%d %H:%M:%S";
                break;
            case DateFmt:
                format = "%Y-%m-%d";
                break;
            case TimeFmt:
                format = "%H:%M:%S";
                break;
            }

            char buf[32];
            tm tm;
            localtime_r(&mTime, &tm);
            const size_t w = strftime(buf, sizeof(buf), format, &tm);
            return String(buf, w);

        }
    private:
        time_t mTime;

    };
}

#endif
