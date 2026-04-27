#pragma once

#include <vector>

#include "professor.h"
#include "student.h"
#include "time.h"

bool check(const Time &time_info, Professor professor, int i);

std::vector<Professor> bit_dp_solver(int start, int end, const Time &time_info,
                                     std::vector<Professor> professors);

std::vector<Student> construct_schedule(int start, int end, const Time &time_info, std::vector<Professor> professors);