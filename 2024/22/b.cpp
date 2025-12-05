#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>

auto read(std::fstream& f) {
    std::vector<int32_t> secrets;
    std::string line;
    while (std::getline(f, line)) {
        secrets.emplace_back(std::stoll(line));
    }

    return secrets;
}

int32_t get_next(int32_t curr) {
    // Calculate the result of multiplying the secret number by 64
    int32_t s1 = curr << 6;
    // mix this result into the secret number
    curr = s1 ^ curr;
    // Finally, prune the secret number
    curr = curr % static_cast<int32_t>(std::pow(2, 24));

    // ones did not change

    // Calculate the result of dividing the secret number by 32
    int32_t s2 = curr >> 5;
    // Round the result down to the nearest integer
    s2 = std::floor(s2);
    // Then, mix this result into the secret number
    curr = curr ^ s2;

    // prune the secret number
    curr = curr % static_cast<int32_t>(std::pow(2, 24));
    // Calculate the result of multiplying the secret number by 2048
    int32_t s3 = curr << 11;
    // mix this result into the secret number
    curr = curr ^ s3;

    // prune the secret number
    curr = curr % static_cast<int32_t>(std::pow(2, 24));

    return curr;
}

int32_t get_next_ones(int32_t curr) {
    // Calculate the result of multiplying the secret number by 64
    int32_t s1 = curr << 6;
    // mix this result into the secret number
    curr = s1 ^ curr;
    // Finally, prune the secret number
    curr = curr % static_cast<int32_t>(std::pow(2, 24));

    // ones did not change

    // Calculate the result of dividing the secret number by 32
    int32_t s2 = curr >> 5;
    // Round the result down to the nearest integer
    s2 = std::floor(s2);
    // Then, mix this result into the secret number
    curr = curr ^ s2;

    // prune the secret number
    curr = curr % static_cast<int32_t>(std::pow(2, 24));
    // Calculate the result of multiplying the secret number by 2048
    int32_t s3 = curr << 11;
    // mix this result into the secret number
    curr = curr ^ s3;

    // prune the secret number
    curr = curr % static_cast<int32_t>(std::pow(2, 24));

    return curr;
}

int64_t solve(const std::vector<int32_t>& secrets) {
    const int32_t NUM_NEXT_SECRETS = 2'000;

    int64_t result = 0;
    for (const auto& secret : secrets) {
        int32_t curr_secret = secret;
        for (int32_t num = 0; num < NUM_NEXT_SECRETS; num++) {
            curr_secret = get_next(curr_secret);
        }

        result += curr_secret;
    }
    return result;
}

int main() {
    static_assert(__cplusplus >= 201700L, "C++ version is not 17 or above");
    std::fstream f("test2.txt", std::ios::in);

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
