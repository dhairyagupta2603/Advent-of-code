#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>

auto read(std::fstream& f) {
    std::list<long long> stones;
    long long num = -1;
    while (f >> num) {
        stones.emplace_back(num);
    }

    return stones;
}

long solve(std::list<long long>&& stones) {
    const auto split = [](const long long num) -> std::pair<long long, long long> {
        const auto str = std::to_string(num);
        const auto half = str.begin() + str.size() / 2;

        const std::string str1(str.begin(), half);
        const std::string str2(half, str.end());
        return {std::stoll(str1), std::stoll(str2)};
    };

    for (int blink = 1; blink <= 25; blink++) {
        for (auto it = stones.begin(); it != stones.end(); it++) {
            if (*it == 0) {
                *it = 1;
                continue;
            }

            const long long num_digits = std::floor(std::log10(static_cast<double>(*it))) + 1;
            if (num_digits > 0 and num_digits % 2 == 0) {
                const auto&& [num1, num2] = split(*it);
                *it = num1;
                stones.insert(it, num2);
                continue;
            }

            *it = (*it) * 2024;
        }
    }

    return stones.size();
}

int main() {
    static_assert(__cplusplus >= 201700L, "C++ version is not 17 or above");
    std::fstream f("input.txt", std::ios::in);

    if (!f.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    auto&& stones = read(f);
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << solve(std::move(stones)) << "\n";
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Elapsed time: " << duration << " μs\n";
    return 0;
}
