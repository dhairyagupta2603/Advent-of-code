#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <complex>
#include <fstream>
#include <iostream>
#include <new>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

auto read(std::fstream& f) {
    char ch = '\0';
    std::vector<int> disk_map;
    while (f.get(ch)) {
        disk_map.emplace_back(static_cast<int>(ch - '0'));
    }
    return disk_map;
}

long compact(std::vector<int>& map) {
    const auto len = static_cast<ssize_t>(map.size());

    // use two-pointer appoch to compact the vector
    long i = 0, j = len - 1;
    while (i <= j) {
        if (map[i] != -1) {
            i++;
            continue;
        }

        if (map[j] == -1) {
            j--;
            continue;
        }

        std::swap(map[i], map[j]);
        i++;
        j--;
    }

    return j;
}

long long solve(const std::vector<int>& disk_map) {
    const auto len = static_cast<ssize_t>(disk_map.size());

    // expanded disk map
    std::vector<int> expanded_map;
    long file_idx = 0;
    for (auto i = 0; i < len; i++) {
        const auto block_size = disk_map[i];
        if (i % 2 == 0) {  // if even, file
            for (auto j = 0; j < block_size; j++) expanded_map.emplace_back(file_idx);
            file_idx++;
        } else {  // if odd, free space
            for (auto j = 0; j < block_size; j++) expanded_map.emplace_back(-1);
        }
    }

    const long reduced_len = compact(expanded_map);
    long long result = 0;
    for (auto i = 0; i <= reduced_len; i++) {
        result += (i * expanded_map[i]);
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

    auto disk_map = read(f);

    auto start = std::chrono::high_resolution_clock::now();
    std::cout << solve(disk_map) << "\n";
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Elapsed time: " << duration << " μs\n";
    return 0;
}
