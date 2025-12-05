#include <algorithm>
#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <utility>

struct computer_t {
    char first;
    char second;

    struct hash {
        std::size_t operator()(const computer_t& c) const {
            std::size_t h1 = std::hash<char>()(c.first);
            std::size_t h2 = std::hash<char>()(c.second);
            return h1 ^ (h2 << 3);
        }
    };

    bool operator==(const computer_t& o) const { return first == o.first && second == o.second; }
    bool operator<(const computer_t& o) const { return first < o.first && second < o.second; }

    friend std::ostream& operator<<(std::ostream& os, const computer_t& c) {
        os << c.first << c.second;
        return os;
    }
};

using link_t = std::pair<computer_t, computer_t>;
auto read(std::fstream& f) {
    std::vector<link_t> computer_links;
    std::string line;
    while (std::getline(f, line)) {
        std::istringstream iss(line);
        computer_t c1, c2;

        char ch = '\0';
        iss.get(ch);
        c1.first = ch;
        iss.get(ch);
        c1.second = ch;

        iss.get(ch);  // -

        iss.get(ch);
        c2.first = ch;
        iss.get(ch);
        c2.second = ch;

        computer_links.emplace_back(c1, c2);
    }

    return computer_links;
}

bool is_connected(const std::vector<std::vector<int32_t>>& graph, const std::unordered_set<size_t>& visited,
                  int32_t idx) {
    for (const int32_t& v : visited) {
        if (graph[idx][v] == 0) return false;
    }

    return true;
}

void dfs(const std::vector<std::vector<int32_t>>& graph, std::unordered_set<size_t>& visited, size_t cidx) {
    for (size_t i = 0; i < graph[cidx].size(); i++) {
        if (graph[cidx][i] == 0 or visited.find(i) != visited.end() or !is_connected(graph, visited, i)) continue;

        visited.insert(i);
        dfs(graph, visited, i);
    }
}

int64_t solve(const std::vector<link_t>& links) {
    std::unordered_set<computer_t, computer_t::hash> computer_set;
    for (const auto& [c1, c2] : links) {
        computer_set.insert(c1);
        computer_set.insert(c2);
    }

    std::vector<computer_t> computers(computer_set.begin(), computer_set.end());
    std::vector<std::vector<int32_t>> graph(computers.size(), std::vector<int>(computers.size(), 0));
    for (size_t i = 0; i < computers.size(); i++) {
        for (size_t j = 0; j < computers.size(); j++) {
            if (computers[i] == computers[j]) continue;

            if (std::find(links.begin(), links.end(), std::make_pair(computers[i], computers[j])) != links.end() ||
                std::find(links.begin(), links.end(), std::make_pair(computers[j], computers[i])) != links.end()) {
                graph[i][j] = 1;
                graph[j][i] = 1;
            }
        }
    }

    std::vector<computer_t> cs;
    size_t largest_lan = 0;
    for (size_t i = 0; i < graph.size(); i++) {
        std::unordered_set<size_t> visited;
        dfs(graph, visited, i);

        if (largest_lan < visited.size()) {
            cs.clear();
            for (const auto& n : visited) cs.emplace_back(computers[n]);
            largest_lan = visited.size();
        }
    }

    std::sort(cs.begin(), cs.end(), [](const computer_t& a, const computer_t& b) {
        return a.first < b.first || (a.first == b.first && a.second < b.second);
    });

    for (const auto& c : cs) std::cout << c << ",";
    std::cout << "\n";

    return 0;
}

int main() {
    static_assert(__cplusplus >= 201700L, "C++ version is not 17 or above");
    std::fstream f("input.txt", std::ios::in);

    if (!f.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    auto links = read(f);
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << solve(links) << "\n";
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Elapsed time: " << duration << " ms\n";
    return 0;
}
