#pragma once

#include <string>
#include <vector>

struct Time {
    int day, section;
    std::vector<std::string> time_window_label;
    std::vector<int> time, accumulate;
    std::vector<int> count_per_day;
    std::vector<int> accumulate_count_per_day;

    void init();
};