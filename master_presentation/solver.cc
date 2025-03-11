#include "solver.h"

#include <cassert>

#include "bit_dp_solver.h"

namespace master_presentation {

master_presentation_solver::master_presentation_solver(
    std::string time_filename, std::string professor_base_info_filename,
    std::string professor_filename, std::string student_filename) {
    input(time_filename, professor_base_info_filename, professor_filename,
          student_filename);
}

void master_presentation_solver::run() {
    auto professors = professors_;

    std::vector<Professor> oookayama, suzukake;
    for (auto prof : professors) {
        if (prof.get_students().empty()) continue;
        if (prof.get_campus() == "O") {
            oookayama.emplace_back(prof);
        } else {
            assert(prof.get_campus() == "S");
            if (prof.get_students().size() > 5) {
                auto lhs = prof;
                auto rhs = prof;
                lhs.get_students() = {prof.get_students().begin(),
                                      prof.get_students().begin() + 4};
                rhs.get_students() = {prof.get_students().begin() + 4,
                                      prof.get_students().end()};
                suzukake.emplace_back(lhs);
                suzukake.emplace_back(rhs);

                continue;
            }
            suzukake.emplace_back(prof);
        }
    }

    auto suzukake_sol = bit_dp_solver(0, time_info.accumulate_count_per_day[3],
                                      time_info, suzukake);
    auto suzukake_schedule = construct_schedule(
        0, time_info.accumulate_count_per_day[3], time_info, suzukake_sol);
    auto oookayama_sol = bit_dp_solver(time_info.accumulate_count_per_day[3],
                                       time_info.accumulate_count_per_day[4],
                                       time_info, oookayama);
    auto oookayama_schedule = construct_schedule(
        time_info.accumulate_count_per_day[3],
        time_info.accumulate_count_per_day[4], time_info, oookayama_sol);

    auto schedule = suzukake_schedule;
    schedule.insert(schedule.end(), oookayama_schedule.begin(),
                    oookayama_schedule.end());
}

}  // namespace master_presentation