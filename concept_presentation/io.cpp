#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include "user.hpp"

namespace concept_presentation {

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &vec) {
    for (std::size_t i = 0; i < vec.size(); i++)
        os << vec[i] << (i + 1 == vec.size() ? "" : " ");
    return os;
}

/*
タイムスタンプ, 学生記番号, 主指導教員,可能か,備考
*/
std::vector<Student> student_input(std::string student_filename) {
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
        students.emplace_back();
        auto &student = students.back();
        student.name = line[1];
        if(line[4] == "1") student.is_possible[1] = 'x';
        else if(line[4] == "2") student.is_possible[0] = 'x';
        student.supervisor = line[2];
    }
    return students;
}

/*
名前,役職,所属研究室,前半,後半,自由記述覧
*/
std::vector<Professor> professor_input(std::string professor_filename) {
    std::ifstream file(professor_filename, std::ios::in);
    std::string str_buf, str_conma_buf;
    std::vector<Professor> professors;
    std::getline(file, str_buf);
    while(std::getline(file, str_buf)) {
        std::istringstream i_stream(str_buf);
        std::vector<std::string> line;
        while(std::getline(i_stream, str_conma_buf, ',')) {
            line.emplace_back(str_conma_buf);
        }
        professors.emplace_back();
        auto &prof = professors.back();
        prof.name = line[0];
        if(line[3].ends_with("OK")) prof.is_possible += 'o';
        else prof.is_possible += 'x';
        if(line[4].ends_with("OK")) prof.is_possible += 'o';
        else prof.is_possible += 'x';

        prof.affiliation = line[2];

        if(line[1] == "教授") {
            prof.type = ProfessorType::professor;
        }
        else if(line[1] == "准教授") {
            prof.type = ProfessorType::associate;
        }
        else if(line[1] == "助教") {
            prof.type = ProfessorType::assistant;
        }
        else {
            std::cerr << line[1] << std::endl;
            assert(0);
        }
    }
    return professors;
}

std::vector<Professor>concept_presentation_input(
    std::string professor_filename, std::string student_filename) {
    auto professors = professor_input(professor_filename);
    auto students = student_input(student_filename);

    for(auto student: students) {
        for(auto &professor: professors) {
            if(professor.name == student.supervisor) {
                professor.students.emplace_back(student);
            }
        }
    }
    return professors;
}

void concept_presentation_output(std::vector<std::vector<Slot>> result) {
    for(auto window: result) {
        std::cout << "学生";
        for(int i = 1; i < k+1; i++) {
            std::cout << " " << "教員" << i;
        }
        std::cout << std::endl;
        for(auto s: window) {
            std::cout << s.presenter;
            for(auto a: s.assign_professor) {
                std::cout << ", ";
                std::cout << a;
            }
            std::cout << std::endl;
        }
    }
}

}