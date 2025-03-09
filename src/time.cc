#include "time.h"

void Time::init() {
    count_per_day.assign(day, 0);
    for(int i = 0; i < day; i++) {
        for(int j = 0; j < section; j++) {
            count_per_day[i] += time[i * section + j];
        }
    }
    auto start_time = count_per_day;
    start_time.insert(start_time.begin(), 0);
    for(int i = 0; i < day; i++) {
        start_time[i+1] = start_time[i] + count_per_day[i];
    }

    accumulate = time;
    accumulate.insert(accumulate.begin(), 0);
    for(int i = 0; i < day; i++) {
        for(int j = 0; j < section; j++) {
            accumulate[i * section + j + 1] = accumulate[i * section + j] + time[i * section + j];
        }
    }
}