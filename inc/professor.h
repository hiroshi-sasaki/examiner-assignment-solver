#pragma once

#include <string>
#include <iostream>
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

    std::vector<std::string> alias;

  public:
    Professor(std::string, std::string);
    // 教員情報のファイルからの読み出し
    Professor(std::string, std::string, ProfessorType, std::string, std::vector<std::string>);
    std::string get_name() const;
    std::string get_campus() const;
    std::vector<Student> get_students() const;
    std::vector<Student>& get_students();
    ProfessorType get_professor_type() const;
    std::string get_is_possible() const;
    std::string get_affiliation() const;

    std::vector<std::string> get_alias() const;
    bool is_same_name(std::string) const;


    void set_is_possible(std::string);
    void add_student(Student);

    bool can_assign(int) const;

    bool operator==(const Professor &rhs) const {
      for(auto s: rhs.get_alias()) {
        if(is_same_name(s)) return true;
      }
      for(auto s: get_alias()) {
        if(rhs.is_same_name(s)) return true;
      }
      return false;
    }
};

std::vector<Professor> combined_professor_info(std::vector<Professor>, std::vector<Professor>);