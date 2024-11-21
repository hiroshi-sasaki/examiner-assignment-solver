#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include "user.hpp" 

namespace research_project {

std::ostream &operator<<(std::ostream &os, const Student &student) {
    os << student.number << ", " << student.name << ", " << student.furigana << ", " << (student.is_first ? student.first_supervisor : student.second_supervisor);
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
    while(std::getline(file, str_buf)) {
        std::istringstream i_stream(str_buf);
        std::vector<std::string> line;
        while(std::getline(i_stream, str_conma_buf, ',')) {
            line.emplace_back(str_conma_buf);
        }
        students.emplace_back(line);
    }
    std::cerr << "[Input is completed]" << std::endl;
    for(int i = 0; i + 1 < students.size(); i++) {
        students[i].second_supervisor.pop_back();
    }
    return students;
}

void reserch_project_output(std::vector<std::vector<Student>> result) {
    std::cout << "枠番号, 学籍番号, 名前, フリガナ, 教員" << std::endl;
    for(auto slot: result) {
        for(int i = 1; auto student: slot) {
            std::cout << i << ", " << student << std::endl;
            i++;
        }
        std::cout << std::endl;
    }
    return;
}

}