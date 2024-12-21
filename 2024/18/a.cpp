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

struct index_t {
    int32_t i;
    int32_t j;

    friend std::ostream& operator<<(std::ostream& os, const index_t& idx) {
        os << "Index(i=" << idx.i << ", j=" << idx.j << ")";
        return os;
    }

    template <typename T, size_t N>
    bool is_valid(const std::array<std::array<T, N>, N>& grid) const {
        const auto rows = static_cast<ssize_t>(grid.size());
        const auto cols = static_cast<ssize_t>(grid[0].size());

        return i >= 0 and i < rows and j >= 0 and j < cols;
    }
};

struct cell_t {
    char ch;
    int64_t cost;
};

void print(const std::vector<index_t>& indicies) {
    for (const auto& idx : indicies) {
        std::cout << idx << "\n";
    }
}

template <size_t N>
void print(std::array<std::array<cell_t, N>, N> grid) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = i + 1; j < N; ++j) {
            std::swap(grid[i][j], grid[j][i]);
        }
    }

    for (auto row : grid) {
        for (const auto& elem : row) {
            std::cout << elem.ch;
        }
        std::cout << "\n";
    }
}

template <size_t N>
void print(std::array<std::array<cell_t, N>, N> grid, bool cost) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = i + 1; j < N; ++j) {
            std::swap(grid[i][j], grid[j][i]);
        }
    }

    for (auto row : grid) {
        for (const auto& elem : row) {
            if (elem.ch == '#' or elem.cost == std::numeric_limits<int64_t>::max()) {
                std::cout << elem.ch;
            } else {
                std::cout << elem.cost;
            }
            std::cout << "\t";
        }
        std::cout << "\n";
    }
}

auto read(std::fstream& f) {
    std::vector<index_t> indicies;

    std::string line;
    while (std::getline(f, line)) {
        std::istringstream iss(line);

        index_t idx;
        iss >> idx.i;
        iss.ignore(1, ' ');
        iss >> idx.j;
        indicies.emplace_back(idx);
    }

    return indicies;
}

const std::array<index_t, 4> dirs = {{index_t{-1, 0}, index_t{0, -1}, index_t{0, 1}, index_t{1, 0}}};

template <size_t N>
void search(std::array<std::array<cell_t, N>, N>& grid, index_t c, int64_t cost) {
    if (!c.is_valid(grid) or grid[c.i][c.j].ch == '#') return;

    if (grid[c.i][c.j].cost <= cost) return;
    grid[c.i][c.j].cost = cost;

    for (const auto& dir : dirs) {
        search(grid, index_t{c.i + dir.i, c.j + dir.j}, cost + 1);
    }
}

int64_t solve(std::vector<index_t> indicies) {
    const size_t dim = 71;
    const ssize_t num_bytes = 1024;

    std::array<std::array<cell_t, dim>, dim> grid;
    for (auto& row : grid) {
        row.fill(cell_t{'.', std::numeric_limits<int64_t>::max()});
    }

    for (ssize_t i = 0; i < num_bytes; i++) {
        grid[indicies[i].i][indicies[i].j].ch = '#';
    }
    search(grid, index_t{0, 0}, 0);
    return grid[dim - 1][dim - 1].cost;
}

int main() {
    static_assert(__cplusplus >= 201700L, "C++ version is not 17 or above");
    std::fstream f("input.txt", std::ios::in);

    if (!f.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    auto indicies = read(f);
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << solve(indicies) << "\n";
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "\nElapsed time: " << duration << " ms\n";
    return 0;
}
