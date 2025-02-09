#pragma once

#include <cassert>
#include <string>
#include <vector>

namespace examiner_assignment {

struct Student {
    std::string number;
    std::string name;
    std::string supervisor;
    std::vector<std::string> assign_professors;

    std::string is_possible;

    bool operator==(const Student &rhs) const {
      return number == rhs.number && name == rhs.name && supervisor == rhs.supervisor;
    }
};

struct Professor {
    std::string name;
    std::string campus;
    std::string is_possible;

    std::vector<Student> students;

    bool operator==(const Professor &rhs) const {
      return name == rhs.name;
    }
};

struct Slot {
    std::string student_number = "rest";
    std::string presenter = "rest";
    std::string supervisor = "rest";
    std::vector<std::string> assign_professor;
    std::string is_possible;
    int m = 0;
};

}  // namespace examiner_assignment