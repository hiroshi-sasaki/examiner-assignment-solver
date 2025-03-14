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
    void professor_input(std::string);
    void student_input(std::string);
    void input(std::string, std::string, std::string);
    void output(std::vector<Student>) const;

    void professor_assign(std::vector<Student> &suzukake_schedule, std::vector<Student> &oookayama_schedule);

  public:
    bachelor_presentation_solver(std::string, std::string,
                                 std::string);

    void run();

  private:
    const int k = 3;
    Time time_info;
    std::vector<Professor> professors_;
    std::vector<Student> students_;

    std::map<std::string, int> assign_count;
};

}  // namespace bachelor_presentation