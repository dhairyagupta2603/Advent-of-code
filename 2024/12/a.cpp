#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <queue>

auto read(std::fstream& f) {
    std::string line;
    std::vector<std::string> map;
    map.emplace_back();

    // Add a buffer
    while (std::getline(f, line)) {
        line = '#' + line + '#';
        map.push_back(line);
    }
    map[0] = std::string(map[1].size(), '#');
    map.emplace_back(map[0]);

    return map;
}

struct point_t {
    int row;
    int col;
    bool operator==(const point_t& p) const { return row == p.row && col == p.col; }
};

constexpr std::array<point_t, 4> directions = {{point_t{0, 1}, point_t{1, 0}, point_t{0, -1}, point_t{-1, 0}}};

int64_t calculate_cost(const std::vector<std::string>& map, const int32_t row_idx, const int32_t col_idx,
                       std::vector<std::vector<bool>>& visited) {
    const auto c = map[row_idx][col_idx];
    if (c == '#') return 0;

    std::queue<point_t> q;
    q.push(point_t{row_idx, col_idx});

    int64_t area = 0, perimeter = 0;
    while (!q.empty()) {
        auto p = q.front();
        q.pop();

        if (visited[p.row][p.col]) continue;

        visited[p.row][p.col] = true;
        area++;
        for (const auto& motion : directions) {
            const auto next = point_t{p.row + motion.row, p.col + motion.col};
            if (map[next.row][next.col] != c) {
                perimeter++;
                continue;
            }

            if (map[next.row][next.col] == c && !visited[next.row][next.col]) {
                q.push(next);
            }
        }
    }
    return area * perimeter;
}

int64_t solve(std::vector<std::string>&& grid) {
    int64_t result = 0;
    std::vector<std::vector<bool>> visited(grid.size(), std::vector<bool>(grid[0].size(), false));
    for (ssize_t i = 0; i < static_cast<ssize_t>(grid.size()); i++) {
        for (ssize_t j = 0; j < static_cast<ssize_t>(grid[i].size()); j++) {
            if (visited[i][j]) continue;
            if (grid[i][j] == '#') continue;
            result += calculate_cost(grid, i, j, visited);
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

    auto&& grid = read(f);
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << solve(std::move(grid)) << "\n";
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Elapsed time: " << duration << " μs\n";
    return 0;
}
