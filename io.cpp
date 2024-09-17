#pragma once

#include <iostream>
#include <fstream>

#include "user.hpp"

std::vector<Professor> concept_presentation_input(
    std::string professor_filename, std::string student_filename) {
    std::ifstream professor_file(professor_filename, std::ios::in);
    if (!professor_file) {
        std::cerr << "cannot open file: " << professor_filename << std::endl;
    }
    int n;
    std::cin >> n;
    std::vector<Professor> professors(n);
    for(int i = 0; i < n; i++) {
        std::string timestamp, mail, name, is_possible;
        professor_file >> timestamp >> mail >> name >> is_possible;
        professors[i].name = name;
        professors[i].is_possible = is_possible;
    }

    int m;
    std::cin >> m;
    std::ifstream student_file(student_filename, std::ios::in);
    for(int i = 0; i < m; i++) {
        std::string timestamp, name, supervisor;
        student_file >> timestamp >> name >> supervisor;
        for(auto &professor: professors) {
            if(professor.name == supervisor) {
                professor.students.emplace_back(name, supervisor);
            }
        }
    }
    return professors;
}