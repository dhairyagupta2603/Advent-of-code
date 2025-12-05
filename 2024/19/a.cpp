#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <complex>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>
#include <unordered_set>
#include <utility>

auto read_a(std::fstream& f) {
    std::unordered_set<std::string> patterns;
    std::string pattern;
    while (std::getline(f, pattern, ',')) {
        std::istringstream iss(pattern);
        iss.ignore(1, ' ');
        iss >> pattern;
        patterns.insert(pattern);
    }

    return patterns;
}

auto read_b(std::fstream& f) {
    std::vector<std::string> designs;

    std::string design;
    while (std::getline(f, design)) {
        designs.emplace_back(design);
    }

    return designs;
}

bool check_if_buildable(const std::string s, const std::unordered_set<std::string>& primitives, const int32_t depth,
                        int32_t max_size) {
    if (s.empty()) return true;

    for (int i = max_size; i >= 1; i--) {
        if (i > static_cast<ssize_t>(s.size())) continue;

        if (primitives.find(s.substr(0, i)) != primitives.end()) {
            if (check_if_buildable(s.substr(i, s.size() - i), primitives, depth + 1, max_size)) {
                return true;
            }
        }
    }

    return false;
}

int32_t solve(std::unordered_set<std::string> patterns, const std::vector<std::string>& designs) {
    // reduce set so only primatives remain
    std::unordered_set<std::string> to_remove;
    for (const auto& p : patterns) {
        if (check_if_buildable(p, patterns, 0, p.size() - 1)) {
            to_remove.insert(p);
        }
    }

    for (const auto& p : to_remove) patterns.erase(p);

    int32_t num_designs = 0;
    for (ssize_t i = 0; i < static_cast<ssize_t>(designs.size()); i++) {
        num_designs += check_if_buildable(designs[i], patterns, 0, designs[i].size());
    }

    return num_designs;
}

int main() {
    static_assert(__cplusplus >= 201700L, "C++ version is not 17 or above");
    std::fstream fa("input1a.txt", std::ios::in), fb("input1b.txt", std::ios::in);

    if (!fa.is_open() or !fb.is_open()) {
        std::cerr << "Failed to open the files." << std::endl;
        return 1;
    }

    auto patterns = read_a(fa);
    auto designs = read_b(fb);

    auto start = std::chrono::high_resolution_clock::now();
    std::cout << solve(patterns, designs) << "\n";
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "\nElapsed time: " << duration << " ms\n";
    return 0;
}
