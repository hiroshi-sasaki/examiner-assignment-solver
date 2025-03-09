#pragma once

#include <map>
#include <vector>

#include "professor.h"
#include "student.h"
#include "time.h"

namespace master_presentation {

struct master_presentation_solver {
  private:
    void professor_input(std::string, std::string);
    void student_input(std::string);
    void input(std::string, std::string, std::string, std::string);

  public:
    master_presentation_solver(std::string, std::string, std::string,
                               std::string);

    void run();

  private:
    int examiner_count = 4;

    Time time_info;
    std::vector<Professor> professors_;
    std::vector<Student> students_;

    std::map<std::string, int> assign_count;
};

}  // namespace master_presentation