#include "io_util.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

int get_column_index(const std::vector<std::string> &row, std::string target) {
    auto itr =
        std::find_if(std::begin(row), std::end(row),
                     [target](std::string s) -> bool { return s == target; });
    if (itr == row.end()) {
        std::cerr << target << "は存在しません" << std::endl;
        assert(0);
    }
    return std::distance(std::begin(row), itr);
}

std::vector<std::string> get_line_split_by_c(std::string row, char c) {
    std::string str_buf;
    std::vector<std::string> line;
    std::istringstream i_stream(row);
    while (std::getline(i_stream, str_buf, c)) {
        if (!str_buf.empty() && std::iscntrl(str_buf.back()))
            str_buf.pop_back();
        line.emplace_back(str_buf);
    }
    return line;
}