#pragma once

#include <cassert>
#include <string>
#include <vector>

namespace research_project {

struct Student {
    std::string number = "N/A";
    std::string name, furigana = "N/A";
    std::string first_supervisor = "N/A";
    std::string second_supervisor = "N/A";

    bool is_first;

    Student() = default;

    Student(std::vector<std::string> line) {
        assert(line.size() == 5);
        number = line[0];
        name = line[1];
        furigana = line[2];
        first_supervisor = line[3];
        second_supervisor = line[4];
    }
};

}