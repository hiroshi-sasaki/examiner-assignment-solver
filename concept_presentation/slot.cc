#include "slot.h"

#include <cassert>

#include <iostream>
namespace concept_presentation {

extern int k;

Slot::Slot(Student presenter_, Professor professor, bool can_assign_prof) : presenter(presenter_) {
    if(can_assign(professor.get_name(), professor.get_affiliation()) && can_assign_prof) {
        assign(professor.get_name(), professor.get_affiliation());
        is_assigned_professor = professor.get_professor_type() == ProfessorType::professor;
    }
}

bool Slot::can_assign(std::string name, std::string affiliation) const {
    if ((k == 3 && presenter.get_supervisor() == name) ||
        (name != affiliation && affiliation == presenter.get_affiliation()) ||
        (int)assign_professors.size() >= k)
        return false;
    for(auto s: assign_professors) {
        if(s == name) return false;
    }
    return true;
}

void Slot::assign(std::string name, std::string affiliation) {
    assert(can_assign(name, affiliation));
    assign_professors.emplace_back(name);
}

}  // namespace concept_presentation