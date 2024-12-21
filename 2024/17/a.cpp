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

void solve(Computer computer) {
    const auto len = static_cast<ssize_t>(computer.program.size());
    ssize_t ip = 0;
    while (ip < len) {
        const auto opcode = computer.program[ip];
        const auto literal_oprand = computer.program[ip + 1];

        const auto combo_oprand = computer.get_combo_oprand(literal_oprand);

        switch (opcode) {
            case 0:  // adv
                computer.A = std::trunc(computer.A / std::exp2(combo_oprand));
                break;
            case 1:  // bxl
                computer.B = computer.B ^ literal_oprand;
                break;
            case 2:  // bst
                computer.B = combo_oprand % 8;
                break;
            case 3:  // jnz
                if (computer.A != 0) {
                    ip = literal_oprand;
                    continue;
                }
                break;
            case 4:  // bxc
                computer.B = computer.B ^ computer.C;
                break;
            case 5:  // out
                std::cout << combo_oprand % 8 << ",";
                break;
            case 6:  // bdv
                computer.B = std::trunc(computer.A / std::exp2(combo_oprand));
                break;
            case 7:  // cdv
                computer.C = std::trunc(computer.A / std::exp2(combo_oprand));
                break;
            default:
                break;
        }

        ip += 2;
    }
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
    solve(computer);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    std::cout << "\nElapsed time: " << duration << " ns\n";
    return 0;
}
