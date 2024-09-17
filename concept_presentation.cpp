#include <algorithm>
#include <map>

#include "user.hpp"

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &vec) {
    for (std::size_t i = 0; i < vec.size(); i++)
        os << vec[i] << (i + 1 == vec.size() ? "" : " ");
    return os;
}

std::vector<std::vector<Slot>> concept_presentation_assignment_solver(
    std::vector<Professor> professors, int k) {
    const int window = 2;
    const int th = 4;
    std::vector plan(window, std::vector<Slot>());
    std::vector assignment(window + 1, std::vector<std::string>());
    std::vector<std::map<std::string, int>> assignment_count(window);

    int sum_student = 0;
    for (auto professor : professors) {
        sum_student += (int)professor.students.size();
    }

    int half = (sum_student + 1) / 2;

    std::sort(professors.begin(), professors.end(), [](auto a, auto b) -> bool {
        return a.students.size() > b.students.size();
    });

    for (auto professor : professors) {
        if (professor.is_possible[0] == 'o' &&
            professor.is_possible[1] == 'x') {
            assignment[0].emplace_back(professor.name);
            assignment_count[0][professor.name] +=
                (int)professor.students.size();
            for (auto student : professor.students) {
                plan[0].emplace_back(student.name, student.supervisor);
            }
        } else if (professor.is_possible[0] == 'x' &&
                   professor.is_possible[1] == 'o') {
            assignment[1].emplace_back(professor.name);
            assignment_count[1][professor.name] +=
                (int)professor.students.size();

            for (auto student : professor.students) {
                plan[1].emplace_back(student.name, student.supervisor);
            }
        } else {
            int student_count = (int)professor.students.size();
            bool can_assign = professor.is_possible[0] == 'o' &&
                              professor.is_possible[1] == 'o';

            if (student_count == 0) {
                if (can_assign) {
                    int index = 0;
                    if (assignment[0].size() > assignment[1].size()) {
                        index = 1;
                    }
                    assignment[index].emplace_back(professor.name);
                }
                continue;
            }

            if (student_count <= th) {
                int index = 0;
                if ((int)plan[0].size() + student_count > half) {
                    index = 1;
                }
                for (auto student : professor.students) {
                    plan[index].emplace_back(
                        student.name, can_assign ? student.supervisor : "null");
                }
                assignment_count[index][professor.name] +=
                    (int)professor.students.size();
                if (can_assign) {
                    assignment[index].emplace_back(professor.name);
                }
            } else {
                int count = 0;
                for (auto student : professor.students) {
                    int index = count >= student_count / 2;
                    plan[index].emplace_back(
                        student.name, can_assign ? student.supervisor : "null");
                }
                if (can_assign) {
                    assignment[0].emplace_back(professor.name);
                    assignment[1].emplace_back(professor.name);
                }
                assignment_count[0][professor.name] +=
                    (int)professor.students.size() / 2;
                assignment_count[1][professor.name] +=
                    (int)(professor.students.size() + 1) / 2;
            }
        }
    }

    for (int i = 0; i < window; i++) {
        int slot_count = (int)plan[i].size() * k;
        int ave_assignment_count =
            (slot_count + (int)assignment[i].size() - 1) /
            (int)assignment[i].size();

        for (auto &slot : plan[i]) {
            if (slot.supervisor != "null") {
                slot.assign_professor.emplace_back(slot.supervisor);
            }
        }

        for (auto professor : assignment[i]) {
            for (int j = 0; j < k; j++) {
                for (auto &slot : plan[i]) {
                    if ((int)slot.assign_professor.size() < j) continue;
                    if (slot.can_assign(professor)) {
                        slot.assign_professor.emplace_back(professor);
                        assignment_count[i][professor]++;
                        if (ave_assignment_count <
                            assignment_count[i][professor])
                            break;
                    }
                }
                if (ave_assignment_count < assignment_count[i][professor])
                    break;
            }
        }

        for (auto slot : plan[i]) {
            assert((int)slot.assign_professor.size() == k);
        }
    }
    return plan;
}
