#pragma once

#include <vector>

#include "io_util.h"
#include "professor.h"
#include "slot.h"
#include "student.h"

namespace concept_presentation {

std::vector<Professor> concept_presentation_input(std::string, std::string, std::string, std::string);
void concept_presentation_output(std::vector<std::vector<Slot>>);

}