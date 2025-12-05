#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <unordered_set>
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

struct Freq {
    int r = 0;
    int l = 0;
};

long long solve(const std::vector<int>& left, const std::vector<int>& right) {
    assert(left.size() == right.size());

    // for 1 unique no. a score addition is value * frequency in right list *
    // fequency in left list create unordered map with key as all numbers and
    // value as pair of left + right frequency
    std::unordered_map<int, Freq> freqMap;

    // iterate through lists
    const auto len = static_cast<ssize_t>(left.size());
    for (auto i = 0; i < len; i++) {
        freqMap[left[i]].l++;
        freqMap[right[i]].r++;
    }

    // iterate though map
    long long result = 0;
    for (const auto& [k, v] : freqMap) {
        result += (k * v.l * v.r);
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

    const auto&& [left, right] = read(f);
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << solve(left, right) << "\n";
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Elapsed time: " << duration << " μs\n";
    return 0;
}
