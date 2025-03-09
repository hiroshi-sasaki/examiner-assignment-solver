#include "solver.h"

#include <algorithm>

#include "bit_dp_solver.h"

namespace intermediate_presentation {

void intermediate_presentation_solver::professor_assign(
    std::vector<Student> &schedule) {
    auto professors = professors_;
    auto can_assign = [&](int i, std::string name) -> bool {
        auto student = schedule[i];
        if (student.get_assign_professors().size() == k ||
            student.get_name() == "N/A")
            return false;
        for (auto professor : student.get_assign_professors()) {
            if (professor == name) return false;
        }
        for (auto prof : professors) {
            if (prof.get_name() == name) {
                return prof.can_assign(i);
            }
        }
        return false;
    };
    auto validator = [&]() -> bool {
        for (auto student : schedule) {
            if (student.get_name() == "N/A") continue;
            if (student.get_assign_professors().size() < k) return false;
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
}

}  // namespace intermediate_presentation