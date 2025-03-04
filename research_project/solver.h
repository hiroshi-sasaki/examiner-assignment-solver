#pragma once

#include <vector>

#include "student.h"

namespace research_project {

bool validator(const std::vector<std::vector<Student>>& schedule);

std::vector<std::vector<Student>> create_schedule(std::vector<Student> students);

void run(std::string filename);

}