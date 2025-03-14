#include "io_util.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

#include "professor.h"
#include "student.h"
#include "time.h"

namespace filename {

std::string professor_base_info_filename = "base/professor_base_info.csv";
std::string student_label_filename = "base/student_label.csv";
std::string professor_label_filename = "base/professor_label.csv";

}  // namespace filename

int get_column_index(const std::vector<std::string> &row, std::string target) {
    auto itr =
        std::find_if(std::begin(row), std::end(row),
                     [target](std::string s) -> bool { return s == target; });
    if (itr == row.end()) {
        return -1;
    }
    return std::distance(std::begin(row), itr);
}

int get_column_index(const std::vector<std::string> &row,
                     std::vector<std::string> targets) {
    for (auto target : targets) {
        int index = get_column_index(row, target);
        if (index < 0) continue;
        return index;
    }
    std::cerr << targets[0] << "という列は存在しません" << std::endl;
    assert(0);
    return -1;
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
        if (line.size() == 1) {
            time.section = std::stoi(line[0]);
            std::getline(file, str_buf);
            time.time_window_label = get_line_split_by_c(str_buf, ',');
            return time;
        } else if (line.size() == 2) {
            time.day = std::stoi(line[0]);
            time.section = std::stoi(line[1]);
        } else {
            time.suzukake_day_count = std::stoi(line[0]);
            time.oookayama_day_count = std::stoi(line[1]);
            time.section = std::stoi(line[2]);
            time.day = time.suzukake_day_count + time.oookayama_day_count;
        }
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
    assert(time.time_window_label.size() == time.day * time.section);
    time.init();
    return time;
}

StudentInputLabel student_label_input() {
    StudentInputLabel label;
    std::ifstream file(filename::student_label_filename, std::ios::in);
    std::string str_buf;
    std::getline(file, str_buf);
    label.number_labels = get_line_split_by_c(str_buf, ',');
    std::getline(file, str_buf);
    label.name_labels = get_line_split_by_c(str_buf, ',');
    std::getline(file, str_buf);
    label.supervisor_labels = get_line_split_by_c(str_buf, ',');
    std::getline(file, str_buf);
    label.main_examiner_labels = get_line_split_by_c(str_buf, ',');
    std::getline(file, str_buf);
    label.deputy_examiner_labels = get_line_split_by_c(str_buf, ',');
    std::getline(file, str_buf);
    label.first_professor_labels = get_line_split_by_c(str_buf, ',');
    std::getline(file, str_buf);
    label.second_professor_labels = get_line_split_by_c(str_buf, ',');
    std::getline(file, str_buf);
    label.other_labels = get_line_split_by_c(str_buf, ',');
    return label;
}

ProfessorInputLabel professor_label_input() {
    ProfessorInputLabel labels;
    std::ifstream file(filename::professor_label_filename, std::ios::in);
    std::string str_buf;
    std::getline(file, str_buf);
    labels.name_labels = get_line_split_by_c(str_buf, ',');
    return labels;
}

std::vector<Professor> professor_base_info_input() {
    std::ifstream file(filename::professor_base_info_filename, std::ios::in);
    std::string str_buf;
    std::vector<Professor> professors;
    std::getline(file, str_buf);
    int name_index, campus_index, work_position_index, affiliation_index,
        alias_index;
    {
        auto line = get_line_split_by_c(str_buf, ',');
        name_index = get_column_index(line, "氏名");
        campus_index = get_column_index(line, "キャンパス");
        work_position_index = get_column_index(line, "役職");
        affiliation_index = get_column_index(line, "所属研究室");
        alias_index = get_column_index(line, "別表記");
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

        std::vector<std::string> alias = {name};
        for (auto s : line | std::views::drop(alias_index)) {
            alias.emplace_back(s);
        }

        insert_or_assign(professors, {name, campus, type, affiliation, alias});
    }
    return professors;
}

void output_schedule(std::vector<Student> schedule, Time time_info) {
    assert(schedule.size() == time_info.accumulate.back());
    std::cout << "発表番号,学籍番号,氏名,審査員" << std::endl;
    for (int i = 0; i < time_info.day * time_info.section; i++) {
        std::cout << time_info.time_window_label[i] << std::endl;
        for (int j = 0; j < time_info.time[i]; j++) {
            int index = time_info.accumulate[i] + j;
            if (schedule[index].valid()) {
                std::cout << index + 1 << "," << schedule[index].get_number()
                          << "," << schedule[index].get_name();
                for (auto professor : schedule[index].get_assign_professors()) {
                    std::cout << "," << professor;
                }
                std::cout << std::endl;
            } else {
                std::cout << "空き" << std::endl;
            }
        }
    }
}