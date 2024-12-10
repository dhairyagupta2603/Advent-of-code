#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <execution>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

std::pair<std::vector<int>, std::vector<int>> read(std::fstream& file) {
    std::pair<std::vector<int>, std::vector<int>> lists;
    int v1 = -1, v2 = -1;
    while (file >> v1 >> v2) {
        lists.first.emplace_back(v1);
        lists.second.emplace_back(v2);
    }

    return lists;
}

long long solve(std::vector<int>& a, std::vector<int>& b) {
    assert((a.size() == b.size()) and "a and b are not same size");

    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());

    const auto len = static_cast<ssize_t>(a.size());
    long long result = 0;
    for (auto i = 0; i < len; i++) {
        result += static_cast<long long>(std::abs(a[i] - b[i]));
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

    auto&& [a, b] = read(f);
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << solve(a, b) << "\n";
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Elapsed time: " << duration << " μs\n";
    return 0;
}
