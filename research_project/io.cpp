#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "../util/io_util.hpp"
#include "user.hpp"

namespace research_project {

std::ostream &operator<<(std::ostream &os, const Student &student) {
    os << student.number << ", " << student.name << ", "
       << (student.is_first ? student.first_supervisor
                            : student.second_supervisor);
    return os;
}

/*
学籍番号, 名前, フリガナ, 前半教員, 後半教員
*/
std::vector<Student> research_project_input(std::string student_filename) {
    std::ifstream file(student_filename, std::ios::in);
    std::string str_buf, str_conma_buf;
    std::vector<Student> students;
    std::getline(file, str_buf);
    int student_number_index, student_name_index, first_index, second_index;
    {
        auto line = get_line_split_by_c(str_buf, ',');
        student_number_index = get_column_index(line, "学籍番号");
        student_name_index = get_column_index(line, "学生所属名");
        first_index = get_column_index(line, "前半教員");
        second_index = get_column_index(line, "後半教員");
    }
    while (std::getline(file, str_buf)) {
        auto line = get_line_split_by_c(str_buf, ',');

        std::string number = line[student_number_index];
        std::string name = line[student_name_index];
        std::string first = line[first_index];
        std::string second = line[second_index];
        students.emplace_back(number, name, first, second);
    }
    std::cerr << "[Input is completed]" << std::endl;
    return students;
}

void research_project_output(std::vector<std::vector<Student>> result) {
    std::cout << "枠番号, 学籍番号, 名前, フリガナ, 教員" << std::endl;
    for (auto slot : result) {
        for (int i = 1; auto student : slot) {
            std::cout << i << ", " << student << std::endl;
            i++;
        }
        std::cout << std::endl;
    }
    return;
}

}  // namespace research_project