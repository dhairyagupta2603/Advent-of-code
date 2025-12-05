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

template <typename T>
using matrix_t = std::vector<std::vector<T>>;

struct index_t {
    int32_t i;
    int32_t j;

    index_t(int32_t _i, int32_t _j) : i{_i}, j{_j} {}

    template <typename T>
    bool is_valid(const matrix_t<T>& grid) const {
        const auto rows = static_cast<ssize_t>(grid.size());
        const auto cols = static_cast<ssize_t>(grid[0].size());

        return i >= 0 and i < rows and j >= 0 and j < cols;
    }

    bool operator==(const index_t& other) { return other.i == i and other.j == j; }
    bool operator!=(const index_t& other) { return !(*this == other); }

    friend std::ostream& operator<<(std::ostream& os, const index_t& idx) {
        os << "Index(i=" << idx.i << ", j=" << idx.j << ")";
        return os;
    }
};

struct Cell {
    char ch;
    int64_t cost;
    bool best;

    Cell(char _ch, int64_t _cost) : ch{_ch}, cost{_cost}, best{false} {}
};

auto read(std::fstream& f) {
    matrix_t<Cell> grid;
    std::vector<Cell> row;

    char ch = '\0';
    while (f.get(ch)) {
        if (ch == '\n') {
            grid.emplace_back(row);
            row.clear();
            continue;
        }

        row.emplace_back(Cell{ch, std::numeric_limits<int64_t>::max()});
    }
    grid.emplace_back(row);
    return grid;
}

const std::array<index_t, 4> dirs = {{index_t{-1, 0}, index_t{0, -1}, index_t{0, 1}, index_t{1, 0}}};
void search(matrix_t<Cell>& grid, index_t c, int64_t cost, index_t c_dir) {
    if (grid[c.i][c.j].ch == '#') return;
    if (grid[c.i][c.j].cost <= cost) return;

    grid[c.i][c.j].cost = cost;

    for (const auto& dir : dirs) {
        if (c_dir == dir) {
            search(grid, index_t{c.i + dir.i, c.j + dir.j}, cost + 1, dir);
        } else {
            search(grid, index_t{c.i + dir.i, c.j + dir.j}, cost + 1001, dir);
        }
    }
}

void count_best_tiles(matrix_t<Cell>& grid, index_t c, int64_t p_cost, int64_t pp_cost) {
    if (grid[c.i][c.j].ch == '#' or grid[c.i][c.j].ch == 'S') return;

    if ((p_cost - grid[c.i][c.j].cost == 1 or p_cost - grid[c.i][c.j].cost == 1001) or
        (pp_cost - grid[c.i][c.j].cost == 2)) {
        grid[c.i][c.j].best = true;
        for (const auto& dir : dirs) {
            count_best_tiles(grid, index_t{c.i + dir.i, c.j + dir.j}, grid[c.i][c.j].cost, p_cost);
        }
    }
}

int64_t solve(matrix_t<Cell>& grid) {
    const auto rows = static_cast<ssize_t>(grid.size());
    const auto cols = static_cast<ssize_t>(grid[0].size());

    index_t ridx{-1, -1}, eidx{-1, -1};
    for (ssize_t i = 0; i < rows; i++) {
        for (ssize_t j = 0; j < cols; j++) {
            if (grid[i][j].ch == 'S') {
                ridx.i = i;
                ridx.j = j;
            } else if (grid[i][j].ch == 'E') {
                eidx.i = i;
                eidx.j = j;
            }
        }
    }

    search(grid, ridx, 0, index_t{0, 1});
    for (const auto& dir : dirs) {
        count_best_tiles(grid, index_t{eidx.i + dir.i, eidx.j + dir.j}, grid[eidx.i][eidx.j].cost,
                         std::numeric_limits<int64_t>::max());
    }

    int64_t best_tiles = 2;
    for (ssize_t i = 0; i < rows; i++) {
        for (ssize_t j = 0; j < cols; j++) {
            if (grid[i][j].best) {
                best_tiles++;
            }
        }
    }

    return best_tiles;
}

int main() {
    static_assert(__cplusplus >= 201700L, "C++ version is not 17 or above");
    std::fstream f("input.txt", std::ios::in);

    if (!f.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    auto grid = read(f);
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << solve(grid) << "\n";
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Elapsed time: " << duration << " ms\n";
    return 0;
}
