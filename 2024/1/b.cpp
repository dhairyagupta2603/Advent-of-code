#include <algorithm>
#include <cassert>
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

long long solve(const std::vector<int>& left, const std::vector<int>& right) {
    assert(left.size() == right.size());

    struct Freq {
        int r = 0;
        int l = 0;
    };

    // for 1 unique no. a score addition is value * frequency in right list *
    // fequency in left list create unordered map with key as all numbers and
    // value as pair of left + right frequency
    std::unordered_map<int, Freq> freqMap;
    // iterate through left list
    for (const auto& num : left) {
        freqMap[num].l++;
    }

    // iterate though right list
    for (const auto& num : right) {
        freqMap[num].r++;
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
    std::fstream f("1/input.txt", std::ios::in);

    if (!f.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    const auto& [left, right] = read(f);
    std::cout << solve(left, right) << "\n";
    return 0;
}
