#include <algorithm>
#include <array>
#include <cassert>
#include <complex>
#include <fstream>
#include <iostream>
#include <stack>
#include <tuple>
#include <unordered_set>
#include <vector>

struct Direction {
    bool top = false, bottom = false, left = false, right = false;

    void update(const long i, const long j) {
        assert((i == -1 or i == 1 or i == 0) and "invalid i direction");
        assert((j == -1 or j == 1 or j == 0) and "invalid j direction");

        if (i == -1 and j == 0)
            top = true;
        else if (i == 1 and j == 0)
            bottom = true;
        else if (i == 0 and j == -1)
            left = true;
        else if (i == 0 and j == 1)
            right = true;
    }

    [[nodiscard]] bool visited(const long i, const long j) const {
        assert((i == -1 or i == 1 or i == 0) and "invalid i direction");
        assert((j == -1 or j == 1 or j == 0) and "invalid j direction");

        return (i == -1 and j == 0 and top) or (i == 1 and j == 0 and bottom) or (i == 0 and j == -1 and left) or
               (i == 0 and j == 1 and right);
    }
};

struct Index {
    long i, j;
    char ch;
    Direction in_dirs;

    Index(const long _i, const long _j) : Index{_i, _j, '.'} {}
    Index(const long _i, const long _j, const char _ch) : i(_i), j(_j), ch(_ch), in_dirs() {}

    bool operator==(const Index& other) { return other.ch == ch and other.i == i and other.j == j; }
    bool operator!=(const Index& other) { return !(other.ch == ch and other.i == i and other.j == j); }
};

template <typename T>
using Matrix = std::vector<std::vector<T>>;

std::ostream& operator<<(std::ostream& os, const Index& idx) {
    os << "Index(row=" << idx.i << ", col=" << idx.j << ", in_dir=(";
    os << "top: " << idx.in_dirs.top << ", ";
    os << "bottom: " << idx.in_dirs.bottom << ", ";
    os << "left: " << idx.in_dirs.left << ", ";
    os << "right: " << idx.in_dirs.right << ")";
    return os;
}

/* helper functions */

void print(const Matrix<Index>& m) {
    for (const auto& row : m) {
        for (const auto& elem : row) {
            std::cout << elem.ch;
        }
        std::cout << "\n";
    }
}

auto read(std::fstream& f) {
    char ch = '\0';
    int i = 0, j = 0;
    Matrix<Index> grid;
    std::vector<Index> row;
    while (f.get(ch)) {
        if (ch == '\n') {
            grid.push_back(row);
            row.clear();
            i++;
            j = 0;
            continue;
        }

        row.emplace_back(i, j, ch);
        j++;
    }
    grid.push_back(row);
    return grid;
}

std::tuple<long, long> rotate_right(const long i_dir, const long j_dir) {
    const std::complex<long> start(-i_dir, j_dir);
    constexpr std::complex<long> right_rotator(0, 1);
    const std::complex<long> end = start * right_rotator;
    return {-end.real(), end.imag()};
}

template <typename T>
bool valid_index(Matrix<T>& grid, const ssize_t& i, const ssize_t& j) {
    const auto max_row = static_cast<ssize_t>(grid.size());
    const auto max_col = static_cast<ssize_t>(grid[0].size());
    return (i >= 0 and i < max_row) and (j >= 0 and j < max_col);
}

/* functions */

// travel the path and get the vector of indexes traveled
void get_path(Matrix<Index>& grid, ssize_t i, ssize_t j, const long i_dir, const long j_dir, std::vector<Index>& path) {
    if (!valid_index(grid, i, j)) return;

    if (grid[i][j].ch == '#') {
        const auto& [new_i_dir, new_j_dir] = rotate_right(i_dir, j_dir);
        return get_path(grid, i - i_dir, j - j_dir, new_i_dir, new_j_dir, path);
    }

    if (path.size() == 0 or path.at(path.size() - 1) != Index{i, j, 'X'}) {
        path.emplace_back(Index{i, j, 'X'});
    }

    get_path(grid, i + i_dir, j + j_dir, i_dir, j_dir, path);
}

void traverse(Matrix<Index> grid, ssize_t i, ssize_t j, std::vector<Index>& path) {
    get_path(grid, i - 1, j + 0, -1, 0, path);
}

bool check_loop(Matrix<Index>& grid, ssize_t i, ssize_t j, const long i_dir, const long j_dir) {
    if (!valid_index(grid, i, j)) return false;

    if (grid[i][j].ch == '#') {
        if (grid[i][j].in_dirs.visited(i_dir, j_dir)) return true;

        grid[i][j].in_dirs.update(i_dir, j_dir);
        const auto& [new_i_dir, new_j_dir] = rotate_right(i_dir, j_dir);
        return check_loop(grid, i - i_dir, j - j_dir, new_i_dir, new_j_dir);
    }

    return check_loop(grid, i + i_dir, j + j_dir, i_dir, j_dir);
}

bool loops(Matrix<Index> grid, ssize_t i, ssize_t j, Index idx) {
    grid[idx.i][idx.j].ch = '#';
    if (idx.i == i and idx.j == j) return false;

    return check_loop(grid, i, j, -1, 0);
}

long long solve(Matrix<Index> grid) {
    const auto max_row = static_cast<ssize_t>(grid.size());
    const auto max_col = static_cast<ssize_t>(grid[0].size());

    std::function<bool(const Index&, const Index&)> equal = [](const Index& lhs, const Index& rhs) {
        return std::tie(lhs.i, lhs.j) == std::tie(rhs.i, rhs.j);
    };

    std::function<std::size_t(const Index&)> hash = [](const Index& idx) {
        std::size_t h1 = std::hash<long>()(idx.i);
        std::size_t h2 = std::hash<long>()(idx.j);
        return h1 ^ (h2 << 1);
    };

    // find start position and travel path
    long si = 0, sj = 0;
    std::vector<Index> path;
    for (ssize_t i = 0; i < max_row; i++) {
        for (ssize_t j = 0; j < max_col; j++) {
            if (grid[i][j].ch == '^') {
                traverse(grid, i, j, path);
                si = i;
                sj = j;
                break;
            }
        }

        if (!path.empty()) break;
    }

    long long result = 0;
    std::unordered_set<Index, decltype(hash), decltype(equal)> set_path(0, hash, equal);
    for (const auto& idx : path) set_path.insert(idx);
    for (const auto& idx : set_path) result += loops(grid, si, sj, idx);
    return result;
}

int main() {
    static_assert(__cplusplus >= 201700L, "C++ version is not 17 or above");
    std::fstream f("input.txt", std::ios::in);
    if (!f.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    const auto grid = read(f);
    std::cout << solve(grid) << "\n";
    return 0;
}
