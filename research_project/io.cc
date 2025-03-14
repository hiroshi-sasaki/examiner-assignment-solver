#include "io.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "io.h"
#include "io_util.h"
#include "student.h"

namespace research_project {

std::pair<std::vector<Student>, std::vector<Student>> research_project_input(
    std::string student_filename) {
    auto student_label = student_label_input();
    std::ifstream file(student_filename, std::ios::in);
    std::string str_buf;
    std::vector<Student> first_section, second_section;
    std::getline(file, str_buf);
    int student_number_index, student_name_index, first_index, second_index;
    {
        auto line = get_line_split_by_c(str_buf, ',');
        student_number_index =
            get_column_index(line, student_label.number_labels);
        student_name_index = get_column_index(line, student_label.name_labels);
        first_index =
            get_column_index(line, student_label.first_professor_labels);
        second_index =
            get_column_index(line, student_label.second_professor_labels);
    }
    while (std::getline(file, str_buf)) {
        auto line = get_line_split_by_c(str_buf, ',');

        std::string number = line[student_number_index];
        std::string name = line[student_name_index];
        std::string first = line[first_index];
        std::string second = line[second_index];
        insert_or_assign(first_section, Student{number, name, first});
        insert_or_assign(second_section, Student{number, name, second});
    }
    return {first_section, second_section};
}

void research_project_output(const std::vector<std::vector<Student>> &result) {
    std::cout << "枠番号,学籍番号,名前,教員" << std::endl;
    for (auto slot : result) {
        for (int i = 1; auto student : slot) {
            std::cout << i << ", " << student.get_number() << " "
                      << student.get_name() << " " << student.get_supervisor()
                      << std::endl;
            i++;
        }
        std::cout << std::endl;
    }
    return;
}

}  // namespace research_project