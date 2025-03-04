#include "student.h"

Student::Student(std::string number_, std::string name_,
                 std::string supervisor_)
    : number(number_), name(name_), supervisor(supervisor_) {}

std::string Student::get_name() const {
    return name;
}

std::string Student::get_number() const {
    return number;
}

std::string Student::get_supervisor() const {
    return supervisor;
}