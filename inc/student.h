#pragma once

#include <string>
#include <vector>

struct Student {
  private:
    std::string number = "N/A";
    std::string name = "N/A";
    std::string supervisor = "N/A";
    std::string is_possible = "";

    std::vector<std::string> assign_professors;

    std::string affiliation = "";

  public:
    std::string get_number() const;
    std::string get_name() const;
    std::string get_supervisor() const;
    std::string get_is_possible() const;
    std::vector<std::string> get_assign_professors() const;
    int assign_count() const;
    std::string get_affiliation() const;

    void set_affiliation(std::string);

    bool can_assign(int) const;
    bool valid() const;

    void assign_professor(std::string name);

    Student(std::string, std::string, std::string);
    Student(std::string, std::string, std::string, std::string);
    Student(std::string, std::string, std::string, std::string, std::vector<std::string>);

    Student() = default;

    bool operator==(const Student &rhs) const {
        return name == rhs.name && number == rhs.number;
    }
};