#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::vector<int>> read(std::fstream& file) {
    std::vector<std::vector<int>> reports;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<int> report;
        int num = -1;

        while (iss >> num) report.emplace_back(num);
        reports.emplace_back(report);
    }

    return reports;
}

bool is_report_safe(const std::vector<int>& report) {
    int sign = 0;
    for (auto i = 0; i < static_cast<ssize_t>(report.size()) - 1; i++) {
        int diff = report[i + 1] - report[i];
        if (!(std::abs(diff) <= 3 and std::abs(diff) >= 1)) return false;

        int new_sign = diff / std::abs(diff);
        if (sign != 0 and sign != new_sign) return false;
        sign = new_sign;
    }
    return true;
}

int solve(const std::vector<std::vector<int>>& reports) {
    int result = 0;
    for (const auto& report : reports) {
        result += (is_report_safe(report) ? 1 : 0);
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

    const auto&& reports = read(f);
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << solve(reports) << "\n";
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Elapsed time: " << duration << " μs\n";
    return 0;
}
