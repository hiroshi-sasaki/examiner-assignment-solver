#pragma once

#include <utility>
#include <vector>

#include "student.h"

namespace research_project {

std::pair<std::vector<Student>, std::vector<Student>> research_project_input(
    std::string student_filename);

void research_project_output(const std::vector<std::vector<Student>> &schedule);

}  // namespace research_project