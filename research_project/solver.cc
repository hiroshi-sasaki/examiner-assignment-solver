#include "solver.h"

#include <algorithm>
#include <cassert>
#include <limits>
#include <map>
#include <random>
#include <set>
#include <vector>

#include "io.cc"

namespace research_project {

// スケジュールがvalidか判定
bool validator(const std::vector<std::vector<Student>> &schedule) {
    int n = (int)schedule.size();
    int m = schedule.back().size();
    for (int i = 0; i < n; i++) {
        if (schedule[i].size() != schedule[0].size()) return false;
    }
    for (int i = 0; i < m; i++) {
        for (int a = 0; a < n; a++) {
            if (schedule[a][i].get_number() == "N/A") continue;
            for (int b = a + 1; b < n; b++) {
                if (schedule[a][i].get_supervisor() !=
                    schedule[b][i].get_supervisor()) {
                    return false;
                }
            }
        }
    }
    return true;
}

std::vector<std::vector<Student>> create_schedule(
    std::vector<Student> students) {
    const int parallel = 8;
    std::vector<std::pair<std::string, std::vector<Student>>> remain;
    int sum = 0;
    {
        std::map<std::string, std::vector<Student>> map;
        for (auto student : students) {
            map[student.get_supervisor()].emplace_back(student);
        }
        for (auto p : map) {
            std::reverse(p.second.begin(), p.second.end());
            remain.emplace_back(p);
            sum += (int)p.second.size();
        }
    }
    int slot = (sum + parallel - 1) / parallel;
    std::vector result(parallel, std::vector<Student>(slot));
    for (int j = 0; j < slot; j++) {
        std::sort(remain.begin(), remain.end(), [](auto lhs, auto rhs) -> bool {
            return lhs.second.size() > rhs.second.size();
        });
        auto check = [&](int x, int y, std::string name) -> bool {
            for (auto s : result[x]) {
                if (s.get_supervisor() == name) return false;
            }
            for (int k = 0; k < parallel; k++) {
                if (result[k][y].get_supervisor() == name) {
                    return false;
                }
            }
            return true;
        };
        for (int i = 0; i < parallel; i++) {
            if (result[i][j].get_number() != "N/A") continue;
            for (auto &s : remain) {
                if (s.second.empty()) continue;
                if (check(i, j, s.first)) {
                    result[i][j] = s.second.back();
                    s.second.pop_back();
                    // 2人までは連続させて良い
                    if (!s.second.empty() && j + 1 < slot) {
                        result[i][j + 1] = s.second.back();
                        s.second.pop_back();
                    }
                    break;
                }
            }
        }
    }
    assert(validator(result));
    return result;
}

void run(std::string filename) {
    auto [first, second] = research_project_input(filename);
    research_project_output(create_schedule(first));
    research_project_output(create_schedule(second));
}

}  // namespace research_project