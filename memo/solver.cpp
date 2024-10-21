#include <cassert>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <string>
#include <vector>

#include "user.hpp"

struct examiner_assignment_solver {
    struct Block {
        char campus;
        std::string supervisor;
        std::vector<Student> students;

        std::string is_assign;
        std::vector<std::vector<int>> orders;

        Block(Professor professor)
            : campus(professor.campus),
              supervisor(professor.name),
              students(professor.students) {
            assert(!students.empty());
            int l = (int)students[0].is_possible.size();
            is_assign = std::string(l, 'x');
            orders.resize(l);
            const int sz = (int)students.size();
            for (int i = 0; i < l - sz + 1; i++) {
                std::vector<int> indexes(sz);
                std::iota(indexes.begin(), indexes.end(), 0);
                do {
                    bool is_ok = true;
                    for (int j = 0; j < sz; j++) {
                        is_ok &= students[indexes[j]].is_possible[i + j] == 'o';
                    }
                    if (is_ok) {
                        orders[i] = indexes;
                        is_assign[i] = 'o';
                        break;
                    }
                } while (std::next_permutation(indexes.begin(), indexes.end()));
            }
        }

        bool check(int i) const {
            return is_assign[i] == 'o';
        }

        int min_left(int i) const {
            if((int)is_assign.size() <= i) return -1;
            for (int j = i; j < (int)is_assign.size(); j++) {
                if (is_assign[j] == 'o') return j;
            }
            return -1;
        }
    };

    examiner_assignment_solver(std::string filename) {
        std::ifstream file(filename, std::ios::in);
        if (!file) {
            std::cerr << "cannot open file: " << filename << std::endl;
        }
        int n, m, k;
        file >> n >> m >> l >> k;
        professors.resize(n);
        std::map<std::string, std::string> table;
        for (auto &prof : professors) {
            file >> prof.name >> prof.campus >> prof.is_possible;
            table[prof.name] = prof.is_possible;
        }
        for (int i = 0; i < m; i++) {
            std::string name, supervisor;
            std::vector<std::string> assign_professors(k);
            file >> name >> supervisor;
            assert(table.contains(supervisor));
            std::string is_possible = table[supervisor];
            for (auto &prof : assign_professors) {
                file >> prof;
                assert(table.contains(prof));
                auto s = table[prof];
                for (int j = 0; j < l; j++) {
                    if (s[j] == 'x') is_possible[j] = 'x';
                }
            }
            for (auto &prof : professors) {
                if (prof.name == supervisor) {
                    prof.students.emplace_back(name, supervisor,
                                               assign_professors, is_possible);
                }
            }
        }
    }

    std::vector<Slot> calc_plan(int start, int end,
                                const std::vector<Block> &blocks,
                                std::vector<int> order) const {
        int w = end - start;
        int now = 0;
        int cont = 0;
        bool is_ok = true;
        std::vector<Slot> slots(w);
        for (auto i : order) {
            if (cont >= section) {
                cont = 0;
                now++;
            }
            int left = blocks[i].min_left(start + now);
            if (left != -1) {
                if (now + start != left) {
                    cont = 0;
                }
                if(end < left + (int)blocks[i].students.size()) {
                    is_ok = false;
                    break;
                }
                now = left - start;
                for (int j = 0; j < (int)blocks[i].students.size(); j++) {
                    int id = blocks[i].orders[left][j];
                    auto student = blocks[i].students[id];
                    slots[now] = {student.name, student.supervisor,
                                              student.assign_professors};
                                              now++;
                    cont++;
                }
            } else {
                is_ok = false;
                break;
            }
        }
        if (is_ok) return slots;
        return {};
    }

    std::vector<std::vector<Slot>> brute_force(
        int start, int end, const std::vector<Block> &blocks) const {
        const int sz = (int)blocks.size();
        assert(sz <= 11);
        std::vector<int> index(sz);
        std::iota(index.begin(), index.end(), 0);
        std::vector<std::vector<Slot>> practicable;
        do {
            auto plan = calc_plan(start, end, blocks, index);
            if (!plan.empty()) {
                practicable.emplace_back(plan);
            }
        } while (std::next_permutation(index.begin(), index.end()));
        return practicable;
    }

    std::vector<Slot> bit_dp_solution(int start, int end,
                                      std::vector<Block> blocks) const {
        const int sz = (int)blocks.size();
        std::vector dp(1 << sz, std::vector<int>(section, INF));
        std::vector memo(1 << sz, std::vector<std::pair<int,int>>(section, {-1, -1}));
        dp[0][0] = start;
        for (int bit = 0; bit < (1 << sz); bit++) {
            for (int i = 0; i < section; i++) {
                if (dp[bit][i] == INF) continue;
                int time = dp[bit][i];
                for (int j = 0; j < sz; j++) {
                    if ((bit >> j) & 1) continue;
                    int nxt = blocks[j].min_left(time);
                    if (nxt == -1) continue;
                    int cont = i;
                    if(time != nxt) {
                        cont = 0;
                    }
                    cont += (int)blocks[j].students.size();
                    nxt += (int)blocks[j].students.size();
                    if(cont >= section) {
                        cont = 0;
                        nxt++;
                    }
                    if(nxt < dp[bit | (1<<j)][cont]) {
                        dp[bit | (1<<j)][cont] = nxt;
                        memo[bit | (1<<j)][cont] = {j, i};
                    }
                }
            }
        }
        int bit = (1<<sz) - 1, idx = -1, now = INF;
        for(int i = 0; i < section; i++) {
            if(dp[bit][i] < now) {
                now = dp[bit][i];
                idx = i;
            }
        }
        if(now == INF) {
            std::cout << "not found" << std::endl;
            assert(0);
        }
        std::vector<int> order;
        while(bit > 0) {
            auto [i, j] = memo[bit][idx];
            order.emplace_back(i);
            bit ^= 1<<i;
            idx = j;
        }
        std::reverse(order.begin(), order.end());
        return calc_plan(start, end, blocks, order);
    }

    std::vector<Slot> assignment_at_presentation(int partition_count) const {
        std::vector<Block> oookayama, suzukake;
        int sum = 0;
        for (auto professor : professors) {
            if (professor.students.empty()) continue;
            sum += (int)professor.students.size();
            if (professor.campus == 'o') {
                oookayama.emplace_back(professor);
            } else if (professor.campus == 's') {
                suzukake.emplace_back(professor);
            } else {
                assert(0);
            }
        }
        std::vector<Slot> result;
        if(partition_count == 2) {
            auto day1 = bit_dp_solution(0, l/2, oookayama);
            auto day2 = bit_dp_solution(l/2, l, suzukake);
            result.insert(result.end(), day1.begin(), day1.end());
            result.insert(result.end(), day2.begin(), day2.end());
        }
        else {
            std::vector groups(partition_count, std::vector<Block>());
            std::vector<int> counter(partition_count, 0);
            int now = 0;
            for(auto block: oookayama) {
                if(counter[now] > (sum + partition_count - 1) / partition_count) {
                    now++;
                }
                assert(now < partition_count);
                groups[now].emplace_back(block);
                counter[now] += (int)block.students.size();
            }
            // グループ間の入れ替えをしていってより良いものにする
            while(true) {

            }
        }
        return result;
    }

    /*
    std::vector<Slot> assignment_at_examinor() {

    }
    */

  private:
    int l;
    std::vector<Professor> professors;
    const int INF = std::numeric_limits<int>::max();
    const int section = 5;
};
