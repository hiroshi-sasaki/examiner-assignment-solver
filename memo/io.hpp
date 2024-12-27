#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "../util/io_util.hpp"
#include "user.hpp"

namespace examiner_assignment {

std::vector<Professor> examiner_assignment_input(std::string professor_filename,
                                                 std::string student_filename) {
    std::ifstream prof_file(professor_filename, std::ios::in);
    if (!prof_file) {
        std::cerr << "cannot open file: " << professor_filename << std::endl;
    }
    std::string str_buf, str_conma_buf;
    std::getline(prof_file, str_buf);
    int professor_name_index, professor_campus_index, professor_possible_index;
    {
        auto line = get_line_split_by_c(str_buf, ',');
        professor_name_index = get_column_index(line, "氏名");
        professor_campus_index = get_column_index(line, "キャンパス");
        professor_possible_index = get_column_index(line, "予定");
    }

    std::vector<Professor> professors;

    std::map<std::string, std::string> table;

    while (std::getline(prof_file, str_buf)) {
        auto line = get_line_split_by_c(str_buf, ',');
        std::string name = line[professor_name_index];
        std::string campus = line[professor_campus_index];
        std::string is_possible = line[professor_possible_index];
        professors.emplace_back(name, campus, is_possible);
        table[name] = is_possible;
    }

    std::ifstream student_file(student_filename, std::ios::in);

    if (!student_file) {
        std::cerr << "cannot open file: " << student_filename << std::endl;
    }

    std::getline(student_file, str_buf);
    int student_number_index, student_name_index, supervisor_index,
        examiner_index;
    {
        auto line = get_line_split_by_c(str_buf, ',');
        student_number_index = get_column_index(line, "学籍番号");
        student_name_index = get_column_index(line, "学生所属名");
        supervisor_index = get_column_index(line, "指導教員");
        examiner_index = get_column_index(line, "審査教員1");
    }

    while (std::getline(student_file, str_buf)) {
        auto line = get_line_split_by_c(str_buf, ',');
        std::string name = line[student_name_index];
        std::string supervisor = line[supervisor_index];
        assert(table.contains(supervisor));
        std::string is_possible = table[supervisor];
        std::vector<std::string> assign_professors;
        for (int i = examiner_index; i < (int)line.size(); i++) {
            std::string prof = line[i];
            assign_professors.emplace_back(prof);
            auto s = table[prof];
            for (int j = 0; j < l; j++) {
                if (s[j] == 'x') is_possible = 'x';
            }
        }
        for (auto &prof : professors) {
            if (prof.name == supervisor) {
                prof.students.emplace_back(name, supervisor, assign_professors,
                                           is_possible);
            }
        }
    }
    return professors;
}

}  // namespace examiner_assignment