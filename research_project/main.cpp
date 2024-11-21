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

#include "research_project.cpp"

int main() {
    std::string filename;
    int type;
    std::cin >> filename >> type;
    std::cerr << "Input csv file is " << filename << std::endl;
    auto result = run(filename, type);
    reserch_project_output("result.csv", result);
}