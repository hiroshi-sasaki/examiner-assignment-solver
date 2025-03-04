#pragma once

#include <string>
#include <vector>

int get_column_index(const std::vector<std::string> &row, std::string target);

std::vector<std::string> get_line_split_by_c(std::string row, char c);

template <class T> void insert_or_assign(std::vector<T> &a, T x) {
    for (auto &val : a) {
        if (val == x) {
            val = x;
            return;
        }
    }
    a.emplace_back(x);
}