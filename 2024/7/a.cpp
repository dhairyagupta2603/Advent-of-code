#include <algorithm>
#include <array>
#include <cassert>
#include <complex>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

template <typename T>
using Calibration = std::pair<T, std::vector<T>>;

void print(const Calibration<long long>& calibration) {
    std::cout << calibration.first << ": ";
    for (const auto& input : calibration.second) {
        std::cout << input << " ";
    }
    std::cout << "\n";
}

auto read(std::fstream& f) {
    std::vector<Calibration<long long>> calibrations;

    std::string line;
    while (std::getline(f, line)) {
        std::stringstream ss(line);

        std::string ans_value_str;
        std::getline(ss, ans_value_str, ':');

        long val = -1;
        std::vector<long long> inputs;
        while (ss >> val) {
            inputs.emplace_back(val);
        }
        calibrations.push_back({std::stoll(ans_value_str), inputs});
    }

    return calibrations;
}

bool calibrate(const Calibration<long long>& calibration, const long pos, const long long final) {
    if (pos == static_cast<ssize_t>(calibration.second.size())) {
        return final == calibration.first;
    }

    if (final > calibration.first) return false;

    if (calibrate(calibration, pos + 1, final + calibration.second[pos])) return true;
    if (calibrate(calibration, pos + 1, final * calibration.second[pos])) return true;
    return false;
}

long long solve(const std::vector<Calibration<long long>>& calibrations) {
    long long result = 0;
    for (const auto& calibration : calibrations) {
        if (calibrate(calibration, 1, calibration.second[0])) {
            result += calibration.first;
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

    auto calibrations = read(f);
    std::cout << solve(calibrations) << "\n";
    return 0;
}
