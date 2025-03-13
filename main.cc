#include <cassert>

#include "bachelor_presentation/solver.h"
#include "concept_presentation/solver.h"
#include "intermediate_examination/solver.h"
#include "master_presentation/solver.h"
#include "research_project/solver.h"

int main(int argc, char *argv[]) {
    assert(argc > 1);
    std::string event = argv[1];
    if (event == "concept_presentation") {
        std::string time_info_filename = argv[2];
        std::string professor_base_info_filename = argv[3];
        std::string professor_filename = argv[4];
        std::string student_filename = argv[5];
        concept_presentation::run(time_info_filename,
                                  professor_base_info_filename,
                                  professor_filename, student_filename);
    } else if (event == "research_project") {
        std::string student_filename = argv[2];
        int parallel = std::stoi(argv[3]);
        research_project::run(student_filename, parallel);
    } else if (event == "intermediate_examination") {
        std::string time_filename = argv[2];
        std::string professor_base_info_filename = argv[3];
        std::string professor_filename = argv[4];
        std::string student_filename = argv[5];
        intermediate_presentation::intermediate_presentation_solver solver(
            time_filename, professor_base_info_filename, professor_filename,
            student_filename);
        solver.run();
    } else if (event == "bachelor_presentation") {
        std::string time_filename = argv[2];
        std::string professor_base_info_filename = argv[3];
        std::string professor_filename = argv[4];
        std::string student_filename = argv[5];
        bachelor_presentation::bachelor_presentation_solver solver(
            time_filename, professor_base_info_filename, professor_filename,
            student_filename);
        solver.run();
    } else if (event == "master_presentation") {
        std::string time_filename = argv[2];
        std::string professor_base_info_filename = argv[3];
        std::string professor_filename = argv[4];
        std::string student_filename = argv[5];
        master_presentation::master_presentation_solver solver(
            time_filename, professor_base_info_filename, professor_filename,
            student_filename);
        solver.run();
    }
    else {
        std::cerr << event << "というイベントはありません" << std::endl;
    }
}