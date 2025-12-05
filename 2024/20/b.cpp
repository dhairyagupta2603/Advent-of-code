#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <tuple>
#include <unordered_map>
#include <vector>

using namespace std;

// Custom hash function for std::pair<int, int>
struct pair_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 3);
    }
};

// Custom hash function for std::tuple<int, int, int, int>
struct tuple_hash {
    template <class T1, class T2, class T3, class T4>
    std::size_t operator()(const std::tuple<T1, T2, T3, T4>& t) const {
        auto h1 = std::hash<T1>{}(std::get<0>(t));
        auto h2 = std::hash<T2>{}(std::get<1>(t));
        auto h3 = std::hash<T3>{}(std::get<2>(t));
        auto h4 = std::hash<T4>{}(std::get<3>(t));
        return h1 ^ (h2 << 3) ^ (h3 << 6) ^ (h4 << 9);
    }
};

// Directions for moving (down, right, up, left)
const vector<pair<int, int>> directions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

// Check if a position is within the grid
bool in_grid(int i, int j, int N) { return i >= 0 && i < N && j >= 0 && j < N; }

int main() {
    ifstream fin("./input.txt");
    vector<vector<char>> grid;
    string line;

    // Reading the grid from input file
    while (getline(fin, line)) {
        grid.push_back(vector<char>(line.begin(), line.end()));
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

    // Check if S and E are found
    if (si == -1 || sj == -1 || ei == -1 || ej == -1) {
        cerr << "Start (S) or End (E) position not found in the grid." << endl;
        return 1;
    }

    // Debugging: Check if S and E are found
    cout << "Start position: " << si << ", " << sj << endl;
    cout << "End position: " << ei << ", " << ej << endl;

    // Determine the original path from S to E
    vector<pair<int, int>> path = {{si, sj}};
    while (path.back() != make_pair(ei, ej)) {
        auto [i, j] = path.back();
        bool found = false;
        for (const auto& [di, dj] : directions) {
            int ii = i + di, jj = j + dj;
            if (!in_grid(ii, jj, N) || grid[ii][jj] == '#' ||
                (path.size() > 1 && make_pair(ii, jj) == path[path.size() - 2])) {
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
    unordered_map<pair<int, int>, int, pair_hash> times;
    for (int t = 0; t < path.size(); ++t) {
        times[path[t]] = og - t;
    }

    int max_len = 20;
    unordered_map<int, int> counts;
    unordered_map<tuple<int, int, int, int>, int, tuple_hash> saved;

    // Process paths and save potential times
    for (int t = 0; t < path.size(); ++t) {
        std::cout << t << "\t" << path.size() << "\n";
        auto [i, j] = path[t];

        for (int ii = i - max_len; ii <= i + max_len; ++ii) {
            for (int jj = j - max_len; jj <= j + max_len; ++jj) {
                int time_used = abs(ii - i) + abs(jj - j);

                if (!in_grid(ii, jj, N) || time_used > max_len || grid[ii][jj] == '#') {
                    continue;
                }

                int rem_t = times.count({ii, jj}) ? times[{ii, jj}] : 0;
                saved[{i, j, ii, jj}] = og - (t + rem_t + time_used);
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

    cout << ans << endl;
    return 0;
}
