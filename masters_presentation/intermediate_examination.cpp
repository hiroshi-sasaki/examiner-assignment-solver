#include "solver.hpp"

#include <map>
#include <bit>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#include <iostream>

namespace examiner_assignment {

    std::vector<Professor> examiner_assignment_solver::construct_intermediate_professors_plan(bool flag = false) {
        auto professors = professors_;

        if(flag) {
            for(int i = 0; i < per; i++) {
                for(auto name: schedule[2 * per + i].assign_professor) {
                    for(auto &prof: professors) {
                        if(prof.name == name) {
                            int x = std::upper_bound(accumulate.begin(), accumulate.end(), 3 * per + i) - accumulate.begin();
                            x--;
                            assert(x >= 0);
                            prof.is_possible[x] = 'x';
                        }
                    }
                }
            }
        }

        int start = 12, end = 16;

        for(auto &prof: professors) {
            prof.students.clear();
            std::string is_possible = "";
            for(int i = 0; i < intermediate_time.size(); i++) {
                if(prof.is_possible[start + i] == 'o') {
                    is_possible += std::string(intermediate_time[i], 'o');
                }
                else {
                    is_possible += std::string(intermediate_time[i], 'x');
                }
            }
            prof.is_possible = is_possible;
        }

        for(auto &student: intermediate_examiner_students) {
            for(auto &prof: professors) {
                if(prof.name == student.supervisor) {
                    student.is_possible = prof.is_possible;
                    prof.students.emplace_back(student);
                    break;
                }
            }
        }
        return professors;
    }

    std::vector<Slot> examiner_assignment_solver::intermediate_examination(std::vector<Professor> profs) const {
        /*
        std::vector<Professor> supervisors;
        for(auto professor: professors) {
            if(!professor.students.empty()) supervisors.emplace_back(professor);
        }
        int n = supervisors.size(), INF = std::numeric_limits<int>::max();
        std::vector<int> dp(1<<n, INF), memo(1<<n, -1);
        dp[0] = 0;
        int start = 0, end = professors[0].is_possible.size();
        int max = -1, index = 0;
        auto next = [&](Professor prof, int i) -> int {
            if(i == INF) return INF;
            while(i + prof.students.size() <= end) {
                bool flag = true;
                for(int j = 0; j < (int)prof.students.size(); j++) {
                    flag &= prof.is_possible[i + j];
                }
                if(flag) return i;
                i++;
            }
            return INF;
        };
        for(int bit = 0; bit < (1<<n); bit++) {
            if(dp[bit] == INF) continue;
            if(max < std::popcount(uint32_t(bit))) {
                max = std::popcount(uint32_t(bit));
                index = bit;
            }
            for(int i = 0; i < n; i++) {
                if((bit >> i) & 1) continue;
                int r = next(professors[i], dp[bit]);
                if(r == INF) continue;
                r += professors[i].students.size();
                if(r < dp[bit | (1<<i)]) {
                    dp[bit | (1<<i)] = r;
                    memo[bit | (1<<i)] = i;
                }
            }
        }

        std::map<std::string, int> assignment_count;
        std::vector<Slot> slots(end - start);
        {
            int bit = (1<<n) - 1;
            while(bit > 0) {
                int i = memo[bit];
                int count = professors[i].students.size();
                for(int j = 0; j < count; j++) {
                    slots[dp[bit] - count + j].student_number = professors[i].students[j].number;
                    slots[dp[bit] - count + j].supervisor = professors[i].name;
                }
                assignment_count[professors[i].name] += count;
                bit ^= 1<<i;
            }
        }
        return slots;
        */
        
        int n = intermediate_examiner_students.size(), INF = std::numeric_limits<int>::max();
        std::vector<std::pair<int,int>> dp(1<<n, {INF, INF});
        std::vector<int> prev(1<<n, -1);
        dp[0] = {0, 0};
        int end = intermediate_examiner_students[0].is_possible.size();
        int max = -1, index = 0;
        for(int bit = 0; bit < (1<<n); bit++) {
            if(dp[bit].first == INF) continue;
            if(max < std::popcount(uint32_t(bit))) {
                max = std::popcount(uint32_t(bit));
                index = bit;
            }
            for(int i = 0; i < n; i++) {
                if((bit >> i) & 1) continue;
                auto [r, cost] = dp[bit];
                if(prev[bit] != -1 && intermediate_examiner_students[prev[bit]].supervisor != intermediate_examiner_students[i].supervisor) {
                    cost++;
                }
                while(r < end) {
                    if(intermediate_examiner_students[i].is_possible[r] == 'o') break;
                    cost++;
                    r++;
                }
                if(r >= end) continue;
                if(std::pair<int,int>{r + 1, cost} < dp[bit | (1<<i)]) {
                    dp[bit | (1<<i)] = {r + 1, cost};
                    prev[bit | (1<<i)] = i;
                }
            }
        }

        std::map<std::string, int> assignment_count;
        std::vector<Slot> slots(end);
        {
            assert(dp.back().first != INF);
            int bit = (1<<n) - 1;
            while(bit > 0) {
                int i = prev[bit];
                slots[dp[bit].first - 1].student_number = intermediate_examiner_students[i].number;
                slots[dp[bit].first - 1].presenter = intermediate_examiner_students[i].name;
                slots[dp[bit].first - 1].supervisor = intermediate_examiner_students[i].supervisor;
                slots[dp[bit].first - 1].assign_professor = intermediate_examiner_students[i].assign_professors;
                bit ^= 1<<i;
            }
        }
        return slots;
    }

    void examiner_assignment_solver::intermediate_examination_assign(std::vector<Slot> &slots) {
        auto professors = construct_intermediate_professors_plan(true);
        auto can_assign = [&](int i, std::string name) -> bool {
            auto slot = slots[i];
            if(slot.assign_professor.size() == k || slot.presenter == "rest") return false;
            for(auto professor: slot.assign_professor) {
                if(professor == name) return false;
            }
            for(auto prof: professors) {
                if(prof.name == name) {
                    return prof.is_possible[i] == 'o';
                }
            }
            return false;
        };
        auto validator = [&]() -> bool {
            for(auto slot: slots) {
                if(slot.presenter == "rest") continue;
                if(slot.assign_professor.size() < k) return false;
            }
            return true;
        };
        while(!validator()) {
            std::vector<std::pair<int, std::string>> s;
            for(auto [name, count]: assign_count) {
                s.emplace_back(count, name);
            }
            std::sort(s.begin(), s.end());
            for(auto [count_, name]: s) {
                int now_assign = 0;
                for(int i = 0; i < slots.size(); i++) {
                    if(can_assign(i, name)) {
                        now_assign++;
                        slots[i].assign_professor.emplace_back(name);
                        assign_count[name]++;
                    }
                    if(now_assign >= 3) break;
                }
            }
        }
        return;
    }
}