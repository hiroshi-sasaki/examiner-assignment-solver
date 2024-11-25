#pragma once

#include <cassert>
#include <string>
#include <vector>

namespace concept_presentation {

int k = 4;

struct Student {
    std::string number = "";
    std::string name = "";
    std::string supervisor = "";
    std::vector<std::string> assign_professors;
    std::string is_possible = "oo";

    bool operator==(const Student &rhs) const {
        return name == rhs.name;
    }
};

enum class ProfessorType : unsigned {
    professor = 0,
    associate,
    assistant,
};

struct Professor {
    char campus;
    std::string name;
    std::string is_possible;
    std::string affiliation;

    ProfessorType type;

    bool student_assign = false;
    std::vector<Student> students;

    bool operator==(const Professor &rhs) const {
        return name == rhs.name;
    }
};

struct Slot {
    Student presenter;
    std::string supervisor = "rest";
    std::vector<std::string> assign_professor;

    bool flag = false;

    int assistant_count = 0;

    Slot(Student s, Professor professor, bool can_assign_prof) {
        presenter = s;
        supervisor = professor.name;
        if(can_assign(professor.name, professor.affiliation) && can_assign_prof) {
            assign_professor.emplace_back(supervisor);
            flag = professor.type == ProfessorType::professor;
        }
    }

    bool can_assign(std::string name, std::string affiliation) {
        if ((k == 3 && supervisor == name) || affiliation == supervisor || (int)assign_professor.size() >= k) return false;
        for (auto s : assign_professor) {
            if (s == name) return false;
        }
        return true;
    }

    void assign(std::string name, std::string affiliation) {
        assert(can_assign(name, affiliation));
        assign_professor.emplace_back(name);
    }
};

}