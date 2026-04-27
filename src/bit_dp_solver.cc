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
            if (!check(time_info, student, now + i)) {
                flag = false;
                break;
            }
            i++;
        }
        if (flag) return now;
        now++;
    }
    return std::numeric_limits<int>::max();
}

std::vector<Professor> bit_dp_solver(int start, int end, const Time &time_info,
                                     std::vector<Professor> professors) {
    {
        std::vector<Professor> target;
        for (auto p : professors) {
            if (p.get_students().empty()) continue;
            target.emplace_back(p);
        }
        std::swap(professors, target);
    }
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
            if (r > end) continue;
            if (r < dp[bit | (1 << i)]) {
                dp[bit | (1 << i)] = r;
                memo[bit | (1 << i)] = i;
            }
        }
    }
    if (dp.back() > end) {
        std::cerr << "\n=== SCHEDULING FAILED - DIAGNOSTIC INFORMATION ===" << std::endl;
        std::cerr << "Cannot find valid schedule. Analyzing constraints...\n" << std::endl;

        // Show which professors have students
        std::cerr << "Professors to schedule (" << n << " total):" << std::endl;
        for (int i = 0; i < n; i++) {
            std::cerr << "  [" << i << "] " << professors[i].get_name()
                      << ": " << professors[i].get_students().size() << " students" << std::endl;
        }

        // Find which states are reachable
        int reachable = 0;
        for (int bit = 0; bit < (1 << n); bit++) {
            if (dp[bit] < INF) reachable++;
        }
        std::cerr << "\nReachable DP states: " << reachable << " / " << (1 << n) << std::endl;

        // Run detailed analysis for reasonable n (complexity: O(n * 2^n))
        // For n=15: 15 * 32768 = ~500K operations (acceptable)
        if (n <= 15) {
            std::cerr << "\nChecking which professors can be placed first:" << std::endl;
            for (int i = 0; i < n; i++) {
                int r = min_left(time_info, professors[i], start);
                if (r == INF) {
                    std::cerr << "  ✗ " << professors[i].get_name()
                              << " - CANNOT be placed anywhere (check student/professor availability)" << std::endl;
                } else {
                    int end_pos = r + professors[i].get_students().size();
                    std::cerr << "  ✓ " << professors[i].get_name()
                              << " can start at position " << r << " (ends at " << end_pos << ")" << std::endl;
                }
            }

            // Check which professors are blocking progress
            std::cerr << "\nAnalyzing why final state is unreachable..." << std::endl;
            std::vector<int> never_scheduled;
            for (int i = 0; i < n; i++) {
                bool can_schedule = false;
                for (int bit = 0; bit < (1 << n); bit++) {
                    if (dp[bit] == INF) continue;
                    if ((bit >> i) & 1) continue; // already scheduled in this state

                    int r = min_left(time_info, professors[i], dp[bit]);
                    if (r != INF && r + professors[i].get_students().size() <= end) {
                        can_schedule = true;
                        break;
                    }
                }
                if (!can_schedule) {
                    never_scheduled.push_back(i);
                }
            }

            if (!never_scheduled.empty()) {
                std::cerr << "\nProfessors that CANNOT be scheduled in any valid position:" << std::endl;
                for (int i : never_scheduled) {
                    std::cerr << "  ✗ " << professors[i].get_name() << std::endl;
                }
            }
        }

        std::cerr << "\nTotal available slots: " << (end - start) << std::endl;
        int total_students = 0;
        for (int i = 0; i < n; i++) {
            total_students += professors[i].get_students().size();
        }
        std::cerr << "Total students to schedule: " << total_students << std::endl;
        std::cerr << "\n=== END DIAGNOSTIC ===" << std::endl;

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