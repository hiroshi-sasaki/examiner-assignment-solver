#pragma once

#include <algorithm>
#include <cassert>
#include <limits>
#include <map>
#include <random>
#include <set>
#include <vector>

#include "io.cpp"
#include "user.hpp"

namespace research_project {

bool validator(std::vector<std::vector<Student>> result) {
    int n = (int)result.size();
    int m = result.back().size();
    for (int i = 0; i < n; i++) {
        if (result[i].size() != result[0].size()) return false;
    }
    bool is_first = result[0][0].is_first;
    for (int i = 0; i < m; i++) {
        for (int a = 0; a < n; a++) {
            if (result[a][i].number == "N/A") continue;
            for (int b = a + 1; b < n; b++) {
                if (is_first) {
                    if (result[a][i].first_supervisor ==
                        result[b][i].first_supervisor)
                        return false;
                } else {
                    if (result[a][i].second_supervisor ==
                        result[b][i].second_supervisor)
                        return false;
                }
            }
        }
    }
    return true;
}

std::vector<std::vector<Student>> research_project(
    std::vector<Student> students, bool is_first) {
    std::cerr << "[Start scheduling]" << std::endl;

    const int parallel = 8;
    std::vector<std::pair<std::string, std::vector<Student>>> remain;
    int sum = 0;
    {
        std::map<std::string, std::vector<Student>> map;
        for (auto student : students) {
            std::string supervisor;
            student.is_first = is_first;
            if (is_first)
                supervisor = student.first_supervisor;
            else
                supervisor = student.second_supervisor;
            map[supervisor].emplace_back(student);
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
                if ((is_first ? s.first_supervisor : s.second_supervisor) ==
                    name)
                    return false;
            }
            for(int k = 0; k < parallel; k++) {
                if((is_first ? result[k][y].first_supervisor : result[k][y].second_supervisor) == name) {
                    return false;
                }
            }
            return true;
        };
        for (int i = 0; i < parallel; i++) {
            if(result[i][j].number != "N/A") continue;
            for (auto &s: remain) {
                if(s.second.empty()) continue;
                if (check(i, j, s.first)) {
                    result[i][j] = s.second.back();
                    s.second.pop_back();
                    if (!s.second.empty() && j + 1 < slot) {
                        result[i][j+1] = s.second.back();
                        s.second.pop_back();
                    }
                    break;
                }
            }
        }
    }
    assert(validator(result));
    std::cerr << "[Finish scheduling]" << std::endl;
    return result;
}

std::vector<std::vector<Student>> run(std::string filename, int type) {
    assert(type == 1 || type == 2);
    auto students = research_project_input(filename);
    return research_project(students, type == 1);
}

}