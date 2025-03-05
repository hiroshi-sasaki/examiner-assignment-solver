#include "io.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "slot.h"

namespace concept_presentation {

extern int k;

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
        std::string name = line[name_index];

        ProfessorType type;
        if (line[work_position_index] == "教授") {
            type = ProfessorType::professor;
        } else if (line[work_position_index] == "准教授") {
            type = ProfessorType::associate;
        } else if (line[work_position_index] == "助教") {
            type = ProfessorType::assistant;
        } else {
            std::cerr << line[work_position_index] << std::endl;
            assert(0);
        }

        std::string is_possible = "";
        if (line[first_slot_index].ends_with("OK"))
            is_possible += 'o';
        else
            is_possible += 'x';
        if (line[second_slot_index].ends_with("OK"))
            is_possible += 'o';
        else
            is_possible += 'x';

        std::string affiliation = line[affiliation_index];

        insert_or_assign(professors, {name, type, is_possible, affiliation});
    }
    return professors;
}

std::vector<Student> student_input(std::string student_filename) {
    std::ifstream file(student_filename, std::ios::in);
    std::string str_buf, str_conma_buf;
    std::vector<Student> students;
    std::getline(file, str_buf);
    int student_number_index, student_name_index, supervisor_name_index,
        other_index;
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
        other_index = get_column_index(line, "備考／Other Notes");
    }
    while (std::getline(file, str_buf)) {
        auto line = get_line_split_by_c(str_buf, ',');
        std::string number = line[student_number_index];
        std::string name = line[student_name_index];
        std::string supervisor = line[supervisor_name_index];
        std::string is_possible = "oo";
        if (line[other_index] == "1")
            is_possible[1] = 'x';
        else if (line[other_index] == "2")
            is_possible[0] = 'x';
        insert_or_assign(students, {number, name, supervisor, is_possible});
    }
    return students;
}

std::vector<Professor> concept_presentation_input(
    std::string professor_filename, std::string student_filename) {
    auto professors = professor_input(professor_filename);
    auto students = student_input(student_filename);

    for (auto student : students) {
        for (auto &professor : professors) {
            if (professor.get_name() == student.get_supervisor()) {
                professor.add_student(student);
            }
        }
    }
    return professors;
}

void concept_presentation_output(std::vector<std::vector<Slot>> schedule) {
    for (auto window : schedule) {
        std::cout << "学籍番号, 氏名";
        for (int i = 1; i < k + 1; i++) {
            std::cout << ", "
                      << "教員" << i;
        }
        std::cout << std::endl;
        for (auto s : window) {
            std::cout << s.presenter.get_number() << ", "
                      << s.presenter.get_name();
            for (auto a : s.assign_professors) {
                std::cout << ", ";
                std::cout << a;
            }
            std::cout << std::endl;
        }
    }
}

}  // namespace concept_presentation