#include <algorithm>
#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <utility>

auto read(std::fstream& f) -> std::tuple<std::vector<std::array<int32_t, 5>>, std::vector<std::array<int32_t, 5>>> {
    std::vector<std::array<int32_t, 5>> locks, keys;

    std::string line;
    std::array<std::array<char, 5>, 7> grid;
    while (std::getline(f, line)) {
        if (!line.empty()) continue;

        for (int i = 0; i < 7; ++i) {
            for (int j = 0; j < 5; ++j) {
                f >> grid[i][j];
            }
        }

        std::array<int32_t, 5> heights;
        if (std::equal(grid[0].begin(), grid[0].end(), "#####")) {
            for (size_t j = 0; j < 5; j++) {
                size_t i;
                for (i = 0; i < 7; i++) {
                    if (grid[i][j] != '#') break;
                }
                heights[j] = i - 1;
            }
            locks.emplace_back(heights);
        } else {
            for (ssize_t j = 0; j < 5; j++) {
                ssize_t i;
                for (i = 6; i >= 0; i--) {
                    if (grid[i][j] != '#') break;
                }
                heights[j] = 5 - i;
            }
            keys.emplace_back(heights);
        }
    }
    return {locks, keys};
}

void print(const std::vector<std::array<int32_t, 5>>& vec) {
    for (const auto& v : vec) {
        for (const auto& h : v) {
            std::cout << h << " ";
        }
        std::cout << "\n";
    }
}

bool fits(const std::array<int32_t, 5>& lock, const std::array<int32_t, 5>& key) {
    const auto len = static_cast<ssize_t>(lock.size());
    for (ssize_t i = 0; i < len; i++) {
        if (lock[i] + key[i] > 5) return false;
    }

    return true;
}

int64_t solve(const std::vector<std::array<int32_t, 5>>& locks, const std::vector<std::array<int32_t, 5>>& keys) {
    std::cout << "locks: -\n";
    print(locks);
    std::cout << "keys: -\n";
    print(keys);

    int64_t result = 0;
    for (const auto& lock : locks) {
        for (const auto& key : keys) {
            if (fits(lock, key)) result++;
        }
    }
    return result;
}

int main() {
    static_assert(__cplusplus >= 201700L, "C++ version is not 17 or above");
    std::fstream f("input.txt", std::ios::in);

    if (!f.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    const auto [locks, keys] = read(f);
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << solve(locks, keys) << "\n";
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    std::cout << "Elapsed time: " << duration << " ns\n";
    return 0;
}
