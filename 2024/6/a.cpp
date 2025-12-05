#include <algorithm>
#include <array>
#include <cassert>
#include <complex>
#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

auto read(std::fstream& f) {
    Matrix<char> grid;

    char ch = '\0';
    std::vector<char> row;
    while (f.get(ch)) {
        if (ch == '\n') {
            grid.push_back(row);
            row.clear();
            continue;
        }

        row.push_back(ch);
    }
    grid.push_back(row);
    return grid;
}

std::tuple<long, long> rotate_right(const long& i_dir, const long& j_dir) {
    const std::complex<long> start(-i_dir, j_dir);
    const std::complex<long> right_rotator(0, 1);

    const std::complex<long> end = start * right_rotator;
    return {-end.real(), end.imag()};
}

bool valid_index(Matrix<char>& grid, const ssize_t& i, const ssize_t& j) {
    const auto max_row = static_cast<ssize_t>(grid.size());
    const auto max_col = static_cast<ssize_t>(grid[0].size());

    return (i >= 0 and i < max_row) and (j >= 0 and j < max_col);
}

int travel(Matrix<char>& grid, ssize_t i, ssize_t j, const long& i_dir, const long& j_dir) {
    if (!valid_index(grid, i, j)) {
        return 0;
    }

    if (grid[i][j] == '#') {
        const auto& [new_i_dir, new_j_dir] = rotate_right(i_dir, j_dir);
        return travel(grid, i - i_dir, j - j_dir, new_i_dir, new_j_dir);
    }

    int unique = (grid[i][j] == 'X' ? 0 : 1);
    grid[i][j] = 'X';

    return unique + travel(grid, i + i_dir, j + j_dir, i_dir, j_dir);
}

long long solve(Matrix<char> grid) {
    const auto max_row = static_cast<ssize_t>(grid.size());
    const auto max_col = static_cast<ssize_t>(grid[0].size());

    // find groud position and direction
    for (ssize_t i = 0; i < max_row; i++) {
        for (ssize_t j = 0; j < max_col; j++) {
            if (grid[i][j] == '^') {
                return travel(grid, i - 1, j + 0, -1, 0);
            }
        }
    }

    return -1;
}

int main() {
    static_assert(__cplusplus >= 201700L, "C++ version is not 17 or above");
    std::fstream f("6/input.txt", std::ios::in);

    if (!f.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    auto grid = read(f);
    std::cout << solve(grid) << "\n";
    return 0;
}
