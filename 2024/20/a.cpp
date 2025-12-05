#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <tuple>
#include <unordered_map>
#include <vector>

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ h2;  // Combine the two hash values
    }
};

struct tuple_hash {
    template <class T1, class T2, class T3, class T4>
    std::size_t operator()(const std::tuple<T1, T2, T3, T4>& t) const {
        auto h1 = std::hash<T1>{}(std::get<0>(t));
        auto h2 = std::hash<T2>{}(std::get<1>(t));
        auto h3 = std::hash<T3>{}(std::get<2>(t));
        auto h4 = std::hash<T4>{}(std::get<3>(t));
        return h1 ^ h2 ^ h3 ^ h4;  // Combine the hash values
    }
};

const std::vector<std::pair<int, int>> directions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

bool in_grid(int i, int j, int N) { return i >= 0 && i < N && j >= 0 && j < N; }

int main() {
    std::fstream f("./input.txt");
    std::vector<std::vector<char>> grid;
    std::string line;

    // Reading the grid from input file
    while (std::getline(f, line)) {
        grid.push_back(std::vector<char>(line.begin(), line.end()));
    }

    int N = grid.size();
    int si = -1, sj = -1, ei = -1, ej = -1;

    // Find the start (S) and end (E) positions
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (grid[i][j] == 'S') {
                si = i;
                sj = j;
            }
            if (grid[i][j] == 'E') {
                ei = i;
                ej = j;
            }
        }
    }

    // Debugging: Check if S and E are found
    std::cout << "Start position: " << si << ", " << sj << "\n";
    std::cout << "End position: " << ei << ", " << ej << "\n";

    // Determine the original path from S to E
    std::vector<std::pair<int, int>> path = {{si, sj}};
    while (path.back() != std::make_pair(ei, ej)) {
        auto [i, j] = path.back();
        bool found = false;
        for (const auto& [di, dj] : directions) {
            int ii = i + di, jj = j + dj;
            if (!in_grid(ii, jj, N) || grid[ii][jj] == '#' ||
                (path.size() > 1 && std::make_pair(ii, jj) == path[path.size() - 2])) {
                continue;
            }
            path.push_back({ii, jj});
            found = true;
            break;
        }
        if (!found) break;
    }

    int og = path.size() - 1;

    // Store times for each coordinate
    std::unordered_map<std::pair<int, int>, int, pair_hash> times;
    for (int t = 0; t < path.size(); ++t) {
        times[path[t]] = og - t;
    }

    // Initialize counts and saved dictionary
    std::unordered_map<int, int> counts;
    std::unordered_map<std::tuple<int, int, int, int>, int, tuple_hash> saved;

    // Process paths and save potential times
    for (int t = 0; t < path.size(); ++t) {
        auto [i, j] = path[t];
        for (const auto& [di1, dj1] : directions) {
            for (const auto& [di2, dj2] : directions) {
                int ii = i + di1 + di2, jj = j + dj1 + dj2;
                if (!in_grid(ii, jj, N) || grid[ii][jj] == '#') {
                    continue;
                }

                int rem_t = times.count({ii, jj}) ? times[{ii, jj}] : 0;
                saved[{i, j, ii, jj}] = og - (t + rem_t + 2);
            }
        }
    }

    // Calculate the final answer
    int ans = 0;
    for (const auto& [key, v] : saved) {
        if (v >= 0) {
            counts[v]++;
        }
        if (v >= 100) {
            ans++;
        }
    }

    std::cout << ans << "\n";
    return 0;
}
