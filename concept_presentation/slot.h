#pragma once

#include <vector>

#include "professor.h"
#include "student.h"

namespace concept_presentation {

struct Slot {
    Student presenter;
    std::vector<std::string> assign_professors;
    bool is_assigned_professor = false;
    int assistant_count = 0;

    Slot(Student, Professor, bool);

    bool can_assign(std::string, std::string) const;
    void assign(std::string, std::string);
};

}  // namespace concept_presentation