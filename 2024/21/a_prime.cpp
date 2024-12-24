#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <complex>
#include <fstream>
#include <iostream>
#include <limits>
#include <nlohmann/json.hpp>
#include <sstream>
#include <tuple>
#include <utility>

auto read(std::fstream& f) {
    std::vector<std::string> combination_keys;
    std::string combination_key;
    while (std::getline(f, combination_key)) {
        combination_keys.emplace_back(combination_key);
    }

    return combination_keys;
}

void print(const std::vector<std::string>& combination_keys) {
    for (const auto& key : combination_keys) {
        std::cout << key << "\n";
    }
}

using src_t = char;
struct mvt_t {
    char dest;
    std::string kp;
};

std::string get_to_dp(const nlohmann::json& data, const std::string& key) {
    std::vector<std::string> dps;
    char curr = 'A';

    // from keypad to dirpad 0
    for (const auto& next : key) {
        if (curr == next) {
            dps.emplace_back("A");
        } else {
            auto dp = data.at(std::string(1, curr)).at(std::string(1, next)).template get<std::string>();
            dps.emplace_back(dp);
        }
        // std::cout << curr << " to " << next << ": " << dp << "\n";
        curr = next;
    }

    std::string result = std::accumulate(dps.begin() + 1, dps.end(), dps[0],
                                         [](const std::string& a, const std::string& b) { return a + b; });

    std::cout << "Resulting string: " << result << " with size " << result.size() << std::endl;
    return result;
}

int64_t solve(const std::vector<std::string>& combination_keys) {
    std::ifstream f1("./kp2d0.json"), f2("./dn2dnx1.json");
    const auto kp2d0 = nlohmann::json::parse(f1);
    const auto dn2dnx1 = nlohmann::json::parse(f2);

    int64_t result = 0;
    for (const auto& key : combination_keys) {
        std::cout << "For key: " << key << "\n";
        auto dpsn = get_to_dp(kp2d0, key);

        for (int32_t i = 0; i < 2; i++) {
            dpsn = get_to_dp(dn2dnx1, dpsn);
        }

        result += (dpsn.size() * std::stoll(key.substr(0, key.size() - 1)));
    }

    return result;
}

int main() {
    static_assert(__cplusplus >= 201700L, "C++ version is not 17 or above");
    std::fstream f("test.txt", std::ios::in);

    if (!f.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    auto combination_keys = read(f);
    // print(grid);
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << solve(combination_keys) << "\n";
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    // print_cost(grid);
    std::cout << "Elapsed time: " << duration << " ms\n";
    return 0;
}
