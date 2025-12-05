#include <algorithm>
#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

void multiply(std::fstream& file, char ch, long long& result) {
    // get next 12 or so chars
    std::vector<char> buf;
    buf.reserve(12);
    buf.emplace_back(ch);

    for (int i = 1; i < 12; i++) {
        buf.emplace_back(file.peek());
        if (buf[i] == EOF or buf[i] == ')' or buf[i] == 'm') break;
        file.get();
    }

    const std::string mul(buf.data(), buf.size());

    // search for mul
    const std::regex mul_pattern("mul\\([0-9]{1,3},[0-9]{1,3}\\)");
    std::smatch mul_matched;
    if (std::regex_search(mul, mul_matched, mul_pattern)) {
        // serch for numbers in mul
        const std::regex num_pattern("[0-9]{1,3}");
        std::string mul_str = mul_matched.str();
        std::smatch num_matched;

        // search for first number
        std::regex_search(mul_str, num_matched, num_pattern);
        int a = std::stoi(num_matched.str());

        // search for second number
        mul_str = num_matched.suffix().str();
        std::regex_search(mul_str, num_matched, num_pattern);
        int b = std::stoi(num_matched.str());

        result += (a * b);
    }
}

long long solve(std::fstream& file) {
    file.rdbuf()->pubsetbuf(nullptr, 0);

    long long result = 0;
    char ch = '\0';
    while (file.get(ch)) {
        if (ch == 'm') {
            multiply(file, ch, result);
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
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << solve(f) << "\n";
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Elapsed time: " << duration << " ms\n";
    return 0;
}
