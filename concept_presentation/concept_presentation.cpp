#include <algorithm>
#include <limits>
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
    const int ty_assistant = 2;
    const int type_count = 3;
    std::vector plan(window, std::vector<Slot>());
    std::vector assignments(
        window, std::vector(type_count, std::vector<std::string>()));
    std::vector<std::map<std::string, int>> assignment_count(window);

    int sum_student = 0;
    for (auto professor : professors) {
        sum_student += (int)professor.students.size();
        if (professor.is_possible[0] == 'o' &&
            professor.is_possible[1] == 'x') {
            assignments[0][static_cast<unsigned>(professor.type)].emplace_back(
                professor.name);
            assignment_count[0][professor.name] +=
                (int)professor.students.size();
            for (auto student : professor.students) {
                plan[0].emplace_back(student.name, professor, true);
            }
        } else if (professor.is_possible[0] == 'x' &&
                   professor.is_possible[1] == 'o') {
            assignments[1][static_cast<unsigned>(professor.type)].emplace_back(
                professor.name);
            assignment_count[1][professor.name] +=
                (int)professor.students.size();

            for (auto student : professor.students) {
                plan[1].emplace_back(student.name, professor, true);
            }
        }
    }

    int half = (sum_student + 1) / 2;

    std::sort(professors.begin(), professors.end(), [](auto a, auto b) -> bool {
        return a.students.size() > b.students.size();
    });

    for (auto professor : professors) {
        if (professor.is_possible[0] == professor.is_possible[1]) {
            int student_count = (int)professor.students.size();
            bool can_assign = professor.is_possible[0] == 'o' &&
                              professor.is_possible[1] == 'o';
            int type = static_cast<unsigned>(professor.type);

            if (student_count == 0) {
                if (can_assign) {
                    int index = 0;
                    if (assignments[0][type].size() >
                        assignments[1][type].size()) {
                        index = 1;
                    }
                    assignments[index][type].emplace_back(professor.name);
                }
                continue;
            }

            {
                int index = 0;
                if ((int)plan[0].size() + student_count > half) {
                    index = 1;
                }
                if ((int)plan[1].size() + student_count <= half && assignments[0][type].size() > assignments[1][type].size()) {
                    index = 1;
                }
                for (auto student : professor.students) {
                    plan[index].emplace_back(student.name, professor,
                                             can_assign);
                }
                assignment_count[index][professor.name] +=
                    (int)professor.students.size();
                if (can_assign) {
                    assignments[index][type]
                        .emplace_back(professor.name);
                }
            }
            /*
            指導学生が多い場合に分割する処理
            {
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
            */
        }
    }

    for (int i = 0; i < window; i++) {
        int slot_count = (int)plan[i].size() * k;
        int assign_count = assignments[i][0].size() + assignments[i][1].size() +
                           assignments[i][2].size();
        int ave_assignment_count =
            (slot_count + assign_count - 1) / assign_count;

        // 教授の割り当て
        for (auto &slot : plan[i]) {
            int min = std::numeric_limits<int>::max();
            std::string name = "null";
            for (auto professor : assignments[i][static_cast<unsigned>(
                     ProfessorType::professor)]) {
                if (slot.flag || slot.supervisor == professor) continue;
                if (assignment_count[i][professor] < ave_assignment_count) {
                    slot.assign_professor.emplace_back(professor);
                    slot.flag = true;
                    assignment_count[i][professor]++;
                    break;
                }
                else {
                    if(assignment_count[i][professor] < min) {
                        min = assignment_count[i][professor];
                        name = professor;
                    }                    
                }
            }
            if(!slot.flag) {
                slot.flag = true;
                slot.assign_professor.emplace_back(name);
                assignment_count[i][name]++;
            }
        }

        // 助教授の割り当て
        for (auto &slot : plan[i]) {
            int min = std::numeric_limits<int>::max();
            std::string name = "null";
            bool is_assigned = false;
            for (auto assistant : assignments[i][static_cast<unsigned>(
                     ProfessorType::assistant)]) {
                if (assignment_count[i][assistant] < ave_assignment_count) {
                    slot.assign_professor.emplace_back(assistant);
                    slot.assistant_count++;
                    assignment_count[i][assistant]++;
                    is_assigned = true;
                    break;
                }
                else {
                    if(assignment_count[i][assistant] < min) {
                        min = assignment_count[i][assistant];
                        name = assistant;
                    }
                }
            }
            if(!is_assigned) {
                slot.assistant_count++;
                slot.assign_professor.emplace_back(name);
                assignment_count[i][name]++;
            }
        }

        for (int j = 0; j < type_count; j++) {
            for (auto professor : assignments[i][j]) {
                for(auto &slot: plan[i]) {
                    if((int)slot.assign_professor.size() == k) continue;
                    bool can_assign = assignment_count[i][professor] < ave_assignment_count;
                    can_assign &= static_cast<unsigned>(ProfessorType::assistant) != j || slot.assistant_count < ty_assistant;
                    for(auto name: slot.assign_professor) {
                        can_assign &= name != professor;
                    }
                    if(can_assign) {
                        slot.assign_professor.insert(slot.assign_professor.end() - 1, professor);
                        assignment_count[i][professor]++;
                        if(static_cast<unsigned>(ProfessorType::assistant) == j) slot.assistant_count++;
                    }
                }
            }
        }

        for (auto slot : plan[i]) {
            assert((int)slot.assign_professor.size() == k);
        }
    }
    return plan;
}
