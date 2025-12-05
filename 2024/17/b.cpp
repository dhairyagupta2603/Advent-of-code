#include <algorithm>
#include <bitset>
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

struct Computer {
    int64_t A;
    int64_t B;
    int64_t C;

    std::vector<int64_t> program;

    int64_t get_combo_oprand(int64_t oprand) const {
        int64_t combo_oprand = 7;
        if (oprand == 4)
            combo_oprand = A;
        else if (oprand == 5)
            combo_oprand = B;
        else if (oprand == 6)
            combo_oprand = C;
        else
            combo_oprand = oprand;

        return combo_oprand;
    }

    std::vector<int64_t> compute() {
        const auto len = static_cast<ssize_t>(program.size());

        std::vector<int64_t> result;
        ssize_t ip = 0;

        while (ip < len) {
            if (result.size() > program.size()) {
                break;
            }

            const auto opcode = program[ip];
            const auto literal_oprand = program[ip + 1];
            const auto combo_oprand = get_combo_oprand(literal_oprand);

            switch (opcode) {
                case 0:  // adv
                    A = std::trunc(A / std::pow(2, combo_oprand));
                    break;
                case 1:  // bxl
                    B = B ^ literal_oprand;
                    break;
                case 2:  // bst
                    B = combo_oprand % 8;
                    break;
                case 3:  // jnz
                    if (A != 0) {
                        ip = literal_oprand;
                        continue;
                    }
                    break;
                case 4:  // bxc
                    B = B ^ C;
                    break;
                case 5:  // out
                    // if (B % 8 != 2) break;
                    result.emplace_back(combo_oprand % 8);
                    break;
                case 6:  // bdv
                    B = std::trunc(A / std::pow(2, combo_oprand));
                    break;
                case 7:  // cdv
                    C = std::trunc(A / std::pow(2, combo_oprand));
                    break;
                default:
                    break;
            }

            ip += 2;
        }

        return result;
    }
};

auto read(std::fstream& f) {
    Computer computer;

    std::string line;
    std::istringstream iss;

    // register A
    std::getline(f, line);
    iss.str(line);

    iss.ignore(10, 'A');
    iss.ignore(1, ':');
    iss.ignore(1, ' ');
    iss >> line;
    computer.A = std::stoll(line);

    // register B
    std::getline(f, line);
    iss.clear();
    iss.str(line);

    iss.ignore(9, 'B');
    iss.ignore(1, ':');
    iss.ignore(1, ' ');
    iss >> line;
    computer.B = std::stoll(line);

    // register C
    std::getline(f, line);
    iss.clear();
    iss.str(line);

    iss.ignore(9, 'C');
    iss.ignore(1, ':');
    iss.ignore(1, ' ');
    iss >> line;
    computer.C = std::stoll(line);

    // program
    iss.clear();
    std::getline(f, line);
    std::getline(f, line);
    iss.str(line);
    iss.ignore(7, ':');
    iss.ignore(1, ' ');

    std::string num;
    while (std::getline(iss, num, ',')) {
        computer.program.emplace_back(std::stoll(num));
    }
    return computer;
}

int64_t solve(Computer computer) {
    int32_t power = 14;
    int64_t possible_A = std::pow(8, 15);  // minimum value required to have a 16 digit output
    std::vector<int64_t> matched(computer.program.rend() - 1,
                                 computer.program.rend());  // last n digits of program that it looks for
    std::vector<int64_t> result;

    while (result != computer.program) {
        possible_A += std::pow(8, power);
        computer.A = possible_A;
        result = computer.compute();

        // when the digits match, decrement the power by 1, by decreasing the power, the matched digits will no longer
        // change
        if (std::equal(result.end() - matched.size(), result.end(), matched.begin())) {
            power = std::max(0, power - 1);
            matched = std::vector<int64_t>(computer.program.end() - matched.size() - 1, computer.program.end());
        }
    }

    return possible_A;
}

int main() {
    static_assert(__cplusplus >= 201700L, "C++ version is not 17 or above");
    std::fstream f("input.txt", std::ios::in);

    if (!f.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    auto computer = read(f);
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << solve(computer) << "\n";
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "\nElapsed time: " << duration << " ms\n";
    return 0;
}
