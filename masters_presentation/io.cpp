#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "solver.hpp"
#include "../util/io_util.hpp"

namespace examiner_assignment
{

    void examiner_assignment_solver::masters_presentation_input(std::string time_file, std::string professor_filename,
                                           std::string student_filename)
    {
        masters_presentation_init(time_file);
        const int examiner_count = 4;
        std::ifstream prof_file(professor_filename, std::ios::in);
        if (!prof_file)
        {
            std::cerr << "cannot open file: " << professor_filename << std::endl;
        }
        std::string str_buf, str_conma_buf;
        std::getline(prof_file, str_buf);
        int professor_name_index, campus_index;
        std::vector<int> professor_possible_index;
        {
            auto line = get_line_split_by_c(str_buf, ',');
            professor_name_index = get_column_index(line, "ご自身のお名前をお選びください");
            campus_index = get_column_index(line, "キャンパス");
            for (auto label : time_window_label)
            {
                professor_possible_index.emplace_back(get_column_index(line, label));
            }
        }

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
            insert_or_assign(professors_, prof);
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
            std::string student_number = line[student_number_index];
            std::string name = line[student_name_index];
            std::string main_examiner = line[main_examiner_index];
            assert(table.contains(main_examiner));

            assign_count[main_examiner]++;

            std::string is_possible = table[main_examiner];
            std::vector<std::string> assign_professors = {main_examiner};
            for (auto index : deputy_examiner_index)
            {
                if(index >= line.size()) continue;
                std::string deputy_name = line[index];
                if(deputy_name.empty()) continue;

                assign_professors.emplace_back(deputy_name);
                assign_count[deputy_name]++;

                if(!table.contains(deputy_name)) {
                    continue;
                }
                auto s = table[deputy_name];
                for (int j = 0; j < (int)s.size(); j++)
                {
                    if (s[j] == 'x')
                        is_possible[j] = 'x';
                }
            }
            for (auto &prof : professors_)
            {
                if (prof.name == main_examiner)
                {
                    prof.students.emplace_back(student_number, name, main_examiner, assign_professors,
                                               is_possible);
                }
            }
        }
    }

    void examiner_assignment_solver::bachelor_presentation_input(std::string time_filename, std::string professor_filename, std::string student_filename) {
        bachelor_presentation_init(time_filename);
        std::ifstream prof_file(professor_filename, std::ios::in);
        if (!prof_file)
        {
            std::cerr << "cannot open file: " << professor_filename << std::endl;
        }
        std::string str_buf, str_conma_buf;
        std::getline(prof_file, str_buf);
        int professor_name_index, campus_index;
        std::vector<int> professor_possible_index;
        {
            auto line = get_line_split_by_c(str_buf, ',');
            professor_name_index = get_column_index(line, "ご自身のお名前をお選びください");
            campus_index = get_column_index(line, "キャンパス");
            for (auto label : time_window_label)
            {
                professor_possible_index.emplace_back(get_column_index(line, label));
            }
        }

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
            insert_or_assign(professors_, prof);
            table[name] = is_possible;
        }

        std::ifstream student_file(student_filename, std::ios::in);

        if (!student_file)
        {
            std::cerr << "cannot open file: " << student_filename << std::endl;
        }

        std::getline(student_file, str_buf);
        int student_number_index, student_name_index, supervisor_index;
        {
            auto line = get_line_split_by_c(str_buf, ',');
            student_number_index = get_column_index(line, "学籍番号");
            student_name_index = get_column_index(line, "氏名");
            supervisor_index = get_column_index(line, "主指導教員を選んでください。");
        }

        while (std::getline(student_file, str_buf))
        {
            auto line = get_line_split_by_c(str_buf, ',');
            std::string student_number = line[student_number_index];
            std::string name = line[student_name_index];
            std::string supervisor = line[supervisor_index];
            assert(table.contains(supervisor));

            std::string is_possible = table[supervisor];

            assign_count[supervisor]++;
            for (auto &prof : professors_)
            {
                if (prof.name == supervisor)
                {
                    Student student;
                    student.name = name;
                    student.number = student_number;
                    student.supervisor = supervisor;
                    student.is_possible = is_possible;
                    int ret = prof.students.size();
                    insert_or_assign(prof.students, student);
                }
            }
        }
    }

    void examiner_assignment_solver::intermediate_examination_input(std::string filename) {
        std::ifstream file(filename, std::ios::in);
        if (!file)
        {
            std::cerr << "cannot open file: " << filename << std::endl;
        }
        std::string str_buf, str_conma_buf;
        std::getline(file, str_buf);

        int student_number_index, student_name_index, supervisor_index;
        {
            auto line = get_line_split_by_c(str_buf, ',');
            student_number_index = get_column_index(line, "学籍番号");
            student_name_index = get_column_index(line, "氏名");
            supervisor_index = get_column_index(line, "主指導教員を選んでください。");
        }

        while (std::getline(file, str_buf))
        {
            auto line = get_line_split_by_c(str_buf, ',');
            std::string student_number = line[student_number_index];
            std::string student_name = line[student_name_index];
            std::string supervisor = line[supervisor_index];
            Student student;
            student.name = student_name;
            student.number = student_number;
            student.supervisor = supervisor;
            student.assign_professors = {supervisor};
            intermediate_examiner_students.emplace_back(student);
            assign_count[supervisor]++;
        }
    }

} // namespace examiner_assignment