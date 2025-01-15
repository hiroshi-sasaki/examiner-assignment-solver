#include <cassert>
#include <string>

#include "concept_presentation/concept_presentation.cpp"
#include "research_project/research_project.cpp"
#include "masters_presentation/solver.cpp"
#include "masters_presentation/io.cpp"
#include "masters_presentation/intermediate_examination.cpp"
#include "util/io_util.cpp"

int main(int argc, char *argv[]) {
    std::string event = argv[1];
    if (event == "concept_presentation") {
        assert(argc == 5);
        std::string professor_filename = argv[2];
        std::string student_filename = argv[3];
        concept_presentation::k = std::stoi(argv[4]);
        concept_presentation::run(professor_filename, student_filename);
    } else if (event == "research_project") {
        assert(argc == 4);
        std::string filename = argv[2];
        int type = std::stoi(argv[3]);
        research_project::run(filename, type);
    } else if(event == "masters_presentation") {
        assert(argc == 5 || argc == 6);
        std::string time_filename = argv[2];
        std::string professor_filename = argv[3];
        std::string student_filename = argv[4];
        examiner_assignment::examiner_assignment_solver solver;
        solver.masters_presentation_input(time_filename, professor_filename, student_filename);

        if(argc == 6) {
            solver.intermediate_examination_input(argv[5]);
        }
        
        solver.run();
    } else if(event == "bachelor_presentation") {
        std::string time_filename = argv[2];
        std::string professor_filename = argv[3];
        std::string student_filename = argv[4];
        examiner_assignment::examiner_assignment_solver solver;
        solver.bachelor_presentation_input(time_filename, professor_filename, student_filename);
        solver.bachelor_presentation_run();
    } else {
        assert(0);
    }
}