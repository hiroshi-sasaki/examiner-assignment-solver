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

bool validator(Slot s, int k) {
    if (ty_assistant < s.assistant_count || !s.flag) return false;
    for (int i = 0; i < k; i++) {
        for (int j = i + 1; j < k; j++) {
            if (s.assign_professor[i] == s.assign_professor[j]) return false;
        }
    }
    return s.flag;
}

std::vector<std::vector<Slot>> concept_presentation_assignment_solver(
    std::vector<Professor> professors) {
    std::vector plan(window, std::vector<Slot>());
    std::vector assignments(
        window,
        std::vector(type_count,
                    std::vector<std::pair<std::string, std::string>>()));
    std::vector<std::map<std::string, int>> assignment_count(window);

    int sum_student = 0;
    for (auto professor : professors) {
        int student_count = (int)professor.students.size();
        sum_student += (int)professor.students.size();
        int half = student_count;
        if (student_count > max_assign) half = (student_count + 1) / 2;

        int index = 0;
        if (professor.is_possible[0] == 'o' &&
            professor.is_possible[1] == 'x') {
            assignments[0][static_cast<unsigned>(professor.type)].emplace_back(
                professor.name, professor.affiliation);
            index = 0;
        } else if (professor.is_possible[0] == 'x' &&
                   professor.is_possible[1] == 'o') {
            assignments[1][static_cast<unsigned>(professor.type)].emplace_back(
                professor.name, professor.affiliation);
            index = 1;
        } else
            continue;

        int first = 0;
        for (auto student : professor.students) {
            if (student.is_possible[0] == student.is_possible[1]) continue;
            if (student.is_possible[0] == 'o') {
                assignment_count[0][professor.name]++;
                first++;
                plan[0].emplace_back(student.name, professor, index == 0);
            } else {
                assignment_count[1][professor.name]++;
                plan[1].emplace_back(student.name, professor, index == 1);
            }
        }
        for (auto student : professor.students) {
            if (student.is_possible[0] != student.is_possible[1]) continue;
            if (first < half) {
                assignment_count[index][professor.name]++;
                plan[index].emplace_back(student.name, professor, true);
                first++;
            } else {
                // assignment_count[index ^ 1][professor.name]++;
                plan[index ^ 1].emplace_back(student.name, professor, false);
            }
        }
    }

    std::sort(professors.begin(), professors.end(), [](auto a, auto b) -> bool {
        return a.students.size() > b.students.size();
    });

    auto f = [&](int i) -> int {
        int sum = 0;
        for (int j = 0; j < type_count; j++) {
            sum += assignments[i][j].size();
        }
        return sum;
    };

    for (auto professor : professors) {
        if (professor.is_possible[0] == professor.is_possible[1]) {
            int student_count = (int)professor.students.size();
            bool can_assign = professor.is_possible[0] == 'o' &&
                              professor.is_possible[1] == 'o';
            int type = static_cast<unsigned>(professor.type);

            if (student_count == 0) {
                if (can_assign) {
                    int index = 0;
                    if (f(0) > f(1)) {
                        index = 1;
                    }
                    assignments[index][type].emplace_back(
                        professor.name, professor.affiliation);
                }
                continue;
            }

            {
                int index = 0;
                int first = 0;
                int half = student_count;
                if (student_count > max_assign) half = (student_count + 1) / 2;
                if (assignments[0][type].size() > assignments[1][type].size() + 3) {
                    index = 1;
                }
                if (plan[0].size() > plan[1].size()) {
                    index = 1;
                }
                for (auto student : professor.students) {
                    if (student.is_possible[0] == student.is_possible[1])
                        continue;
                    if (student.is_possible[0] == 'o') {
                        assignment_count[0][professor.name]++;
                        first++;
                        plan[0].emplace_back(student.name, professor,
                                             can_assign && index == 0);
                    } else {
                        assignment_count[1][professor.name]++;
                        plan[1].emplace_back(student.name, professor,
                                             can_assign && index == 1);
                    }
                }
                for (auto student : professor.students) {
                    if (student.is_possible[0] != student.is_possible[1])
                        continue;
                    if (first < half) {
                        assignment_count[index][professor.name]++;
                        plan[index].emplace_back(student.name, professor,
                                                 can_assign);
                        first++;
                    } else {
                        assignment_count[index ^ 1][professor.name]++;
                        plan[index ^ 1].emplace_back(student.name, professor,
                                                     false);
                    }
                }
                if (can_assign) {
                    assignments[index][type].emplace_back(
                        professor.name, professor.affiliation);
                }
            }
        }
    }

    for (int i = 0; i < window; i++) {
        // 教授の割り当て
        for (auto &slot : plan[i]) {
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
            if (!slot.flag) {
                int min = std::numeric_limits<int>::max();
                std::string s = "", a = "";
                for (auto [professor, affiliation] :
                     assignments[i ^ 1][static_cast<unsigned>(
                         ProfessorType::professor)]) {
                    if (!slot.can_assign(professor, affiliation)) continue;
                    if (assignment_count[i ^ 1][professor] < min) {
                        min = assignment_count[i][professor];
                        s = professor;
                    }
                }
                slot.assign_professor.emplace_back(s);
                slot.flag = true;
                assignment_count[i][s]++;
            }
        }

        // 助教授の割り当て
        for (auto &slot : plan[i]) {
            for (int j = 0; j < ty_assistant; j++) {
                for (auto [assistant, affiliation] :
                     assignments[i][static_cast<unsigned>(
                         ProfessorType::assistant)]) {
                    if (!slot.can_assign(assistant, affiliation)) continue;
                    if (assignment_count[i][assistant] < max_assign) {
                        slot.assign_professor.emplace_back(assistant);
                        slot.assistant_count++;
                        assignment_count[i][assistant]++;
                        break;
                    }
                }
            }
        }

        for (int j = 0; j < type_count; j++) {
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
        /*
        for (auto &slot : plan[i]) {
            while ((int)slot.assign_professor.size() < k) {
                int min = std::numeric_limits<int>::max();
                std::string s = "", a = "";
                bool is_assistant = false;
                for (auto [professor, affiliation] :
                     assignments[i][static_cast<unsigned>(
                         ProfessorType::professor)]) {
                    if (!slot.can_assign(professor, affiliation)) continue;
                    if (assignment_count[i][professor] < min) {
                        min = assignment_count[i][professor];
                        s = professor;
                    }
                }
                for (auto [professor, affiliation] :
                     assignments[i][static_cast<unsigned>(
                         ProfessorType::associate)]) {
                    if (!slot.can_assign(professor, affiliation)) continue;

                    if (assignment_count[i][professor] < min) {
                        min = assignment_count[i][professor];
                        s = professor;
                    }
                }
                if (slot.assistant_count < ty_assistant) {
                    for (auto [professor, affiliation] :
                         assignments[i][static_cast<unsigned>(
                             ProfessorType::associate)]) {
                        if (!slot.can_assign(professor, affiliation)) continue;

                        if (assignment_count[i][professor] < min) {
                            min = assignment_count[i][professor];
                            s = professor;
                            a = affiliation;
                            is_assistant = true;
                        }
                    }
                }
                slot.assign(s, a);
                assignment_count[i][s]++;
                if (is_assistant) {
                    slot.assistant_count++;
                }
            }
            assert(validator(slot, k));
        }
        */
    }
    for(int i = 0; i < 2; i++) {
        for (auto &slot : plan[i ^ 1]) {
            while ((int)slot.assign_professor.size() < k) {
                int min = std::numeric_limits<int>::max();
                std::string s = "", a = "";
                bool is_assistant = false;
                for (auto [professor, affiliation] :
                     assignments[i][static_cast<unsigned>(
                         ProfessorType::professor)]) {
                    if (!slot.can_assign(professor, affiliation)) continue;
                    if (assignment_count[i][professor] < min) {
                        min = assignment_count[i][professor];
                        s = professor;
                    }
                }
                for (auto [professor, affiliation] :
                     assignments[i][static_cast<unsigned>(
                         ProfessorType::associate)]) {
                    if (!slot.can_assign(professor, affiliation)) continue;

                    if (assignment_count[i][professor] < min) {
                        min = assignment_count[i][professor];
                        s = professor;
                    }
                }
                if (slot.assistant_count < ty_assistant) {
                    for (auto [professor, affiliation] :
                         assignments[i][static_cast<unsigned>(
                             ProfessorType::associate)]) {
                        if (!slot.can_assign(professor, affiliation)) continue;

                        if (assignment_count[i][professor] < min) {
                            min = assignment_count[i][professor];
                            s = professor;
                            a = affiliation;
                            is_assistant = true;
                        }
                    }
                }
                slot.assign(s, a);
                assignment_count[i][s]++;
                if (is_assistant) {
                    slot.assistant_count++;
                }
            }
            assert(validator(slot, k));
        }
    }
    for(int i = 0; i < 2; i++) for(auto [s, c]: assignment_count[i]) std::cerr << s << " " << c << std::endl;
    return plan;
}

void run(std::string professor_filename, std::string student_filename) {
    auto professors =
        concept_presentation_input(professor_filename, student_filename);
    auto plan = concept_presentation_assignment_solver(professors);
    concept_presentation_output(plan);
}