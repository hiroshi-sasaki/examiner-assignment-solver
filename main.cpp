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

#include "concept_presentation.cpp"
#include "io.cpp"
#include "user.hpp"



int main() {
    auto professors = concept_presentation_input("professor.txt", "student.txt");
    auto result = concept_presentation_assignment_solver(professors, 4);
    for(auto slots: result) {
        std::cout << "---" << std::endl;
        for(auto slot: slots) {
            std::cout << slot.presenter << " " << slot.assign_professor << std::endl;
        }
    }
}