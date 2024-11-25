#pragma once

#include <cassert>
#include <string>
#include <vector>

namespace research_project {

struct Student {
    std::string number = "N/A";
    std::string name;
    std::string first_supervisor = "N/A";
    std::string second_supervisor = "N/A";

    bool is_first;

    Student() = default;

    Student(std::string number_, std::string name_, std::string first,
            std::string second)
        : number(number_),
          name(name_),
          first_supervisor(first),
          second_supervisor(second) {}

    bool operator==(const Student &rhs) const {
      return name == rhs.name && number == rhs.number;
    }
};

}  // namespace research_project