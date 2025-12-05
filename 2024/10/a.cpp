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

template <typename T>
using Matrix = std::vector<std::vector<T>>;

struct Point {
    int height{};
    bool is_visted{};

    explicit Point(const int _height) : Point{_height, false} {}
    Point(const int _height, const bool _visted) : height(_height), is_visted(_visted) {}
};

auto read(std::fstream& f) {
    Matrix<Point> grid;

    char ch = '\0';
    std::vector<Point> row;
    while (f.get(ch)) {
        if (ch == '\n') {
            grid.emplace_back(row);
            row.clear();
            continue;
        }

        row.emplace_back(Point{ch - '0'});
    }
    grid.push_back(row);
    return grid;
}

int hike(Matrix<Point>& hill, const long pi, const long pj) {
    const auto valid_index = [&hill](const long i, const long j) {
        const auto row_size = static_cast<ssize_t>(hill.size());
        const auto col_size = static_cast<ssize_t>(hill[0].size());
        return (i >= 0 and i < row_size) and (j >= 0 and j < col_size);
    };

    if (!valid_index(pi, pj)) return 0;

    if (hill[pi][pj].height == 9) {
        if (hill[pi][pj].is_visted) return 0;

        hill[pi][pj].is_visted = true;
        return 1;
    }

    int num_headtrails = 0;
    const std::array<std::pair<int, int>, 4> dirs = {{{-1, 0}, {0, -1}, {0, 1}, {1, 0}}};
    for (const auto& [i, j] : dirs) {
        if (!valid_index(pi + i, pj + j) or hill[pi + i][pj + j].height - hill[pi][pj].height != 1) continue;
        num_headtrails += hike(hill, pi + i, pj + j);
    }
    return num_headtrails;
}

int count_trailheads(Matrix<Point> hill, const long si, const long sj) {
    const auto res = hike(hill, si, sj);
    return res;
}

long long solve(const Matrix<Point>& hill) {
    const auto row_size = static_cast<ssize_t>(hill.size());
    const auto col_size = static_cast<ssize_t>(hill[0].size());

    long long result{0};
    for (auto i = 0; i < row_size; i++) {
        for (auto j = 0; j < col_size; j++) {
            if (hill[i][j].height != 0) continue;

            const auto headtrails = count_trailheads(hill, i, j);
            result += headtrails;
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

    auto hill = read(f);
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << solve(hill) << "\n";
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Elapsed time: " << duration << " μs\n";
    return 0;
}
