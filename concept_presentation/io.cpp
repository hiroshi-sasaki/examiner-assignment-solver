#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include "user.hpp"

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &vec) {
    for (std::size_t i = 0; i < vec.size(); i++)
        os << vec[i] << (i + 1 == vec.size() ? "" : " ");
    return os;
}

/*
タイムスタンプ, 氏名, 主指導教員
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
        if('A' <= line[2].back() && line[2].back() <= 'Z') {
            while('A' <= line[2].back() && line[2].back() <= 'Z') {
                line[2].pop_back();
            }
        }
        student.supervisor = line[2];
    }
    return students;
}

/*
タイムスタンプ, メールアドレス, 名前, 前半, 後半
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
        prof.name = line[2];
        if(line[3].ends_with("OK")) prof.is_possible += 'o';
        else prof.is_possible += 'x';
        if(line[4].ends_with("OK")) prof.is_possible += 'o';
        else prof.is_possible += 'x';
    }
    return professors;
}

std::vector<Professor> concept_presentation_input(
    std::string professor_filename, std::string student_filename) {
    auto professors = professor_input(professor_filename);
    auto students = student_input(student_filename);

    for(auto student: students) {
        for(auto &professor: professors) {
            if(professor.name == student.supervisor) {
                professor.students.emplace_back(student.name, student.supervisor);
            }
        }
    }

    for(auto &prof: professors) {
        if(prof.students.empty()) {
            prof.type = ProfessorType::assistant;
        }
        else {
            prof.type = ProfessorType::professor;
        }
    }

    return professors;
}

void concept_presentation_output(std::vector<std::vector<Slot>> result) {
    for(auto window: result) {
        std::cout << "学生, 教員1, 教員2, 教員3, 教員4, 教員5" << std::endl;
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