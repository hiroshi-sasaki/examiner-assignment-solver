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

#include "research_project/research_project.cpp"
#include "concept_presentation/concept_presentation.cpp"

int main(int argc, char *argv[]) {
    assert(argc == 3);
    std::string filename = argv[1];
    int type = std::stoi(argv[2]);
    auto result = research_project::run(filename, type);
    research_project::reserch_project_output(result);
}