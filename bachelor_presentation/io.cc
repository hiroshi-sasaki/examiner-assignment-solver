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
    std::string professor_filename) {
    auto professor_base_info = professor_base_info_input();
    auto professor_label = professor_label_input();

    std::ifstream prof_file(professor_filename, std::ios::in);
    if (!prof_file) {
        std::cerr << "cannot open file: " << professor_filename << std::endl;
    }
    std::string str_buf;
    std::getline(prof_file, str_buf);
    int professor_name_index;
    std::vector<int> professor_possible_index;
    {
        auto line = get_line_split_by_c(str_buf, ',');
        professor_name_index =
            get_column_index(line, professor_label.name_labels);
        for (auto label : time_info.time_window_label) {
            professor_possible_index.emplace_back(
                get_column_index(line, {label}));
        }
    }

    std::vector<Professor> professors;

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
        professors.emplace_back(name, is_possible);
    }
    professors_ = combined_professor_info(professor_base_info, professors);
}

void bachelor_presentation_solver::student_input(std::string student_filename) {
    auto student_label = student_label_input();

    std::ifstream student_file(student_filename, std::ios::in);

    if (!student_file) {
        std::cerr << "cannot open file: " << student_filename << std::endl;
    }
    std::string str_buf;
    std::getline(student_file, str_buf);
    int student_number_index, student_name_index, supervisor_index;
    {
        auto line = get_line_split_by_c(str_buf, ',');
        student_number_index =
            get_column_index(line, student_label.number_labels);
        student_name_index = get_column_index(line, student_label.name_labels);
        supervisor_index =
            get_column_index(line, student_label.supervisor_labels);
    }

    while (std::getline(student_file, str_buf)) {
        auto line = get_line_split_by_c(str_buf, ',');
        std::string number = line[student_number_index];
        std::string name = line[student_name_index];
        std::string supervisor = line[supervisor_index];

        auto itr = std::find_if(
            professors_.begin(), professors_.end(),
            [&supervisor](Professor p) { return p.is_same_name(supervisor); });
        assert(itr != professors_.end());
        supervisor = itr->get_name();
        itr->add_student({number, name, supervisor, itr->get_is_possible()});
        assign_count[supervisor]++;
    }
}

void bachelor_presentation_solver::input(std::string time_filename,
                                         std::string professor_filename,
                                         std::string student_filename) {
    time_info = time_info_input(time_filename);
    professor_input(professor_filename);
    student_input(student_filename);
}

}  // namespace bachelor_presentation