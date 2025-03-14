#include "student.h"

#include "io_util.h"

Student::Student(std::string number_, std::string name_,
                 std::string supervisor_)
    : number(number_), name(name_), supervisor(supervisor_) {}

Student::Student(std::string number_, std::string name_,
                 std::string supervisor_, std::string is_possible_)
    : number(number_),
      name(name_),
      supervisor(supervisor_),
      is_possible(is_possible_) {}

Student::Student(std::string number_, std::string name_,
                 std::string supervisor_, std::string is_possible_,
                 std::vector<std::string> assign_professors_)
    : number(number_),
      name(name_),
      supervisor(supervisor_),
      is_possible(is_possible_),
      assign_professors(assign_professors_) {}

std::string Student::get_name() const {
    return name;
}

std::string Student::get_number() const {
    return number;
}

std::string Student::get_supervisor() const {
    return supervisor;
}

std::string Student::get_is_possible() const {
    return is_possible;
}

std::vector<std::string> Student::get_assign_professors() const {
    return assign_professors;
}

int Student::assign_count() const {
    return assign_professors.size();
}

std::string Student::get_affiliation() const {
    return affiliation;
}

void Student::set_supervisor(std::string supervisor_) {
    supervisor = supervisor_;
}

void Student::set_affiliation(std::string affiliation_) {
    affiliation = affiliation_;
}

bool Student::can_assign(int i) const {
    return is_possible[i] == 'o';
}

bool Student::valid() const {
    return number != "N/A";
}

void Student::assign_professor(std::string name) {
    insert_or_assign(assign_professors, name);
}