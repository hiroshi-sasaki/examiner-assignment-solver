#pragma once

#include <string>
#include <vector>

struct Time {
    int day, section;
    std::vector<std::string> time_window_label;
    std::vector<int> time;
};