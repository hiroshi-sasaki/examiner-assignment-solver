#include "io.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "io.h"
#include "io_util.h"
#include "student.h"

namespace research_project {

std::ostream &operator<<(std::ostream &os, const Student &student) {
    os << student.get_number() << ", " << student.get_name() << ", "
       << student.get_supervisor();
    return os;
}

std::pair<std::vector<Student>, std::vector<Student>> research_project_input(
    std::string student_filename) {
    std::ifstream file(student_filename, std::ios::in);
    std::string str_buf, str_conma_buf;
    std::vector<Student> first_section, second_section;
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
        insert_or_assign(first_section, Student{number, name, first});
        insert_or_assign(second_section, Student{number, name, second});
    }
    return {first_section, second_section};
}

void research_project_output(const std::vector<std::vector<Student>> &result) {
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