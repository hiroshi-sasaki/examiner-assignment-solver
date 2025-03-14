#include "io.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "slot.h"

namespace concept_presentation {

extern int k;

std::vector<Professor> professor_input(Time time_info,
                                       std::string professor_filename) {
    auto professor_base_info = professor_base_info_input();
    auto professor_label = professor_label_input();

    std::ifstream file(professor_filename, std::ios::in);
    std::string str_buf;
    std::vector<Professor> professors;
    std::getline(file, str_buf);
    int name_index;
    std::vector<int> time_window_index;
    {
        auto line = get_line_split_by_c(str_buf, ',');
        name_index = get_column_index(line, professor_label.name_labels);
        for (auto label : time_info.time_window_label) {
            time_window_index.emplace_back(get_column_index(line, {label}));
        }
    }
    while (std::getline(file, str_buf)) {
        auto line = get_line_split_by_c(str_buf, ',');
        std::string name = line[name_index];

        std::string is_possible = "";
        for (auto index : time_window_index) {
            if (line[index].ends_with("OK")) {
                is_possible += 'o';
            } else {
                is_possible += 'x';
            }
        }

        insert_or_assign(professors, {name, is_possible});
    }
    return combined_professor_info(professor_base_info, professors);
}

std::vector<Student> student_input(std::string student_filename) {
    auto student_label = student_label_input();
    std::ifstream file(student_filename, std::ios::in);
    std::string str_buf;
    std::vector<Student> students;
    std::getline(file, str_buf);
    int student_number_index, supervisor_name_index, other_index;
    {
        auto line = get_line_split_by_c(str_buf, ',');

        // ファイルの対象の行によって変更
        student_number_index =
            get_column_index(line, student_label.number_labels);
        supervisor_name_index =
            get_column_index(line, student_label.supervisor_labels);
        other_index = get_column_index(line, student_label.other_labels);
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

std::vector<Professor> concept_presentation_input(
    std::string time_filename, std::string professor_filename,
    std::string student_filename) {
    Time time_info = time_info_input(time_filename);
    k = time_info.section;
    auto professors = professor_input(time_info, professor_filename);
    auto students = student_input(student_filename);

    for (auto student : students) {
        for (auto &professor : professors) {
            if (professor.is_same_name(student.get_supervisor())) {
                student.set_supervisor(professor.get_name());
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