#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "../util/io_util.hpp"
#include "user.hpp"

namespace memo
{

    std::vector<Professor> examiner_assignment_input(std::string professor_filename,
                                                     std::string student_filename)
    {
        const int examiner_count = 4;
        std::ifstream prof_file(professor_filename, std::ios::in);
        if (!prof_file)
        {
            std::cerr << "cannot open file: " << professor_filename << std::endl;
        }
        std::string str_buf, str_conma_buf;
        std::getline(prof_file, str_buf);
        std::vector<std::string> time_window;
        int professor_name_index, campus_index;
        std::vector<int> professor_possible_index;
        {
            auto line = get_line_split_by_c(str_buf, ',');
            professor_name_index = get_column_index(line, "ご自身のお名前をお選びください");
            campus_index = get_column_index(line, "キャンパス");
            for (auto time : time_window)
            {
                professor_possible_index.emplace_back(get_column_index(line, time));
            }
        }

        std::vector<Professor> professors;

        std::map<std::string, std::string> table;

        while (std::getline(prof_file, str_buf))
        {
            auto line = get_line_split_by_c(str_buf, ',');
            std::string name = line[professor_name_index];
            std::string campus = line[campus_index];
            std::string is_possible = "";
            for (auto index : professor_possible_index)
            {
                if (line[index].ends_with("OK"))
                {
                    is_possible += 'o';
                }
                else
                {
                    assert(line[index].ends_with("NG"));
                    is_possible += 'x';
                }
            }
            Professor prof;
            prof.name = name;
            prof.is_possible = is_possible;
            prof.campus = campus;
            professors.emplace_back(prof);
            table[name] = is_possible;
        }

        std::ifstream student_file(student_filename, std::ios::in);

        if (!student_file)
        {
            std::cerr << "cannot open file: " << student_filename << std::endl;
        }

        std::getline(student_file, str_buf);
        int student_number_index, student_name_index, supervisor_index, main_examiner_index;
        std::vector<int> deputy_examiner_index;
        {
            auto line = get_line_split_by_c(str_buf, ',');
            student_number_index = get_column_index(line, "学籍番号");
            student_name_index = get_column_index(line, "学生氏名");
            supervisor_index = get_column_index(line, "指導教員");
            main_examiner_index = get_column_index(line, "主審査");
            for (int i = 1; i <= examiner_count; i++)
            {
                deputy_examiner_index.emplace_back(get_column_index(line, "副審査" + std::to_string(i)));
            }
        }

        while (std::getline(student_file, str_buf))
        {
            auto line = get_line_split_by_c(str_buf, ',');
            std::string name = line[student_name_index];
            std::string supervisor = line[supervisor_index];
            std::string main_examiner = line[main_examiner_index];
            assert(table.contains(main_examiner));
            std::string is_possible = table[main_examiner];
            std::vector<std::string> assign_professors;
            for (auto index : deputy_examiner_index)
            {
                std::string name = line[index];
                assign_professors.emplace_back(name);
                auto s = table[name];
                for (int j = 0; j < l; j++)
                {
                    if (s[j] == 'x')
                        is_possible = 'x';
                }
            }
            for (auto &prof : professors)
            {
                if (prof.name == supervisor)
                {
                    prof.students.emplace_back(name, supervisor, assign_professors,
                                               is_possible);
                }
            }
        }
        return professors;
    }

} // namespace examiner_assignment