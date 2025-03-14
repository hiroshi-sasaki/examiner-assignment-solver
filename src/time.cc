#include "time.h"

void Time::init() {
    count_per_day.assign(day, 0);
    for (int i = 0; i < day; i++) {
        for (int j = 0; j < section; j++) {
            count_per_day[i] += time[i * section + j];
        }
    }
    accumulate_count_per_day = count_per_day;
    accumulate_count_per_day.insert(accumulate_count_per_day.begin(), 0);
    for (int i = 0; i < day; i++) {
        accumulate_count_per_day[i + 1] =
            accumulate_count_per_day[i] + count_per_day[i];
    }

    accumulate = time;
    accumulate.insert(accumulate.begin(), 0);
    for (int i = 0; i < day; i++) {
        for (int j = 0; j < section; j++) {
            accumulate[i * section + j + 1] =
                accumulate[i * section + j] + time[i * section + j];
        }
    }
}