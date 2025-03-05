#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <map>
#include <random>
#include <set>

#include "io.h"
#include "slot.h"

namespace concept_presentation {

extern int k;
extern int window;
extern int ty_assistant;
extern int type_count;
extern int max_assign;
extern int parameter_1;

bool validator(Slot s, std::map<std::string, int> &map) {
    if (ty_assistant < s.assistant_count || !s.is_assigned_professor)
        return false;
    if ((int)s.assign_professors.size() != k) return false;
    int c = 0;
    for (int i = 0; i < k; i++) {
        c += map[s.assign_professors[i]] == 2;
        if (k == 3 && s.assign_professors[i] == s.presenter.get_supervisor())
            return false;
        for (int j = i + 1; j < k; j++) {
            if (s.assign_professors[i] == s.assign_professors[j]) return false;
        }
    }
    assert(c == s.assistant_count);
    return s.is_assigned_professor;
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
    std::set<std::string> already_assigned;

    int sum_student = 0;
    for (auto &professor : professors) {
        map[professor.get_name()] =
            static_cast<int>(professor.get_professor_type());
        int student_count = (int)professor.get_students().size();
        sum_student += student_count;
        if (professor.get_is_possible() == "oo")
            assignments[2]
                       [static_cast<unsigned>(professor.get_professor_type())]
                           .emplace_back(professor.get_name(),
                                         professor.get_affiliation());
        else {
            for (int i = 0; i < 2; i++) {
                if (professor.get_is_possible()[i] == 'o') {
                    assignments[i][static_cast<unsigned>(
                                       professor.get_professor_type())]
                        .emplace_back(professor.get_name(),
                                      professor.get_affiliation());
                }
            }
        }

        std::sort(std::begin(professor.get_students()),
                  std::end(professor.get_students()), [](auto lhs, auto rhs) {
                      int l = int(lhs.get_is_possible()[0] == 'o') +
                              int(lhs.get_is_possible()[1] == 'o');
                      int r = int(rhs.get_is_possible()[0] == 'o') +
                              int(rhs.get_is_possible()[1] == 'o');
                      return l < r;
                  });

        if (student_count > max_assign) {
            already_assigned.insert(professor.get_name());

            int first = 0;
            int half = (student_count + 1) / 2;
            for (auto student : professor.get_students()) {
                if (student.get_is_possible()[1] == 'x' || first < half) {
                    assert(student.get_is_possible()[0] == 'o');
                    plan[0].emplace_back(student, professor,
                                         professor.get_is_possible()[0] == 'o');
                    if (!plan[0].back().assign_professors.empty())
                        assignment_count[0][professor.get_name()]++;
                    first++;
                } else {
                    assert(student.get_is_possible()[1] == 'o');
                    plan[1].emplace_back(student, professor,
                                         professor.get_is_possible()[1] == 'o');
                    if (!plan[1].back().assign_professors.empty())
                        assignment_count[1][professor.get_name()]++;
                }
            }
            continue;
        }

        {
            bool flag = true;
            for (auto student : professor.get_students()) {
                flag &= student.get_is_possible()[0] ==
                        student.get_is_possible()[1];
            }
            if (flag || professor.get_is_possible()[0] !=
                            professor.get_is_possible()[1])
                continue;
            already_assigned.insert(professor.get_name());
            int index = -1;
            for (auto student : professor.get_students()) {
                if (student.get_is_possible()[1] == 'x') {
                    index = 0;
                    assert(student.get_is_possible()[0] == 'o');
                    plan[0].emplace_back(student, professor,
                                         professor.get_is_possible()[0] == 'o');
                    if (!plan[0].back().assign_professors.empty())
                        assignment_count[0][professor.get_name()]++;
                } else if (student.get_is_possible()[0] == 'x') {
                    index = 1;
                    assert(student.get_is_possible()[1] == 'o');
                    plan[1].emplace_back(student, professor,
                                         professor.get_is_possible()[1] == 'o');
                    if (!plan[1].back().assign_professors.empty())
                        assignment_count[1][professor.get_name()]++;
                } else {
                    if (professor.get_is_possible()[0] !=
                        professor.get_is_possible()[1]) {
                        if (professor.get_is_possible()[0] == 'o') {
                            plan[0].emplace_back(
                                student, professor,
                                professor.get_is_possible()[0] == 'o');
                            if (!plan[0].back().assign_professors.empty())
                                assignment_count[0][professor.get_name()]++;
                        } else {
                            plan[1].emplace_back(
                                student, professor,
                                professor.get_is_possible()[1] == 'o');
                            if (!plan[1].back().assign_professors.empty())
                                assignment_count[1][professor.get_name()]++;
                        }
                    } else {
                        plan[index].emplace_back(
                            student, professor,
                            professor.get_is_possible()[index] == 'o');
                        if (!plan[index].back().assign_professors.empty())
                            assignment_count[index][professor.get_name()]++;
                    }
                }
            }
        }
    }
    for (auto &professor : professors) {
        int student_count = (int)professor.get_students().size();
        if (already_assigned.contains(professor.get_name()) ||
            student_count == 0)
            continue;
        int index = 0;
        if ((int)plan[0].size() + student_count > (sum_student + 1) / 2) {
            index = 1;
        }

        for (auto student : professor.get_students()) {
            if (student.get_is_possible()[0] != student.get_is_possible()[1]) {
                if (student.get_is_possible()[0] == 'o') {
                    plan[0].emplace_back(student, professor,
                                         professor.get_is_possible()[0] == 'o');
                    if (!plan[0].back().assign_professors.empty()) {
                        assignment_count[0][professor.get_name()]++;
                    }
                } else {
                    plan[1].emplace_back(student, professor,
                                         professor.get_is_possible()[1] == 'o');
                    if (!plan[1].back().assign_professors.empty()) {
                        assignment_count[1][professor.get_name()]++;
                    }
                }
                continue;
            }
            plan[index].emplace_back(student, professor,
                                     professor.get_is_possible()[index] == 'o');
            if (!plan[index].back().assign_professors.empty()) {
                assignment_count[index][professor.get_name()]++;
            }
        }
        already_assigned.insert(professor.get_name());
    }

    auto assign_count = [&](std::string name) -> int {
        return assignment_count[0][name] + assignment_count[1][name];
    };

    for (int i = 0; i < window; i++) {
        // 教授の割り当て
        for (auto &slot : plan[i]) {
            if (slot.is_assigned_professor) continue;
            for (auto [professor, affiliation] :
                 assignments[i]
                            [static_cast<unsigned>(ProfessorType::professor)]) {
                if (slot.is_assigned_professor ||
                    slot.presenter.get_supervisor() == professor)
                    continue;
                if (assignment_count[i][professor] < max_assign) {
                    slot.assign(professor, affiliation);
                    slot.is_assigned_professor = true;
                    assignment_count[i][professor]++;
                    break;
                }
            }
            if (slot.is_assigned_professor) continue;

            std::string p = "", a = "";
            int min = std::numeric_limits<int>::max();
            for (auto [professor, affiliation] :
                 assignments[2]
                            [static_cast<unsigned>(ProfessorType::professor)]) {
                if (slot.presenter.get_supervisor() == professor) continue;
                if (assignment_count[i][professor] < max_assign &&
                    assign_count(professor) / parameter_1 < min) {
                    min = assign_count(professor) / parameter_1;
                    p = professor;
                    a = affiliation;
                }
            }
            slot.assign(p, a);
            slot.is_assigned_professor = true;
            assignment_count[i][p]++;
            assert(slot.is_assigned_professor);
        }

        for (int j = type_count - 1; j >= 0; j--) {
            for (auto [professor, affiliation] : assignments[i][j]) {
                for (auto &slot : plan[i]) {
                    if ((int)slot.assign_professors.size() == k) continue;

                    bool can_assign = slot.can_assign(professor, affiliation);
                    can_assign &= assignment_count[i][professor] < max_assign;
                    can_assign &=
                        static_cast<unsigned>(ProfessorType::assistant) != j ||
                        slot.assistant_count < ty_assistant;
                    for (auto name : slot.assign_professors) {
                        can_assign &= name != professor;
                    }
                    if (can_assign) {
                        slot.assign_professors.insert(
                            slot.assign_professors.end() -
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
    }

    // どっちもokを割り当て
    std::vector<std::pair<int, int>> indexs;
    for (int i = 0; i < window; i++) {
        for (int j = 0; j < (int)plan[i].size(); j++) {
            if (plan[i][j].assign_professors.size() < k)
                indexs.emplace_back(i, j);
        }
    }

    for (auto [i, j] : indexs) {
        auto &slot = plan[i][j];
        {
            while (slot.assign_professors.size() < k) {
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
                        for (auto name : slot.assign_professors) {
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
                slot.assign_professors.insert(
                    slot.assign_professors.end() -
                        slot.assistant_count * (!is_assistant),
                    s);
                assignment_count[i][s]++;
                if (is_assistant) slot.assistant_count++;
            }
        }
    }
    for (int i = 0; i < window; i++) {
        for (auto &slot : plan[i]) {
            std::sort(std::begin(slot.assign_professors) + 1,
                      std::end(slot.assign_professors),
                      [&](auto lhs, auto rhs) { return map[lhs] < map[rhs]; });
            assert(validator(slot, map));
        }
    }

    std::cout << "教員, 前半, 後半, 合計" << std::endl;
    for (auto professor : professors)
        std::cout << professor.get_name() << ", "
                  << assignment_count[0][professor.get_name()] << ", "
                  << assignment_count[1][professor.get_name()] << ", "
                  << assign_count(professor.get_name()) << std::endl;
    return plan;
}

void run(std::string professor_filename, std::string student_filename) {
    auto professors =
        concept_presentation_input(professor_filename, student_filename);
    auto plan = concept_presentation_assignment_solver(professors);
    concept_presentation_output(plan);
}

}  // namespace concept_presentation