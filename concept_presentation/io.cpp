#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "../util/io_util.hpp"
#include "user.hpp"

namespace concept_presentation {

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &vec) {
    for (std::size_t i = 0; i < vec.size(); i++)
        os << vec[i] << (i + 1 == vec.size() ? "" : " ");
    return os;
}

std::vector<Student> student_input(std::string student_filename) {
    std::ifstream file(student_filename, std::ios::in);
    std::string str_buf, str_conma_buf;
    std::vector<Student> students;
    std::getline(file, str_buf);
    int student_number_index, student_name_index, supervisor_name_index,
        t_or_f_index, other_index;
    {
        auto line = get_line_split_by_c(str_buf, ',');

        // ファイルの対象の行によって変更
        student_number_index =
            get_column_index(line, "学籍番号／Student ID  No.");
        student_name_index = get_column_index(line, "氏名／Name");
        supervisor_name_index =
            get_column_index(line,
                             "主指導教員を選んでください／Choose your main "
                             "academic supervisor.");
        t_or_f_index =
            get_column_index(line, "12/11に変更後も参加できるかどうか");
        other_index = get_column_index(line, "備考／Other Notes");
    }
    while (std::getline(file, str_buf)) {
        auto line = get_line_split_by_c(str_buf, ',');
        if (line[t_or_f_index].ends_with("NG")) {
            continue;
        }
        Student student;
        student.number = line[student_number_index];
        student.name = line[student_name_index];
        if (line[other_index] == "1")
            student.is_possible[1] = 'x';
        else if (line[other_index] == "2")
            student.is_possible[0] = 'x';
        student.supervisor = line[supervisor_name_index];
        insert_or_assign(students, student);
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
    int name_index, work_position_index, affiliation_index, first_slot_index,
        second_slot_index;
    {
        std::istringstream i_stream(str_buf);
        std::vector<std::string> line;
        while (std::getline(i_stream, str_conma_buf, ',')) {
            line.emplace_back(str_conma_buf);
        }
        name_index = get_column_index(line, "ご自身のお名前をお選びください");
        work_position_index = get_column_index(line, "役職");
        affiliation_index = get_column_index(line, "所属研究室");
        first_slot_index = get_column_index(line, "[13:30~15:00]");
        second_slot_index = get_column_index(line, "[15:15~16:45]");
    }
    while (std::getline(file, str_buf)) {
        std::istringstream i_stream(str_buf);
        std::vector<std::string> line;
        while (std::getline(i_stream, str_conma_buf, ',')) {
            line.emplace_back(str_conma_buf);
        }
        Professor prof;
        prof.name = line[name_index];
        if (line[first_slot_index].ends_with("OK"))
            prof.is_possible += 'o';
        else
            prof.is_possible += 'x';
        if (line[second_slot_index].ends_with("OK"))
            prof.is_possible += 'o';
        else
            prof.is_possible += 'x';

        prof.affiliation = line[affiliation_index];

        if (line[work_position_index] == "教授") {
            prof.type = ProfessorType::professor;
        } else if (line[work_position_index] == "准教授") {
            prof.type = ProfessorType::associate;
        } else if (line[work_position_index] == "助教") {
            prof.type = ProfessorType::assistant;
        } else {
            std::cerr << line[work_position_index] << std::endl;
            assert(0);
        }
        insert_or_assign(professors, prof);
    }
    return professors;
}

std::vector<Professor> concept_presentation_input(
    std::string professor_filename, std::string student_filename) {
    auto professors = professor_input(professor_filename);
    auto students = student_input(student_filename);

    for (auto student : students) {
        for (auto &professor : professors) {
            if (professor.name == student.supervisor) {
                professor.students.emplace_back(student);
            }
        }
    }
    return professors;
}

void concept_presentation_output(std::vector<std::vector<Slot>> result) {
    for (auto window : result) {
        std::cout << "学籍番号, 氏名";
        for (int i = 1; i < k + 1; i++) {
            std::cout << ", "
                      << "教員" << i;
        }
        std::cout << std::endl;
        for (auto s : window) {
            std::cout << s.presenter.number << ", " << s.presenter.name;
            for (auto a : s.assign_professor) {
                std::cout << ", ";
                std::cout << a;
            }
            std::cout << std::endl;
        }
    }
}

}  // namespace concept_presentation