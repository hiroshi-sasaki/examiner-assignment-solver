#include "time.h"

void Time::init() {
    count_per_day.assign(day, 0);
    for(int i = 0; i < day; i++) {
        for(int j = 0; j < section; j++) {
            count_per_day[i] += time[i * section + j];
        }
    }

    accumulate = time;
    accumulate.insert(accumulate.begin(), 0);
    for(int i = 0; i < day; i++) {
        for(int j = 0; j < section; j++) {
            accumulate[i * section + j + 1] = accumulate[i * section + j] + time[i * section + j];
        }
    }
}