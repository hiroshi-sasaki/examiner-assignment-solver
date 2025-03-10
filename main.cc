#include <cassert>

#include "concept_presentation/solver.h"
#include "research_project/solver.h"

int main(int argc, char *argv[]) {
    assert(argc > 1);
    std::string event = argv[1];
    if(event == "concept_presentation") {
        std::string professor_filename = argv[2];
        std::string student_filename = argv[3];
        int k = std::stoi(argv[4]);
        concept_presentation::run(professor_filename, student_filename, k);
    }
    else if(event == "research_project") {
        std::string student_filename = argv[2];
        int parallel = std::stoi(argv[3]);
        research_project::run(student_filename, parallel);
    }
}