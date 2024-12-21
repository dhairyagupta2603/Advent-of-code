#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <unordered_map>

auto read(std::fstream& f) {
    std::unordered_map<long long, long long> stones;
    long long num = -1;
    while (f >> num) {
        stones[num]++;
    }

    return stones;
}

void print(const std::unordered_map<long long, long long>& stones) {
    for (const auto& [k, v] : stones) {
        std::cout << k << ": " << v << "\n";
    }
}

long long get_digits(const long long num) { return std::floor(std::log10(static_cast<double>(num))) + 1; }

void blink(std::unordered_map<long long, long long>& stones,
           std::unordered_map<long long, std::vector<long long>>& results) {
    std::unordered_map<long long, long long> next_stones;
    for (const auto& [stone, freq] : stones) {
        if (stone == 0) {
            next_stones[1] += freq;
            continue;
        }

        auto& res = results[stone];
        if (res.size() == 0) {
            const auto num_digits = get_digits(stone);
            if (num_digits % 2 != 0) {
                res = {stone * 2024};
            } else {
                const long long divisor = std::pow(10, static_cast<int>(num_digits / 2));
                res = {stone / divisor, stone % divisor};
            }
        }

        for (const auto& val : res) {
            next_stones[val] += freq;
        }
    }

    stones = next_stones;
}

long solve(std::unordered_map<long long, long long>& stones) {
    std::unordered_map<long long, std::vector<long long>> results;
    for (int i = 1; i <= 75; i++) {
        blink(stones, results);
    }

    long result = 0;
    for (const auto& [_, freq] : stones) {
        result += freq;
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

    auto&& stones = read(f);
    // print(stones);
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << solve(stones) << "\n";
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Elapsed time: " << duration << " μs\n";
    return 0;
}
