
#include "professor.h"

#include <algorithm>
#include <cassert>

#include "io_util.h"

Professor::Professor(std::string name_, std::string is_possible_)
    : name(name_), is_possible(is_possible_) {}

Professor::Professor(std::string name_, ProfessorType type_,
                     std::string is_possible_, std::string affiliation_)
    : name(name_),
      type(type_),
      is_possible(is_possible_),
      affiliation(affiliation_) {}

Professor::Professor(std::string name_, std::string campus_,
                     ProfessorType type_, std::string affiliation_)
    : name(name_), campus(campus_), type(type_), affiliation(affiliation_) {}

std::string Professor::get_campus() const {
    return campus;
}

std::string Professor::get_name() const {
    return name;
}

std::vector<Student> Professor::get_students() const {
    return students;
}

std::vector<Student>& Professor::get_students() {
    return students;
}

ProfessorType Professor::get_professor_type() const {
    return type;
}

std::string Professor::get_is_possible() const {
    return is_possible;
}

std::string Professor::get_affiliation() const {
    return affiliation;
}

bool Professor::can_assign(int i) const {
    return is_possible[i] == 'o';
}

void Professor::set_is_possible(std::string is_possible_) {
    is_possible = is_possible_;
}

void Professor::add_student(Student student) {
    student.set_affiliation(affiliation);
    insert_or_assign(students, student);
}

std::vector<Professor> combined_professor_info(
    std::vector<Professor> professor_base_info,
    std::vector<Professor> professors) {
    std::vector<Professor> combined;
    for (auto p : professors) {
        auto itr =
            std::find_if(professor_base_info.begin(), professor_base_info.end(),
                         [&p](Professor professor) {
                             return professor.get_name() == p.get_name();
                         });
        assert(itr != professor_base_info.end());
        itr->set_is_possible(p.get_is_possible());
        combined.emplace_back(*itr);
    }
    return combined;
}