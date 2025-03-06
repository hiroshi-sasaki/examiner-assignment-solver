#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "io_util.h"
#include "solver.h"

namespace bachelor_presentation {

void bachelor_presentation_solver::professor_input(
    std::string professor_base_info_filename, std::string professor_filename) {
    professors_ = professor_base_info_input(professor_base_info_filename);

    std::ifstream prof_file(professor_filename, std::ios::in);
    if (!prof_file) {
        std::cerr << "cannot open file: " << professor_filename << std::endl;
    }
    std::string str_buf, str_conma_buf;
    std::getline(prof_file, str_buf);
    int professor_name_index;
    std::vector<int> professor_possible_index;
    {
        auto line = get_line_split_by_c(str_buf, ',');
        professor_name_index =
            get_column_index(line, "ご自身のお名前をお選びください");
        for (auto label : time_info.time_window_label) {
            professor_possible_index.emplace_back(
                get_column_index(line, label));
        }
    }

    while (std::getline(prof_file, str_buf)) {
        auto line = get_line_split_by_c(str_buf, ',');
        std::string name = line[professor_name_index];
        std::string is_possible = "";
        for (auto index : professor_possible_index) {
            if (line[index].ends_with("OK")) {
                is_possible += 'o';
            } else {
                assert(line[index].ends_with("NG"));
                is_possible += 'x';
            }
        }
        for (auto &prof : professors_) {
            if (prof.get_name() == name) {
                prof.set_is_possible(is_possible);
                break;
            }
        }
    }
}

void bachelor_presentation_solver::student_input(std::string student_filename) {
    std::ifstream student_file(student_filename, std::ios::in);

    if (!student_file) {
        std::cerr << "cannot open file: " << student_filename << std::endl;
    }
    std::string str_buf;
    std::getline(student_file, str_buf);
    int student_number_index, student_name_index, supervisor_index;
    {
        auto line = get_line_split_by_c(str_buf, ',');
        student_number_index = get_column_index(line, "学籍番号");
        student_name_index = get_column_index(line, "氏名");
        supervisor_index =
            get_column_index(line, "主指導教員を選んでください。");
    }

    while (std::getline(student_file, str_buf)) {
        auto line = get_line_split_by_c(str_buf, ',');
        std::string number = line[student_number_index];
        std::string name = line[student_name_index];
        std::string supervisor = line[supervisor_index];

        auto itr =
            std::find_if(professors_.begin(), professors_.end(),
                         [&name](Professor p) { return p.get_name() == name; });
        assert(itr != professors_.end());
        itr->add_student({number, name, supervisor});
        assign_count[supervisor]++;
    }
}

void bachelor_presentation_solver::input(
    std::string time_filename, std::string professor_base_info_filename,
    std::string professor_filename, std::string student_filename) {
    time_info = time_info_input(time_filename);
    professor_input(professor_base_info_filename, professor_filename);
    student_input(student_filename);
}

}  // namespace bachelor_presentation