#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "io_util.h"
#include "solver.h"

namespace master_presentation {

void master_presentation_solver::professor_input(
    std::string professor_base_info_filename, std::string professor_filename) {
    auto professor_base_info =
        professor_base_info_input(professor_base_info_filename);

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
            get_column_index(line, "ご自身のお名前をお選びください");
        for (auto label : time_info.time_window_label) {
            professor_possible_index.emplace_back(
                get_column_index(line, label));
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

void master_presentation_solver::student_input(std::string student_filename) {
    std::ifstream student_file(student_filename, std::ios::in);

    if (!student_file) {
        std::cerr << "cannot open file: " << student_filename << std::endl;
    }

    std::string str_buf;
    std::getline(student_file, str_buf);
    int student_number_index, student_name_index, supervisor_index,
        main_examiner_index;
    std::vector<int> deputy_examiner_index;
    {
        auto line = get_line_split_by_c(str_buf, ',');
        student_number_index = get_column_index(line, "学籍番号");
        student_name_index = get_column_index(line, "学生氏名");
        supervisor_index = get_column_index(line, "指導教員");
        main_examiner_index = get_column_index(line, "主審査");
        for (int i = 1; i <= examiner_count; i++) {
            deputy_examiner_index.emplace_back(
                get_column_index(line, "副審査" + std::to_string(i)));
        }
    }

    while (std::getline(student_file, str_buf)) {
        auto line = get_line_split_by_c(str_buf, ',');
        std::string student_number = line[student_number_index];
        std::string name = line[student_name_index];
        std::string main_examiner = line[main_examiner_index];

        assign_count[main_examiner]++;

        auto itr = std::find_if(professors_.begin(), professors_.end(),
                                [&main_examiner](Professor p) {
                                    return p.get_name() == main_examiner;
                                });
        assert(itr != professors_.end());
        std::string is_possible = itr->get_is_possible();
        std::vector<std::string> assign_professors = {main_examiner};
        for (auto index : deputy_examiner_index) {
            if (index >= line.size()) continue;
            std::string deputy_name = line[index];
            if (deputy_name.empty()) continue;

            assign_professors.emplace_back(deputy_name);
            assign_count[deputy_name]++;

            auto deputy_itr =
                std::find_if(professors_.begin(), professors_.end(),
                             [&deputy_name](Professor p) {
                                 return p.get_name() == deputy_name;
                             });
            if (deputy_itr == professors_.end()) {
                continue;
            }
            assert(deputy_itr != professors_.end());
            if (deputy_itr == professors_.end()) continue;
            auto s = deputy_itr->get_is_possible();
            for (int j = 0; j < (int)s.size(); j++) {
                if (s[j] == 'x') is_possible[j] = 'x';
            }
        }
        itr->add_student({student_number, name, main_examiner, is_possible,
                          assign_professors});
    }
}

void master_presentation_solver::input(std::string time_filename,
                                       std::string professor_base_info_filename,
                                       std::string professor_filename,
                                       std::string student_filename) {
    time_info = time_info_input(time_filename);
    professor_input(professor_base_info_filename, professor_filename);
    student_input(student_filename);
}

}  // namespace master_presentation