#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template <typename K, typename V>
using Graph = std::unordered_map<K, std::unordered_set<V>>;

void print_rules(const Graph<int, int>& graph) {
    for (const auto& [k, connections] : graph) {
        std::cout << k << ": ";
        for (const auto& elem : connections) std::cout << elem << " ";
        std::cout << "\n";
    }
}

auto read_rules(std::fstream& f) {
    Graph<int, int> rules;
    int a = -1, b = -1;
    char pipe = '\0';
    while (f >> a >> pipe >> b) {
        if (rules.find(a) == rules.end()) {
            std::unordered_set<int> conn;
            conn.insert(b);
            rules.insert({a, conn});
        } else {
            rules.at(a).insert(b);
        }
    }

    return rules;
}

auto read_prints(std::fstream& f) {
    std::vector<std::vector<int>> prints;

    std::string line;
    while (std::getline(f, line)) {
        std::stringstream ss(line);
        std::vector<int> row;
        std::string value;

        while (std::getline(ss, value, ',')) {
            row.push_back(std::stoi(value));
        }

        prints.push_back(row);
    }

    return prints;
}

std::pair<bool, std::vector<int>> make_valid(const Graph<int, int>& rules, std::vector<int> print) {
    const auto rules_contain = [&rules](const int a) -> bool { return (rules.find(a) != rules.end()); };

    bool is_changed = false;
    const auto len = static_cast<ssize_t>(print.size());
    for (ssize_t i = 0; i < len - 1; i++) {
        for (ssize_t j = i + 1; j < len; j++) {
            // if a in list of b, than a comes after b (invalid)
            while (rules_contain(print.at(j))) {
                const auto node = rules.at(print.at(j));
                // canot find a in b
                if (node.find(print.at(i)) == node.end()) break;
                std::swap(print.at(i), print.at(j));
                is_changed = true;
            }
        }
    }

    return {is_changed, print};
}

int solve(const Graph<int, int>& rules, const std::vector<std::vector<int>>& prints) {
    // for eah i check if all i+1, i+2,... follwthe graph,eles retun false
    // if all test pass, add index to result set
    std::vector<std::vector<int>> invalid_prints;
    for (const auto& print : prints) {
        if (const auto& [is_changed, valid_print] = make_valid(rules, print); is_changed) {
            invalid_prints.push_back(valid_print);
        }
    }

    // for all index of set, get middle element sum
    int result_sum = 0;
    for (auto print : invalid_prints) {
        result_sum += print.at(int(std::floor(static_cast<ssize_t>(print.size()) / 2.0)));
    }
    return result_sum;
}

int main() {
    static_assert(__cplusplus >= 201700L, "C++ version is not 20 or above");
    std::fstream rule_file("5/input1.txt", std::ios::in);
    if (!rule_file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }
    std::fstream print_file("5/input2.txt", std::ios::in);
    if (!print_file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    auto rules = read_rules(rule_file);
    auto prints = read_prints(print_file);
    std::cout << solve(rules, prints) << "\n";
    return 0;
}
