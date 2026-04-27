#include "solver.h"

#include <algorithm>
#include <iostream>
#include <random>

#include "bit_dp_solver.h"
#include "io_util.h"
namespace intermediate_presentation {

intermediate_presentation_solver::intermediate_presentation_solver(
    std::string time_filename, std::string professor_filename,
    std::string student_filename) {
    input(time_filename, professor_filename, student_filename);
}

void intermediate_presentation_solver::professor_assign(
    std::vector<Student> &schedule) {
    auto professors = professors_;

    // Initialize assign_count with all professors
    for (const auto& prof : professors) {
        assign_count[prof.get_name()] = 0;
    }

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
        std::vector<std::tuple<int, int, std::string>> s;
        for (auto [name, count] : assign_count) {
            // Find professor type for priority (lower number = higher priority)
            int rank_priority = 2; // default to assistant priority
            for (const auto& prof : professors_) {
                if (prof.get_name() == name) {
                    rank_priority = static_cast<int>(prof.get_professor_type());
                    break;
                }
            }
            s.emplace_back(rank_priority, count, name);
        }
        std::sort(s.begin(), s.end());

        // Shuffle professors with same counts and ranks for fairness
        static std::random_device rd;
        static std::mt19937 rng(rd());
        auto it = s.begin();
        while (it != s.end()) {
            auto next = it;
            // Find all entries with same count and rank
            while (next != s.end() && std::get<0>(*next) == std::get<0>(*it) && std::get<1>(*next) == std::get<1>(*it)) {
                ++next;
            }
            std::shuffle(it, next, rng);
            it = next;
        }

        for (auto [rank_, count_, name] : s) {
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

    auto final_count = count_final_assignments(schedule);

    std::cout << "\nAssignment Summary:\n";
    std::cout << "============================\n";
    for (const auto& [name, count] : final_count) {
        std::cout << name << ": " << count << " students\n";
    }
    std::cout << "============================\n\n";

    return;
}

std::map<std::string, int> intermediate_presentation_solver::count_final_assignments(const std::vector<Student>& schedule) {
    std::map<std::string, int> final_count;
    for (const auto& prof : professors_) {
        final_count[prof.get_name()] = 0;
    }
    for (const auto& student : schedule) {
        if (!student.valid()) continue;
        for (const auto& prof_name : student.get_assign_professors()) {
            final_count[prof_name]++;
        }
    }
    return final_count;
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
