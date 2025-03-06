#include "professor.h"

#include "io_util.h"

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

void Professor::set_is_possible(std::string is_possible_) {
    is_possible = is_possible_;
}

void Professor::add_student(Student student) {
    insert_or_assign(students, student);
}
