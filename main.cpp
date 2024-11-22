#include <cassert>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <string>
#include <vector>

#include "concept_presentation/concept_presentation.cpp"
#include "research_project/research_project.cpp"

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
        auto result = research_project::run(filename, type);
        research_project::research_project_output(result);
    }
    else {
        assert(0);
    }
}