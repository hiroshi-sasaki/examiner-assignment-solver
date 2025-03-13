#include "solver.h"

#include <algorithm>
#include <iostream>

#include "bit_dp_solver.h"
#include "io_util.h"
namespace intermediate_presentation {

intermediate_presentation_solver::intermediate_presentation_solver(
    std::string time_filename, std::string professor_base_info_filename, std::string professor_filename,
    std::string student_filename) {
    input(time_filename, professor_base_info_filename, professor_filename, student_filename);
}

void intermediate_presentation_solver::professor_assign(
    std::vector<Student> &schedule) {
    auto professors = professors_;
    for (auto &student : schedule) {
        if (!student.valid()) continue;
        assign_count[student.get_supervisor()]++;
        student.assign_professor(student.get_supervisor());
    }
    auto can_assign = [&](int i, std::string name) -> bool {
        auto student = schedule[i];
        if (!student.valid() || student.get_assign_professors().size() == k)
            return false;
        for (auto professor : student.get_assign_professors()) {
            if (professor == name) return false;
        }
        for (auto prof : professors) {
            if (prof.is_same_name(name)) {
                return check(time_info, prof, i);
            }
        }
        return false;
    };

    auto validator = [&]() -> bool {
        for (auto student : schedule) {
            if (!student.valid()) continue;
            if (student.get_assign_professors().size() < k) {
                return false;
            }
        }
        return true;
    };
    while (!validator()) {
        std::vector<std::pair<int, std::string>> s;
        for (auto [name, count] : assign_count) {
            s.emplace_back(count, name);
        }
        std::sort(s.begin(), s.end());
        for (auto [count_, name] : s) {
            int now_assign = 0;
            for (int i = 0; i < schedule.size(); i++) {
                if (can_assign(i, name)) {
                    now_assign++;
                    schedule[i].assign_professor(name);
                    assign_count[name]++;
                }
                if (now_assign >= 3) break;
            }
        }
    }
    return;
}

void intermediate_presentation_solver::run() {
    auto professors = professors_;
    auto sol = bit_dp_solver(0, time_info.accumulate_count_per_day.back(),
                             time_info, professors);
    auto schedule = construct_schedule(
        0, time_info.accumulate_count_per_day.back(), time_info, sol);
    professor_assign(schedule);
    output_schedule(schedule, time_info);
}

}  // namespace intermediate_presentation