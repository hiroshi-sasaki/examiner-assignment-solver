#include "solver.h"

#include <algorithm>
#include <cassert>

#include "bit_dp_solver.h"
#include "io_util.h"
#include "student.h"

namespace bachelor_presentation {

bachelor_presentation_solver::bachelor_presentation_solver(
    std::string time_filename, std::string professor_base_info_filename,
    std::string professor_filename, std::string student_filename) {
    input(time_filename, professor_base_info_filename, professor_filename,
          student_filename);
}

void bachelor_presentation_solver::professor_assign(
    std::vector<Student> &suzukake_schedule,
    std::vector<Student> &oookayama_schedule) {
    auto professors = professors_;
    std::vector<Professor> oookayama_professors, suzukake_professors;
    for (auto p : professors) {
        if (p.get_campus() == "O") {
            oookayama_professors.emplace_back(p);
        } else {
            suzukake_professors.emplace_back(p);
        }
    }
    for (auto &student : suzukake_schedule) {
        if (!student.valid()) continue;
        assign_count[student.get_supervisor()]++;
        student.assign_professor(student.get_supervisor());
    }
    for (auto &student : oookayama_schedule) {
        if (!student.valid()) continue;
        assign_count[student.get_supervisor()]++;
        student.assign_professor(student.get_supervisor());
    }
    auto can_assign = [&](int i, std::string name) -> bool {
        auto student = i < time_info.count_per_day[0]
                           ? suzukake_schedule[i]
                           : oookayama_schedule[i - time_info.count_per_day[0]];
        if (!student.valid() || student.assign_count() == k) return false;
        for (auto professor : student.get_assign_professors()) {
            if (professor == name) return false;
        }
        for (auto prof : professors) {
            if (prof.get_name() == name) {
                return check(time_info, prof, i);
            }
        }
        return false;
    };
    auto validator = [&]() -> bool {
        for (auto student : suzukake_schedule) {
            if (!student.valid()) continue;
            if (student.assign_count() < k) {
                return false;
            }
        }
        for (auto student : oookayama_schedule) {
            if (!student.valid()) continue;
            if (student.assign_count() < k) {
                return false;
            }
        }
        return true;
    };
    int c = 0;
    while (!validator()) {
        std::sort(oookayama_professors.begin(), oookayama_professors.end(),
                  [&](auto lhs, auto rhs) -> bool {
                      return assign_count[lhs.get_name()] <
                             assign_count[rhs.get_name()];
                  });
        std::sort(suzukake_professors.begin(), suzukake_professors.end(),
                  [&](auto lhs, auto rhs) -> bool {
                      return assign_count[lhs.get_name()] <
                             assign_count[rhs.get_name()];
                  });
        for (auto prof : oookayama_professors) {
            int cont = 0;
            for (int i = 0; i < time_info.count_per_day[1]; i++) {
                if (can_assign(i + time_info.count_per_day[0],
                               prof.get_name())) {
                    oookayama_schedule[i].assign_professor(prof.get_name());
                    cont++;
                    assign_count[prof.get_name()]++;
                }
                if (cont >= 3) break;
            }
        }
        for (auto prof : suzukake_professors) {
            int cont = 0;
            for (int i = 0; i < time_info.count_per_day[0]; i++) {
                if (can_assign(i, prof.get_name())) {
                    cont++;
                    suzukake_schedule[i].assign_professor(prof.get_name());
                    assign_count[prof.get_name()]++;
                }
                if (cont >= 3) break;
            }
        }
    }
    return;
}

void bachelor_presentation_solver::run() {
    auto professors = professors_;
    std::vector<Professor> oookayama, suzukake;
    for (auto prof : professors) {
        if (prof.get_students().empty()) continue;
        if (prof.get_campus() == "O") {
            oookayama.emplace_back(prof);
        } else {
            assert(prof.get_campus() == "S");
            suzukake.emplace_back(prof);
        }
    }
    auto suzukake_sol = bit_dp_solver(0, time_info.accumulate_count_per_day[1],
                                      time_info, suzukake);
    auto suzukake_schedule = construct_schedule(
        0, time_info.accumulate_count_per_day[1], time_info, suzukake_sol);
    auto oookayama_sol = bit_dp_solver(time_info.accumulate_count_per_day[1],
                                       time_info.accumulate_count_per_day[2],
                                       time_info, oookayama);
    auto oookayama_schedule = construct_schedule(
        time_info.accumulate_count_per_day[1],
        time_info.accumulate_count_per_day[2], time_info, oookayama_sol);
    professor_assign(suzukake_schedule, oookayama_schedule);
    auto schedule = suzukake_schedule;
    schedule.insert(schedule.end(), oookayama_schedule.begin(),
                    oookayama_schedule.end());
    output_schedule(schedule, time_info);
}

}  // namespace bachelor_presentation
