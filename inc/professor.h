#pragma once

#include <string>
#include <vector>

#include "student.h"

enum class ProfessorType : unsigned {
    professor = 0,
    associate,
    assistant,
};

struct Professor {
  private:
    std::string name = "N/A";
    std::string campus = "N/A";
    std::vector<Student> students = {};
    ProfessorType type;
    std::string is_possible = "";
    std::string affiliation = "";

  public:
    // 構想発表用
    Professor(std::string, ProfessorType, std::string, std::string);
    std::string get_name() const;
    std::string get_campus() const;
    std::vector<Student> get_students() const;
    std::vector<Student>& get_students();
    ProfessorType get_professor_type() const;
    std::string get_is_possible() const;
    std::string get_affiliation() const;

    void add_student(Student);

    bool operator==(const Professor &rhs) const {
      return name == rhs.name;
    }
};