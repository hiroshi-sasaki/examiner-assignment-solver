#include "bit_dp_solver.h"

#include <algorithm>
#include <cassert>
#include <limits>
#include <vector>

bool check(const Time &time_info, Student student, int i) {
    int x = std::upper_bound(time_info.accumulate.begin(),
                             time_info.accumulate.end(), i) -
            time_info.accumulate.begin();
    x--;
    assert(x >= 0);
    return student.can_assign(x);
}

bool check(const Time &time_info, Professor professor, int i) {
    int x = std::upper_bound(time_info.accumulate.begin(),
                             time_info.accumulate.end(), i) -
            time_info.accumulate.begin();
    x--;
    assert(x >= 0);
    return professor.can_assign(x);
}

int min_left(const Time &time_info, Professor professor, int now) {
    int sz = professor.get_students().size();
    while (now + sz <= time_info.accumulate.back()) {
        bool flag = true;
        for (int i = 0; auto student : professor.get_students()) {
            flag &= check(time_info, student, now + i);
            i++;
        }
        if (flag) return now;
        now++;
    }
    return std::numeric_limits<int>::max();
}

std::vector<Professor> bit_dp_solver(int start, int end, const Time &time_info,
                                     std::vector<Professor> professors) {
    const int n = (int)professors.size();
    const int INF = std::numeric_limits<int>::max();
    std::vector<int> dp(1 << n, INF);
    std::vector<int> memo(1 << n, -1);
    dp[0] = start;

    for (int bit = 0; bit < (1 << n); bit++) {
        if (dp[bit] == INF) continue;
        for (int i = 0; i < n; i++) {
            if ((bit >> i) & 1) continue;
            int r = min_left(time_info, professors[i], dp[bit]);
            if (r == INF) continue;
            int c = professors[i].get_students().size();
            r += c;
            if (r < dp[bit | (1 << i)]) {
                dp[bit | (1 << i)] = r;
                memo[bit | (1 << i)] = i;
            }
        }
    }
    if (dp.back() > end) {
        assert(0);
        return {};
    }
    std::vector<int> order;
    std::vector<Professor> reorder_professors;
    int bit = (1 << n) - 1;
    while (bit > 0) {
        int i = memo[bit];
        reorder_professors.emplace_back(professors[i]);
        order.emplace_back(i);
        bit ^= 1 << i;
    }
    std::reverse(reorder_professors.begin(), reorder_professors.end());
    std::reverse(order.begin(), order.end());

    return reorder_professors;
}

std::vector<Student> construct_schedule(int start, int end,
                                        const Time &time_info,
                                        std::vector<Professor> professors) {
    if (professors.empty()) return {};
    std::vector<Student> schedule(end - start);
    int now = start;
    for (auto prof : professors) {
        now = min_left(time_info, prof, now);
        for (int i = 0; i < prof.get_students().size(); i++) {
            auto student = prof.get_students()[i];
            assert(check(time_info, student, now));
            schedule[now - start] = student;
            now++;
        }
    }
    assert(now <= end);
    return schedule;
}