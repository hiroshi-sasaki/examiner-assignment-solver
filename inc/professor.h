#pragma once

#include "student.h"

#include <string>
#include <vector>


struct Professor {
private:
    std::string name = "N/A";
    std::string campus = "N/A";
    std::vector<Student> students = {};
    std::string is_possible = "";
public:
};