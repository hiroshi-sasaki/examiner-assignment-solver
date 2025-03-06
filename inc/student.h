#pragma once

#include <string>
#include <vector>

struct Student {
  private:
    std::string number = "N/A";
    std::string name = "N/A";
    std::string supervisor = "N/A";
    std::string is_possible = "";

    std::vector<std::string> assignment_professor;

  public:
    std::string get_number() const;
    std::string get_name() const;
    std::string get_supervisor() const;
    std::string get_is_possible() const;

    Student(std::string, std::string, std::string);
    Student(std::string, std::string, std::string, std::string);

    Student() = default;

    bool operator==(const Student &rhs) const {
        return name == rhs.name && number == rhs.number;
    }
};