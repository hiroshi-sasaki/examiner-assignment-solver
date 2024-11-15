#pragma once

#include <algorithm>
#include <limits>
#include <map>

#include "io.cpp"
#include "user.hpp"

const int window = 2;
const int ty_assistant = 2;
const int type_count = 3;
const int max_assign = 6;
const int parameter_1 = 3;

bool validator(Slot s, std::map<std::string, int> &map) {
    if (ty_assistant < s.assistant_count || !s.flag) return false;
    assert((int)s.assign_professor.size() == k);
    int c = 0;
    for (int i = 0; i < k; i++) {
        c += map[s.assign_professor[i]] == 2;
        for (int j = i + 1; j < k; j++) {
            if (s.assign_professor[i] == s.assign_professor[j]) return false;
        }
    }
    assert(c == s.assistant_count);
    return s.flag;
}

std::vector<std::vector<Slot>> concept_presentation_assignment_solver(
    std::vector<Professor> professors) {
    std::vector plan(window, std::vector<Slot>());
    std::vector assignments(
        window + 1,
        std::vector(type_count,
                    std::vector<std::pair<std::string, std::string>>()));
    std::vector<std::map<std::string, int>> assignment_count(window);
    std::map<std::string, int> map;

    int sum_student = 0;
    for (auto &professor : professors) {
        map[professor.name] = static_cast<int>(professor.type);
        int student_count = (int)professor.students.size();
        sum_student += student_count;
        if (professor.is_possible == "oo")
            assignments[2][static_cast<unsigned>(professor.type)].emplace_back(
                professor.name, professor.affiliation);
        else {
            for (int i = 0; i < 2; i++) {
                if (professor.is_possible[i] == 'o') {
                    assignments[i][static_cast<unsigned>(professor.type)]
                        .emplace_back(professor.name, professor.affiliation);
                }
            }
        }

        std::sort(std::begin(professor.students), std::end(professor.students),
                  [](auto lhs, auto rhs) {
                      int l = int(lhs.is_possible[0] == 'o') +
                              int(lhs.is_possible[1] == 'o');
                      int r = int(rhs.is_possible[0] == 'o') +
                              int(rhs.is_possible[1] == 'o');
                      return l < r;
                  });

        if (student_count > max_assign) {
            professor.student_assign = true;

            int first = 0;
            int half = (student_count + 1) / 2;
            for (auto student : professor.students) {
                if (student.is_possible[1] == 'x' || first < half) {
                    assert(student.is_possible[0] == 'o');
                    plan[0].emplace_back(student.name, professor,
                                         professor.is_possible[0] == 'o');
                    assignment_count[0][professor.name]++;
                    first++;
                } else {
                    assert(student.is_possible[1] == 'o');
                    plan[1].emplace_back(student.name, professor,
                                         professor.is_possible[1] == 'o');
                    assignment_count[1][professor.name]++;
                }
            }
            continue;
        }

        {
            bool flag = true;
            for (auto student : professor.students) {
                flag &= student.is_possible[0] == student.is_possible[1];
            }
            if (flag || professor.is_possible[0] != professor.is_possible[1])
                continue;
            professor.student_assign = true;
            int index = -1;
            for (auto student : professor.students) {
                if (student.is_possible[1] == 'x') {
                    index = 0;
                    assert(student.is_possible[0] == 'o');
                    plan[0].emplace_back(student.name, professor,
                                         professor.is_possible[0] == 'o');
                    assignment_count[0][professor.name]++;
                } else if (student.is_possible[0] == 'x') {
                    index = 1;
                    assert(student.is_possible[1] == 'o');
                    plan[1].emplace_back(student.name, professor,
                                         professor.is_possible[1] == 'o');
                    assignment_count[1][professor.name]++;
                } else {
                    if (professor.is_possible[0] != professor.is_possible[1]) {
                        if (professor.is_possible[0] == 'o') {
                            plan[0].emplace_back(
                                student.name, professor,
                                professor.is_possible[0] == 'o');
                            assignment_count[0][professor.name]++;
                        } else {
                            plan[1].emplace_back(
                                student.name, professor,
                                professor.is_possible[1] == 'o');
                            assignment_count[1][professor.name]++;
                        }
                    } else {
                        plan[index].emplace_back(
                            student.name, professor,
                            professor.is_possible[index] == 'o');
                        assignment_count[index][professor.name]++;
                    }
                }
            }
        }
    }
    for (auto &professor : professors) {
        int student_count = (int)professor.students.size();
        if (professor.student_assign || student_count == 0) continue;
        int index = 0;
        if ((int)plan[0].size() + student_count > (sum_student + 1) / 2) {
            index = 1;
        }

        for (auto student : professor.students) {
            if (student.is_possible[0] != student.is_possible[1]) {
                if (student.is_possible[0] == 'o') {
                    plan[0].emplace_back(student.name, professor,
                                         professor.is_possible[0] == 'o');
                    if (professor.is_possible[0] == 'o') {
                        assignment_count[0][professor.name]++;
                    }
                } else {
                    plan[1].emplace_back(student.name, professor,
                                         professor.is_possible[1] == 'o');
                    if (professor.is_possible[1] == 'o') {
                        assignment_count[1][professor.name]++;
                    }
                }
                continue;
            }
            plan[index].emplace_back(student.name, professor,
                                     professor.is_possible[index] == 'o');
            if (professor.is_possible[index] == 'o') {
                assignment_count[index][professor.name]++;
            }
        }
    }

    auto assign_count = [&](std::string name) -> int {
        return assignment_count[0][name] + assignment_count[1][name];
    };

    for (int i = 0; i < window; i++) {
        // 教授の割り当て
        for (auto &slot : plan[i]) {
            if (slot.flag) continue;
            for (auto [professor, affiliation] :
                 assignments[i]
                            [static_cast<unsigned>(ProfessorType::professor)]) {
                if (slot.flag || slot.supervisor == professor) continue;
                if (assignment_count[i][professor] < max_assign) {
                    slot.assign_professor.emplace_back(professor);
                    slot.flag = true;
                    assignment_count[i][professor]++;
                    break;
                }
            }
            if (slot.flag) continue;

            std::string s = "";
            int min = std::numeric_limits<int>::max();
            for (auto [professor, affiliation] :
                 assignments[2]
                            [static_cast<unsigned>(ProfessorType::professor)]) {
                if (slot.flag || slot.supervisor == professor) continue;
                if (assignment_count[i][professor] < max_assign &&
                    assign_count(professor) / parameter_1 < min) {
                    min = assign_count(professor) / parameter_1;
                    s = professor;
                }
            }
            slot.assign_professor.emplace_back(s);
            slot.flag = true;
            assignment_count[i][s]++;
            assert(slot.flag);
        }

        for (int j = type_count - 1; j >= 0; j--) {
            for (auto [professor, affiliation] : assignments[i][j]) {
                for (auto &slot : plan[i]) {
                    if ((int)slot.assign_professor.size() == k) continue;

                    bool can_assign = slot.can_assign(professor, affiliation);
                    can_assign &= assignment_count[i][professor] < max_assign;
                    can_assign &=
                        static_cast<unsigned>(ProfessorType::assistant) != j ||
                        slot.assistant_count < ty_assistant;
                    for (auto name : slot.assign_professor) {
                        can_assign &= name != professor;
                    }
                    if (can_assign) {
                        slot.assign_professor.insert(
                            slot.assign_professor.end() -
                                slot.assistant_count * (j < 2),
                            professor);
                        assignment_count[i][professor]++;
                        if (static_cast<unsigned>(ProfessorType::assistant) ==
                            j)
                            slot.assistant_count++;
                    }
                }
            }
        }
        for (auto &slot : plan[i]) {
            while (slot.assign_professor.size() < k) {
                std::string s = "";
                int min = std::numeric_limits<int>::max();
                bool is_assistant = false;
                for (int j = type_count - 1; j >= 0; j--) {
                    for (auto [professor, affiliation] : assignments[2][j]) {
                        bool can_assign =
                            slot.can_assign(professor, affiliation);
                        can_assign &=
                            assignment_count[i][professor] < max_assign;
                        can_assign &= static_cast<unsigned>(
                                          ProfessorType::assistant) != j ||
                                      slot.assistant_count < ty_assistant;
                        for (auto name : slot.assign_professor) {
                            can_assign &= name != professor;
                        }
                        if (can_assign) {
                            if (assign_count(professor) / parameter_1 < min) {
                                is_assistant =
                                    j == static_cast<unsigned>(
                                             ProfessorType::assistant);
                                s = professor;
                                min = assign_count(professor) / parameter_1;
                            }
                        }
                    }
                }
                slot.assign_professor.insert(
                    slot.assign_professor.end() -
                        slot.assistant_count * (!is_assistant),
                    s);
                assignment_count[i][s]++;
                if (is_assistant) slot.assistant_count++;
            }
            std::sort(std::begin(slot.assign_professor) + 1, std::end(slot.assign_professor), [&](auto lhs, auto rhs) {
                return map[lhs] < map[rhs]; 
            });
        }
        for (auto [s, c] : assignment_count[i]) {
            assert(c <= max_assign);
        }
        for (auto slot : plan[i]) {
            assert(validator(slot, map));
        }
    }
    // for(auto professor: professors) std::cerr << assign_count(professor.name)
    // << std::endl;
    return plan;
}

void run(std::string professor_filename, std::string student_filename) {
    auto professors =
        concept_presentation_input(professor_filename, student_filename);
    auto plan = concept_presentation_assignment_solver(professors);
    concept_presentation_output(plan);
}