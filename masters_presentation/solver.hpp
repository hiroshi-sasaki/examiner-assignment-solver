#pragma once

#include <string>
#include <vector>

#include "user.hpp"

namespace examiner_assignment
{

    struct examiner_assignment_solver
    {
    private:
        void input(std::string, std::string);

        void output_slot(Slot) const;

        void output_time(int, int) const;

        int min_left(Professor, int) const;

        bool check(std::string, int) const;

        // std::vector<Student> bit_dp_solution(int start, int end, std::vector<Student> students) const;
        std::vector<Professor> bit_dp_solution(int, int, std::vector<Professor>) const;

    public:
        examiner_assignment_solver(std::string, std::string);

        std::vector<Slot> construct_schedule(int start, int end, std::vector<Professor>) const;

        void output(std::vector<Slot>) const;

        void run();

    private:
        std::vector<std::string> time_window_label;
        const int day = 5, section = 4;
        int per = 0;
        std::vector<int> time = {8, 4, 9, 3};
        std::vector<int> accumulate;
        std::vector<Professor> oookayama, suzukake;
    };

}