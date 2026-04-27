#pragma once

#include <string>
#include <vector>

struct Time {
    int day, suzukake_day_count, ookayama_day_count, section, max_assign;
    std::vector<std::string> time_window_label;
    std::vector<int> time, accumulate;
    std::vector<int> count_per_day;
    std::vector<int> accumulate_count_per_day;
    std::vector<int> suzukake_slots, ookayama_slots;  // Lists of slot indices

    void init();
    void parse_campus_from_labels();
};
