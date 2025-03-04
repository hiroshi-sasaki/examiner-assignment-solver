#pragma once

#include <string>

struct Student {
  private:
    std::string number = "N/A";
    std::string name = "N/A";
    std::string supervisor = "N/A";
  public:
    std::string get_number() const;
    std::string get_name() const;
    std::string get_supervisor() const;

    Student(std::string, std::string, std::string);

    bool operator==(const Student &rhs) const {
      return name == rhs.name && number == rhs.number;
    }
};