#pragma once

#include <cassert>
#include <string>
#include <vector>

struct Student {
    std::string name;
    std::string supervisor;
    std::vector<std::string> assign_professors;

    std::string is_possible;
};

struct Professor {
    char campus;
    std::string name;
    std::string is_possible;

    std::vector<Student> students;
};

struct Slot {
    std::string presenter = "rest";
    std::string supervisor = "rest";
    std::vector<std::string> assign_professor;

    bool can_assign(std::string name) {
        if (supervisor == name) return false;
        for (auto s : assign_professor) {
            if (s == name) return false;
        }
        return true;
    }

    void assign(std::string name) {
        assert(can_assign(name));
        assign_professor.emplace_back(name);
    }
};