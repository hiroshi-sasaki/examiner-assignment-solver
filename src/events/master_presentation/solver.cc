#include "solver.h"

#include <cassert>

#include "bit_dp_solver.h"
#include "io_util.h"
namespace master_presentation {

master_presentation_solver::master_presentation_solver(
    std::string time_filename, std::string professor_filename,
    std::string student_filename) {
    input(time_filename, professor_filename, student_filename);
}

void master_presentation_solver::run() {
    auto professors = professors_;

    std::vector<Professor> ookayama, suzukake;
    for (auto prof : professors) {
        if (prof.get_students().empty()) continue;
        if (prof.get_campus() == "O") {
            ookayama.emplace_back(prof);
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

    // Use campus-aware slot allocation based on parsed labels
    int suzukake_start = time_info.suzukake_slots.empty() ? 0 : time_info.suzukake_slots.front();
    int suzukake_end = time_info.suzukake_slots.empty() ? 
        time_info.accumulate_count_per_day[time_info.suzukake_day_count] : 
        time_info.suzukake_slots.back() + 1;

    int ookayama_start = time_info.ookayama_slots.empty() ? 
        time_info.accumulate_count_per_day[time_info.suzukake_day_count] :
        time_info.ookayama_slots.front();
    int ookayama_end = time_info.ookayama_slots.empty() ?
        time_info.accumulate_count_per_day[time_info.day] :
        time_info.ookayama_slots.back() + 1;

    auto suzukake_sol = bit_dp_solver(suzukake_start, suzukake_end, time_info, suzukake);
    auto suzukake_schedule = construct_schedule(suzukake_start, suzukake_end, time_info, suzukake_sol);

    auto ookayama_sol = bit_dp_solver(ookayama_start, ookayama_end, time_info, ookayama);
    auto ookayama_schedule = construct_schedule(ookayama_start, ookayama_end, time_info, ookayama_sol);

    auto schedule = suzukake_schedule;
    schedule.insert(schedule.end(), ookayama_schedule.begin(),
                    ookayama_schedule.end());
    output_schedule(schedule, time_info);
}

}  // namespace master_presentation
