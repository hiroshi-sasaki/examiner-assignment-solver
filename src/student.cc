#include "student.h"

Student::Student(std::string number_, std::string name_,
                 std::string supervisor_)
    : number(number_), name(name_), supervisor(supervisor_) {}

Student::Student(std::string number_, std::string name_,
                 std::string supervisor_, std::string is_possible_)
    : number(number_),
      name(name_),
      supervisor(supervisor_),
      is_possible(is_possible_) {}

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