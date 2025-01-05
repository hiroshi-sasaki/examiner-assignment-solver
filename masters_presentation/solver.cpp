#include <bit>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <algorithm>

#include "solver.hpp"

namespace examiner_assignment
{

    template <class T>
    void shuffle_vector(std::vector<T> &v)
    {
        std::random_device seed_gen;
        std::mt19937 engine(seed_gen());
        std::shuffle(v.begin(), v.end(), engine);
    }

    examiner_assignment_solver::examiner_assignment_solver(std::string professor_filename, std::string student_filename)
    {
        accumulate.resize(day * section + 1);
        accumulate[0] = 0;
        for (int i = 0; i < section; i++)
        {
            per += time[i];
        }
        for (int i = 0; i < day; i++)
        {
            for (int j = 0; j < section; j++)
            {
                accumulate[i * section + j + 1] = accumulate[i * section + j] + time[j];
            }
        }
        input(professor_filename, student_filename);
    }

    int examiner_assignment_solver::min_left(Professor prof, int start) const
    {
        int sz = prof.students.size();
        while (start + sz <= accumulate.back())
        {
            bool flag = true;
            for (int i = 0; i < sz; i++)
            {
                int x = std::upper_bound(accumulate.begin(), accumulate.end(), start + i) - accumulate.begin();
                x--;
                assert(0 <= x);
                flag &= prof.students[i].is_possible[x] == 'o';
            }
            if (flag)
                return start;
            start++;
        }
        return std::numeric_limits<int>::max();
    }

    bool examiner_assignment_solver::check(std::string is_possible, int i) const
    {
        int x = std::upper_bound(accumulate.begin(), accumulate.end(), i) - accumulate.begin();
        x--;
        return is_possible[x] == 'o';
    }

    /*
    std::vector<Student> examiner_assignment_solver::bit_dp_solution(int start, int end, std::vector<Student> students) const
    {
        const int n = (int)students.size();
        const int INF = std::numeric_limits<int>::max();
        std::vector<int> dp(1 << n, INF);
        std::vector<int> prev(1 << n, -1);
        dp[0] = 0;
        for (int bit = 0; bit < (1 << n) - 1; bit++)
        {
            if (dp[bit] == INF)
                continue;
            int t = std::popcount(uint32_t(bit));
            for (int i = 0; i < n; i++)
            {
                if ((bit >> i) & 1)
                    continue;
                if (!check(students[i].is_possible, start + t))
                    continue;
                if (dp[bit] + (students[i].supervisor != students[prev[bit] == -1 ? i : prev[bit]].supervisor) < dp[bit | (1 << i)])
                {
                    dp[bit | (1 << i)] = dp[bit] + (students[i].supervisor != students[prev[bit] == -1 ? i : prev[bit]].supervisor);
                    prev[bit | (1 << i)] = i;
                }
            }
        }
        int bit = (1 << n) - 1;
        return {};
    }
    */

    std::vector<Professor> examiner_assignment_solver::bit_dp_solution(int start, int end, std::vector<Professor> professors) const
    {
        const int n = (int)professors.size();
        const int INF = std::numeric_limits<int>::max();
        std::vector<int> dp(1 << n, INF);
        std::vector<int> memo(1 << n, -1);
        dp[0] = start;

        int max = -1;

        for (int bit = 0; bit < (1 << n); bit++)
        {
            if (dp[bit] == INF)
                continue;
            for (int i = 0; i < n; i++)
            {
                if ((bit >> i) & 1)
                    continue;
                int r = min_left(professors[i], dp[bit]);
                if (r == INF)
                    continue;
                if (r / per != (r + (int)professors[i].students.size() - 1) / per)
                {
                    r = (r / per + 1) * per;
                }
                r += (int)professors[i].students.size();
                if (r < dp[bit | (1 << i)])
                {
                    dp[bit | (1 << i)] = r;
                    memo[bit | (1 << i)] = i;
                }
            }
        }
        if (dp.back() == INF)
        {
            return {};
        }
        std::vector<int> order;
        std::vector<Professor> reorder_professors;
        int bit = (1 << n) - 1;
        while (bit > 0)
        {
            int i = memo[bit];
            reorder_professors.emplace_back(professors[i]);
            order.emplace_back(i);
            bit ^= 1 << i;
        }
        std::reverse(reorder_professors.begin(), reorder_professors.end());
        std::reverse(order.begin(), order.end());

        return reorder_professors;
    }

    std::vector<Slot> examiner_assignment_solver::construct_schedule(int start, int end, std::vector<Professor> professors) const
    {
        if (professors.empty())
            return {};
        std::vector<Slot> schedule(end - start);
        int now = start;
        for (auto prof : professors)
        {
            now = min_left(prof, now);
            for (int i = 0; i < prof.students.size(); i++)
            {
                auto student = prof.students[i];
                assert(check(student.is_possible, now));
                schedule[now - start] = Slot{student.number, student.name, student.supervisor, student.assign_professors};
                now++;
            }
        }
        assert(now <= end);
        return schedule;
    }

    void examiner_assignment_solver::output_slot(Slot slot) const
    {
        std::cout << slot.student_number;
        for (auto prof : slot.assign_professor)
        {
            std::cout << ',' << prof;
        }
    }
    void examiner_assignment_solver::output_time(int h, int m) const
    {
        std::cout << h << ":" << m << "0";
    }

    void examiner_assignment_solver::output(std::vector<Slot> schedule) const
    {
        int i = 0;
        while (i * per < (int)schedule.size())
        {
            std::cout << "day" << i + 1 << std::endl;
            std::cout << "時間,学籍番号,主審査員,副審査1,副審査2,副審査3,副審査4" << std::endl;

            int h = 9, m = 0;
            for (int j = 0; j < per; j++)
            {
                if (j > 0 && j % 4 == 0 && j != per - 1)
                {
                    output_time(h, m);
                    std::cout << "休憩" << std::endl;
                    m++;
                    while (m >= 6)
                    {
                        m -= 6;
                        h++;
                    }

                    if (h == 16 && m == 3)
                        m++;
                }

                output_time(h, m);
                std::cout << ',';
                output_slot(schedule[i * per + j]);
                std::cout << std::endl;
                m += 2;
                while (m >= 6)
                {
                    m -= 6;
                    h++;
                }
            }
            i++;
        }
    }

    void examiner_assignment_solver::run()
    {
        for (int i = 0; i < 1; i++)
        {
            std::cout << "--plan" << i + 1 << "---" << std::endl;
            auto suzukake_sol = bit_dp_solution(0, per * 3, suzukake);
            auto suzukake_schedule = construct_schedule(0, per * 3, suzukake_sol);
            auto oookayama_sol = bit_dp_solution(per * 4, per * 5, oookayama);
            auto oookayama_schedule = construct_schedule(per * 4, per * 5, oookayama_sol);

            auto schedule = suzukake_schedule;
            schedule.insert(schedule.end(), oookayama_schedule.begin(), oookayama_schedule.end());

            /*
            std::vector<Student> s;
            for (auto prof : oookayama)
            {
                s.insert(s.end(), prof.students.begin(), prof.students.end());
            }
            bit_dp_solution(per * 4, per * 5, s);
            */

            if (!oookayama_schedule.empty() && !suzukake_schedule.empty())
            {
                output(schedule);
            }

            shuffle_vector(suzukake);
            shuffle_vector(oookayama);
        }
    }
}