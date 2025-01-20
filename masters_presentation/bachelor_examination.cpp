#include "solver.hpp"
#include "../util/io_util.hpp"

#include <fstream>
#include <iostream>
#include <algorithm>

namespace examiner_assignment {

    void examiner_assignment_solver::bachelor_presentation_init(std::string time_filename)
    {
        std::ifstream time_file(time_filename, std::ios::in);
        if(!time_file) {
            std::cerr << "cannot open file: " << time_filename << std::endl;
        }
        std::string str_buf;
        {
            std::getline(time_file, str_buf);
            auto line = get_line_split_by_c(str_buf, ' ');
            day = std::stoi(line[0]);
            section = std::stoi(line[1]);
        }
        {
            std::getline(time_file, str_buf);
            auto line = get_line_split_by_c(str_buf, ' ');
            for(auto x: line) {
                time.emplace_back(std::stoi(x));
            }
            assert(time.size() == day * section);
        }
        accumulate = time;
        accumulate.insert(accumulate.begin(), 0);
        for (int i = 0; i < section; i++)
        {
            day1_count += time[i];
            day2_count += time[section + i];
        }
        for (int i = 0; i < day; i++)
        {
            for (int j = 0; j < section; j++)
            {
                accumulate[i * section + j + 1] = accumulate[i * section + j] + time[i * section + j];
            }
        }
        {
            std::getline(time_file, str_buf);
            time_window_label = get_line_split_by_c(str_buf, ',');
        }
    }

    void examiner_assignment_solver::output_bachelor_presentation_assign(std::vector<Slot> &suzukake, std::vector<Slot> &oookayama) const {
        {
            std::cout << "すずかけ" << std::endl;
            std::cout << "時間,学籍番号,主審査員,副審査1,副審査2" << std::endl;
            int h = 9, m = 0;
            int index = 0;
            int cont = 0;
            for(auto &slot: suzukake) {
                if(h == 12 && m == 15) {
                    std::cout << "昼休憩,12:15~13:00" << std::endl;
                    h = 13;
                    m = 0;
                    cont = 0;
                }
                slot.m = h * 60 + m;
                output_time(h, m);
                std::cout << ',' << slot.student_number;
                for(auto prof: slot.assign_professor) {
                    std::cout << ',' << prof;
                }
                std::cout << std::endl;
                cont++;
                m += 15;
                while(m >= 60) {
                    m -= 60;
                    h++;
                }
                if(cont == 6 && h != 12) {
                    output_time(h, m);
                    m += 15;
                    std::cout << ",休憩" << std::endl;
                    cont = 0;
                }
                while(m >= 60) {
                    m -= 60;
                    h++;
                }
            }
        }
        {
            std::cout << "大岡山" << std::endl;
            std::cout << "時間,学籍番号,主審査員,副審査1,副審査2" << std::endl;
            int h = 9, m = 0;
            int index = 0;
            int cont = 0;
            for(auto &slot: oookayama) {
                if(h == 12 && m == 0) {
                    std::cout << "昼休憩,12:00~13:00" << std::endl;
                    h++;
                }
                slot.m = h * 60 + m;
                output_time(h, m);
                if(slot.presenter == "rest") {
                    std::cout << "空き" << std::endl;
                }
                else {
                    std::cout << ',' << slot.student_number;
                    for(auto prof: slot.assign_professor) {
                        std::cout << ',' << prof;
                    }
                    std::cout << std::endl;
                    cont++;
                }
                m += 15;
                while(m >= 60) {
                    m -= 60;
                    h++;
                }
                if(cont == 5) {
                    output_time(h, m);
                    m += 15;
                    std::cout << ",休憩" << std::endl;
                    cont = 0;
                }
                while(m >= 60) {
                    m -= 60;
                    h++;
                }
            }
        }
    }

    void examiner_assignment_solver::bachelor_presentation_run() {
        auto professors = professors_;
        std::vector<Professor> oookayama, suzukake;
        for(auto prof: professors) {
            if(prof.students.empty()) continue;
            if(prof.campus == "O") {
                oookayama.emplace_back(prof);
            }
            else {
                assert(prof.campus == "S");
                suzukake.emplace_back(prof);
            }
        }
        auto oookayama_sol = bit_dp_solution(day1_count, day1_count + day2_count, oookayama);
        auto oookayama_schedule = construct_schedule(day1_count, day1_count + day2_count, oookayama_sol);
        auto suzukake_sol = bit_dp_solution(0, day1_count, suzukake);
        auto suzukake_schedule = construct_schedule(0, day1_count, suzukake_sol);
        bachelor_presentation_assign(suzukake_schedule, oookayama_schedule);
        output_bachelor_presentation_assign(suzukake_schedule, oookayama_schedule);
    }

    void examiner_assignment_solver::bachelor_presentation_assign(std::vector<Slot> &suzukake_schedule, std::vector<Slot> &oookayama_schedule) {
        auto professors = professors_;
        std::vector<Professor> oookayama_professors, suzukake_professors;
        for(auto p: professors) {
            if(p.campus == "O") {
                oookayama_professors.emplace_back(p);
            }
            else {
                suzukake_professors.emplace_back(p);
            }
        }
        for(auto &slot: suzukake_schedule) {
            assign_count[slot.supervisor]++;
            slot.assign_professor.emplace_back(slot.supervisor);
        }
        for(auto &slot: oookayama_schedule) {
            assign_count[slot.supervisor]++;
            slot.assign_professor.emplace_back(slot.supervisor);
        }
        auto can_assign = [&](int i, std::string name) -> bool {
            auto slot = i < day1_count ? suzukake_schedule[i] : oookayama_schedule[i - day1_count];
            if(slot.assign_professor.size() == k || slot.presenter == "rest") return false;
            for(auto professor: slot.assign_professor) {
                if(professor == name) return false;
            }
            for(auto prof: professors) {
                if(prof.name == name) {
                    int index = std::lower_bound(std::begin(accumulate), std::end(accumulate), i) - accumulate.begin();
                    return prof.is_possible[index] == 'o';
                }
            }
            return false;
        };
        auto validator = [&]() -> bool {
            for(auto slot: suzukake_schedule) {
                if(slot.presenter == "rest") continue;
                if(slot.assign_professor.size() < k) return false;
            }
            for(auto slot: oookayama_schedule) {
                if(slot.presenter == "rest") continue;
                if(slot.assign_professor.size() < k) return false;
            }
            return true;
        };
        int c = 0;
        while(!validator()) {
            std::sort(oookayama_professors.begin(), oookayama_professors.end(), [&](auto lhs, auto rhs) -> bool {
                return assign_count[lhs.name] < assign_count[rhs.name];
            });
            std::sort(suzukake_professors.begin(), suzukake_professors.end(), [&](auto lhs, auto rhs) -> bool {
                return assign_count[lhs.name] < assign_count[rhs.name];
            });
            for(auto prof: oookayama_professors) {
                int cont = 0;
                for(int i = 0; i < day2_count; i++) {
                    if(can_assign(i + day1_count, prof.name)) {
                        oookayama_schedule[i].assign_professor.emplace_back(prof.name);
                        cont++;
                        assign_count[prof.name]++;
                    }
                    if(cont >= 3) break;
                }
            }
            for(auto prof: suzukake_professors) {
                int cont = 0;
                for(int i = 0; i < day1_count; i++) {
                    if(can_assign(i, prof.name)) {
                        cont++;
                        suzukake_schedule[i].assign_professor.emplace_back(prof.name);
                        assign_count[prof.name]++;
                    }
                    if(cont >= 3) break;
                }
            }
        }
        return;
    }

}