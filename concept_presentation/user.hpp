#pragma once

#include <cassert>
#include <string>
#include <vector>

const int k = 4;

struct Student {
    std::string name = "";
    std::string supervisor = "";
    std::vector<std::string> assign_professors;
    std::string is_possible = "oo";
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
};

struct Slot {
    std::string presenter = "rest";
    std::string supervisor = "rest";
    std::vector<std::string> assign_professor;

    bool flag = false;

    int assistant_count = 0;

    Slot(std::string name, Professor professor, bool can_assign) {
        presenter = name;
        supervisor = professor.name;
        if(can_assign) {
            assign_professor.emplace_back(supervisor);
            flag = professor.type == ProfessorType::professor;
        }
    }

    bool can_assign(std::string name, std::string affiliation) {
        if (supervisor == name || affiliation == supervisor || (int)assign_professor.size() >= k) return false;
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