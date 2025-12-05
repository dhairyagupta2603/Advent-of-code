#include <algorithm>
#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <optional>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <utility>

enum class GATE { OR, AND, XOR };
struct operation_t {
    std::string inp1;
    std::string inp2;
    GATE gate;
    std::string result;

    operation_t(const std::string& in1, const std::string& in2, GATE g, const std::string& res)
        : inp1(in1), inp2(in2), gate(g), result(res) {}

    friend std::ostream& operator<<(std::ostream& os, const operation_t& op) {
        os << op.inp1 << " " << static_cast<int32_t>(op.gate) << " " << op.inp2 << " -> " << op.result;
        return os;
    }
};

auto read(std::fstream& fa, std::fstream& fb)
    -> std::tuple<std::unordered_map<std::string, int16_t>, std::list<operation_t>> {
    std::unordered_map<std::string, int16_t> wires;

    std::string line;
    while (std::getline(fa, line)) {
        std::istringstream iss(std::move(line));
        iss >> line;
        iss.ignore(4, ' ');
        int32_t val;
        iss >> val;
        wires[line.substr(0, 3)] = val;
    }

    std::list<operation_t> operations;
    while (std::getline(fb, line)) {
        std::istringstream iss(line);
        std::string input1, input2, result, op, temp;

        // Parsing the gate operation
        iss >> input1 >> op >> input2 >> temp >> result;
        GATE g;
        if (op == "AND") {
            g = GATE::AND;
        } else if (op == "OR") {
            g = GATE::OR;
        } else {
            g = GATE::XOR;
        }

        operations.push_back(operation_t{input1, input2, g, result});
    }

    return {wires, operations};
}

int16_t operate(const int16_t& a, const int16_t& b, GATE g) {
    if (g == GATE::AND) {
        return a & b;
    } else if (g == GATE::OR) {
        return a | b;
    } else {
        return a ^ b;
    }
}

int64_t solve(std::unordered_map<std::string, int16_t> wires, std::list<operation_t> operations) {
    std::vector<std::pair<int32_t, int64_t>> zwires;

    auto it = operations.begin();
    while (operations.size() > 0) {
        if (it == operations.end()) {
            it = operations.begin();
        }

        if (wires.find(it->inp1) == wires.end() or wires.find(it->inp2) == wires.end()) {
            it++;
            continue;
        }

        const int16_t input1 = wires.at(it->inp1);
        const int16_t input2 = wires.at(it->inp2);
        const auto res_val = operate(input1, input2, it->gate);
        if (it->result[0] == 'z') {
            zwires.emplace_back(std::stoll(it->result.substr(1, 2)), res_val);
        }

        wires[it->result] = res_val;
        it = operations.erase(it);
    }

    std::sort(zwires.begin(), zwires.end(), [](const auto& a, const auto& b) { return a.first < b.first; });

    int64_t result = 0;
    for (ssize_t i = static_cast<ssize_t>(zwires.size()) - 1; i >= 0; i--) {
        result = (result << 1) | zwires[i].second;
    }
    return result;
}

int main() {
    static_assert(__cplusplus >= 201700L, "C++ version is not 17 or above");
    std::fstream fa("inputa.txt", std::ios::in);
    std::fstream fb("inputb.txt", std::ios::in);

    if (!fa.is_open() or !fb.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    auto [wires, operations] = read(fa, fb);
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << solve(wires, operations) << "\n";
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Elapsed time: " << duration << " μs\n";
    return 0;
}
