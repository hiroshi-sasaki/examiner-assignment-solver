#include <string>
#include <iostream>

#include "concept_presentation.cpp"

int main() {
    std::string professor_filename, student_filename;
    std::cin >> professor_filename >> student_filename;
    run(professor_filename, student_filename);
}