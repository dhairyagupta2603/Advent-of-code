#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <complex>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <tuple>
#include <utility>

auto read(std::fstream& f) {
    std::vector<int64_t> secrets;
    std::string line;
    while (std::getline(f, line)) {
        secrets.emplace_back(std::stoll(line));
    }

    return secrets;
}

int64_t get_next(int64_t curr) {
    // Calculate the result of multiplying the secret number by 64
    int64_t s1 = curr * 64;

    // mix this result into the secret number
    curr = s1 ^ curr;

    // Finally, prune the secret number
    curr = curr % 16777216;

    // Calculate the result of dividing the secret number by 32
    int64_t s2 = curr / 32;

    // Round the result down to the nearest integer
    s2 = std::floor(s2);

    // Then, mix this result into the secret number
    curr = curr ^ s2;

    // prune the secret number
    curr = curr % 16777216;

    // Calculate the result of multiplying the secret number by 2048
    int64_t s3 = curr * 2048;

    // mix this result into the secret number
    curr = curr ^ s3;

    // prune the secret number
    curr = curr % 16777216;

    return curr;
}

int64_t solve(const std::vector<int64_t>& secrets) {
    const int32_t NUM_NEXT_SECRETS = 2'000;

    int64_t result = 0;
    for (const auto& secret : secrets) {
        int64_t curr_secret = secret;
        for (int32_t num = 0; num < NUM_NEXT_SECRETS; num++) {
            curr_secret = get_next(curr_secret);
        }

        result += curr_secret;
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

    auto secrets = read(f);
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << solve(secrets) << "\n";
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Elapsed time: " << duration << " ms\n";
    return 0;
}
