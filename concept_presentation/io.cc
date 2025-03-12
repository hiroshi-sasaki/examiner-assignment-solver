#include "io.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "slot.h"

namespace concept_presentation {

extern int k;

std::vector<Professor> professor_input(std::string professor_base_info_filename, std::string professor_filename) {
    auto professor_base_info = professor_base_info_input(professor_base_info_filename);

    std::ifstream file(professor_filename, std::ios::in);
    std::string str_buf;
    std::vector<Professor> professors;
    std::getline(file, str_buf);
    int name_index, first_slot_index,
        second_slot_index;
    {
        auto line = get_line_split_by_c(str_buf, ',');
        name_index = get_column_index(line, "ご自身のお名前をお選びください");
        first_slot_index = get_column_index(line, " [13:30~15:00]");
        second_slot_index = get_column_index(line, " [15:15~16:45]");
    }
    while (std::getline(file, str_buf)) {
        auto line = get_line_split_by_c(str_buf, ',');
        std::string name = line[name_index];

        std::string is_possible = "";
        if (line[first_slot_index].ends_with("OK"))
            is_possible += 'o';
        else
            is_possible += 'x';
        if (line[second_slot_index].ends_with("OK"))
            is_possible += 'o';
        else
            is_possible += 'x';

        insert_or_assign(professors, {name, is_possible});
    }
    return combined_professor_info(professor_base_info, professors);
}

std::vector<Student> student_input(std::string student_filename) {
    std::ifstream file(student_filename, std::ios::in);
    std::string str_buf, str_conma_buf;
    std::vector<Student> students;
    std::getline(file, str_buf);
    int student_number_index, supervisor_name_index,
        other_index;
    {
        auto line = get_line_split_by_c(str_buf, ',');

        // ファイルの対象の行によって変更
        student_number_index =
            get_column_index(line, "学籍番号／Student ID  No.");
        supervisor_name_index =
            get_column_index(line,
                             "主指導教員を選んでください／Choose your main "
                             "academic supervisor.");
        other_index = get_column_index(line, "備考／Other Notes");
    }
    while (std::getline(file, str_buf)) {
        auto line = get_line_split_by_c(str_buf, ',');
        std::string number = line[student_number_index];
        std::string supervisor = line[supervisor_name_index];
        std::string is_possible = "oo";
        if (line[other_index] == "1")
            is_possible[1] = 'x';
        else if (line[other_index] == "2")
            is_possible[0] = 'x';
        insert_or_assign(students, {number, "", supervisor, is_possible});
    }
    return students;
}

std::vector<Professor> concept_presentation_input(std::string professor_base_info_filename,
    std::string professor_filename, std::string student_filename) {
    auto professors = professor_input(professor_base_info_filename, professor_filename);
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
        std::cout << "学籍番号";
        for (int i = 1; i < k + 1; i++) {
            std::cout << ", "
                      << "教員" << i;
        }
        std::cout << std::endl;
        for (auto s : window) {
            std::cout << s.presenter.get_number();
            for (auto a : s.assign_professors) {
                std::cout << ", ";
                std::cout << a;
            }
            std::cout << std::endl;
        }
    }
}

}  // namespace concept_presentation