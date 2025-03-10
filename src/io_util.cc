#include "io_util.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include "professor.h"
#include "time.h"

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

Time time_info_input(std::string filename) {
    std::ifstream file(filename, std::ios::in);
    Time time;
    if (!file) {
        std::cerr << "cannot open file: " << filename << std::endl;
    }
    std::string str_buf;
    {
        std::getline(file, str_buf);
        auto line = get_line_split_by_c(str_buf, ' ');
        time.day = std::stoi(line[0]);
        time.section = std::stoi(line[1]);
    }
    {
        std::getline(file, str_buf);
        auto line = get_line_split_by_c(str_buf, ' ');
        for (auto x : line) {
            time.time.emplace_back(std::stoi(x));
        }
        assert(time.time.size() == time.day * time.section);
    }
    {
        std::getline(file, str_buf);
        time.time_window_label = get_line_split_by_c(str_buf, ',');
    }
    time.init();
    return time;
}

std::vector<Professor> professor_base_info_input(std::string filename) {
    std::ifstream file(filename, std::ios::in);
    std::string str_buf;
    std::vector<Professor> professors;
    std::getline(file, str_buf);
    int name_index, campus_index, work_position_index, affiliation_index;
    {
        auto line = get_line_split_by_c(str_buf, ',');
        name_index = get_column_index(line, "氏名");
        campus_index = get_column_index(line, "キャンパス");
        work_position_index = get_column_index(line, "役職");
        affiliation_index = get_column_index(line, "所属研究室");
    }
    while (std::getline(file, str_buf)) {
        auto line = get_line_split_by_c(str_buf, ',');
        std::string name = line[name_index];
        std::string campus = line[campus_index];

        ProfessorType type;
        if (line[work_position_index] == "教授") {
            type = ProfessorType::professor;
        } else if (line[work_position_index] == "准教授") {
            type = ProfessorType::associate;
        } else if (line[work_position_index] == "助教") {
            type = ProfessorType::assistant;
        } else {
            std::cerr << line[work_position_index] << std::endl;
            assert(0);
        }

        std::string affiliation = line[affiliation_index];

        insert_or_assign(professors, {name, campus, type, affiliation});
    }
    return professors;
}