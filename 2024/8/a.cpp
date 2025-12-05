#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <complex>
#include <fstream>
#include <iostream>
#include <new>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

struct Index {
    std::complex<long> pos;
    char ch = '.';

    Index(const long& _i, const long _j) : Index{_i, _j, '.'} {}
    Index(const std::complex<long> _pos) : Index{_pos.real(), _pos.imag(), '.'} {}
    Index(const long& _i, const long _j, const char _ch) : pos(_i, _j), ch(_ch) {}

    [[nodiscard]] long i() const { return pos.real(); }
    [[nodiscard]] long j() const { return pos.imag(); }
};

std::ostream& operator<<(std::ostream& os, const Index& idx) {
    os << "Index(row=" << idx.i() << ", col=" << idx.j() << ", ch=" << idx.ch << ")";
    return os;
}

struct Anti_Node {
    Index antenna1, antenna2;
    Index idx;

    Anti_Node(const Index& _antenna1, const Index& _antenna2, const Index& _idx)
        : antenna1(_antenna1), antenna2(_antenna2), idx(_idx) {}
};

std::ostream& operator<<(std::ostream& os, const Anti_Node& an) {
    os << "Anti_Node(row=" << an.idx.i() << ", col=" << an.idx.j() << ")\n\t";
    os << an.antenna1 << "\n\t" << an.antenna2;
    return os;
}

template <typename T>
using Matrix = std::vector<std::vector<T>>;

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

void print(const Matrix<Index>& grid) {
    for (const auto& row : grid) {
        for (const auto& elem : row) {
            std::cout << elem.ch;
        }
        std::cout << "\n";
    }
}

template <typename T>
bool valid_index(const Matrix<T>& grid, const long& i, const long& j) {
    const auto max_row = static_cast<ssize_t>(grid.size());
    const auto max_col = static_cast<ssize_t>(grid[0].size());
    return (i >= 0 and i < max_row) and (j >= 0 and j < max_col);
}

std::vector<Anti_Node> get_anti_nodes(const Matrix<Index>& grid, const Index& a, const Index& b) {
    // distance vector from a to b
    const std::complex<long> complex_vec(b.i() - a.i(), b.j() - a.j());

    // possible antinodes
    const auto an1 = b.pos + complex_vec;
    const auto an2 = a.pos - complex_vec;

    // check if in grid
    std::vector<Anti_Node> anti_nodes;
    if (valid_index(grid, an1.real(), an1.imag())) {
        anti_nodes.emplace_back(Anti_Node{a, b, Index{an1}});
    }
    if (valid_index(grid, an2.real(), an2.imag())) {
        anti_nodes.emplace_back(Anti_Node{a, b, Index{an2}});
    }

    return anti_nodes;
}

long solve(const Matrix<Index>& grid) {
    const auto max_row = static_cast<ssize_t>(grid.size());
    const auto max_col = static_cast<ssize_t>(grid[0].size());

    // get all antennas
    std::vector<Index> antennas;
    for (auto i = 0; i < max_row; i++) {
        for (auto j = 0; j < max_col; j++) {
            if (grid[i][j].ch == '.') continue;
            antennas.emplace_back(grid[i][j]);
        }
    }

    // calculate antinodes
    std::function<bool(const Anti_Node&, const Anti_Node&)> equal = [](const Anti_Node& a, const Anti_Node& b) {
        return a.idx.i() == b.idx.i() && a.idx.j() == b.idx.j();
    };

    std::function<std::size_t(const Anti_Node&)> hash = [](const Anti_Node& anti_node) {
        std::size_t h1 = std::hash<long>()(anti_node.idx.i());
        std::size_t h2 = std::hash<long>()(anti_node.idx.j());
        return h1 ^ (h2 << 1);
    };

    std::unordered_set<Anti_Node, decltype(hash), decltype(equal)> anti_node_set(0, hash, equal);
    const auto antennas_len = static_cast<ssize_t>(antennas.size());
    for (auto i = 0; i < antennas_len - 1; i++) {
        for (auto j = i + 1; j < antennas_len; j++) {
            if (antennas[i].ch != antennas[j].ch) continue;

            const auto anti_nodes = get_anti_nodes(grid, antennas[i], antennas[j]);
            for (const auto& an : anti_nodes) {
                anti_node_set.insert(an);
            }
        }
    }

    return static_cast<ssize_t>(anti_node_set.size());
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
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Elapsed time: " << duration << " μs\n";
    return 0;
}
