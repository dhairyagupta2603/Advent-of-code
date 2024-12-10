#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

struct Index {
    ssize_t row;
    ssize_t col;
    char ch;

    explicit Index(const ssize_t& _row, const ssize_t& _col) : Index{_row, _col, '*'} {}

    Index(const ssize_t& _row, const ssize_t& _col, const char& _ch) : row(_row), col(_col), ch(_ch) {}
};

// Overloading the << operator
std::ostream& operator<<(std::ostream& os, const Index& idx) {
    os << "Index(row=" << idx.row << ", col=" << idx.col << ", ch=" << idx.ch << ")";
    return os;
}

const std::array<Index, 8> directions = {Index{-1, -1}, Index{-1, 0}, Index{-1, 1}, Index{0, -1},
                                         Index{0, 1},   Index{1, -1}, Index{1, 0},  Index{1, 1}};

void print(const Matrix<char>& grid) {
    for (const auto& row : grid) {
        for (const auto& elem : row) std::cout << elem;
        std::cout << "\n";
    }
}

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

bool is_valid(const ssize_t& row_size, const ssize_t& col_size, const long row, const long col) {
    return (row >= 0 and row < row_size) and (col >= 0 and col < col_size);
}

bool _dfs(const Matrix<char>& grid, const Index& curr, const Index& prev, const Index& dir, Matrix<char>& sgrid) {
    if (!is_valid(static_cast<ssize_t>(grid.size()), static_cast<ssize_t>(grid[0].size()), curr.row, curr.col)) {
        return false;
    }

    if (curr.ch == 'S' and prev.ch == 'A') {
        sgrid.at(curr.row).at(curr.col) = 'S';
        return true;
    }

    const ssize_t next_r = curr.row + dir.row;
    const ssize_t next_c = curr.col + dir.col;
    if (!is_valid(static_cast<ssize_t>(grid.size()), static_cast<ssize_t>(grid[0].size()), next_r, next_c)) {
        return false;
    }

    if (curr.ch == 'A' and prev.ch == 'M') {
        auto next_ch = grid.at(next_r).at(next_c);
        auto next_idx = Index{next_r, next_c, next_ch};
        bool res = _dfs(grid, next_idx, curr, dir, sgrid);
        if (res) sgrid.at(curr.row).at(curr.col) = 'A';
        return res;
    }

    if (curr.ch == 'M' and prev.ch == 'X') {
        char next_ch = grid[next_r][next_c];
        bool res = _dfs(grid, Index{next_r, next_c, next_ch}, curr, dir, sgrid);
        if (res) sgrid.at(curr.row).at(curr.col) = 'M';
        return res;
    }

    return false;
}

long long dfs(const Matrix<char>& grid, const Index& curr, Matrix<char>& sgrid) {
    long long res = 0;
    sgrid.at(curr.row).at(curr.col) = 'X';
    for (const auto& dir : directions) {
        const auto next_r = curr.row + dir.row;
        const auto next_c = curr.col + dir.col;
        if (!is_valid(static_cast<ssize_t>(grid.size()), static_cast<ssize_t>(grid[0].size()), next_r, next_c)) {
            continue;
        }

        const auto next = Index{next_r, next_c, grid.at(next_r).at(next_c)};
        res += (_dfs(grid, next, curr, dir, sgrid) ? 1 : 0);
    }
    if (res == 0) {
        sgrid[curr.row][curr.col] = '*';
    }

    return res;
}

long long solve(Matrix<char>& grid) {
    long long result = 0;
    const auto row_size = static_cast<ssize_t>(grid.size());
    const auto col_size = static_cast<ssize_t>(grid[0].size());

    Matrix<char> sgrid(row_size, std::vector<char>(col_size, '*'));

    for (auto i = 0; i < row_size; i++) {
        for (auto j = 0; j < col_size; j++) {
            if (grid[i][j] == 'X') {
                auto idx = Index{i, j, 'X'};
                result += dfs(grid, idx, sgrid);
                std::cout << "Running result : " << result << " for " << idx << "\n";
            }
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

    auto grid = read(f);
    // print(grid);
    std::cout << solve(grid) << "\n";
    return 0;
}
