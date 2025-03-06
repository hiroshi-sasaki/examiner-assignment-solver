#pragma once

#include <map>
#include <string>
#include <vector>

#include "professor.h"
#include "student.h"
#include "time.h"

namespace bachelor_presentation {

struct bachelor_presentation_solver {
  private:
    void professor_input(std::string, std::string);
    void student_input(std::string);
    void input(std::string, std::string, std::string, std::string);

  public:
    bachelor_presentation_solver(std::string, std::string, std::string,
                                 std::string);

    void run();

  private:
    Time time_info;
    std::vector<Professor> professors_;
    std::vector<Student> students_;

    std::map<std::string, int> assign_count;
};

}  // namespace bachelor_presentation