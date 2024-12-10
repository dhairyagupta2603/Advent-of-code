#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <span>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

template <typename T, ssize_t N>
using Kernel = std::array<std::array<T, N>, N>;

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

Kernel<char, 3> rotate_right(const Kernel<char, 3>& kernel) {
    const auto transpose = [](const Kernel<char, 3>& kernel) -> Kernel<char, 3> {
        return {{
            {{kernel[0][0], kernel[1][0], kernel[2][0]}},
            {{kernel[0][1], kernel[1][1], kernel[2][1]}},
            {{kernel[0][2], kernel[1][2], kernel[2][2]}},
        }};
    };

    const auto reverse_rows = [](const Kernel<char, 3>& kernel) -> Kernel<char, 3> {
        return {{
            {{kernel[0][2], kernel[0][1], kernel[0][0]}},
            {{kernel[1][2], kernel[1][1], kernel[1][0]}},
            {{kernel[2][2], kernel[2][1], kernel[2][0]}},
        }};
    };

    return reverse_rows(transpose(kernel));
}

bool match(const Matrix<char>& grid, const long rb, const long cb, const std::array<Kernel<char, 3>, 4>& kernels) {
    const auto match_kernel = [&grid, rb, cb](const Kernel<char, 3>& k) {
        for (long i = 0; i < 3; i++) {
            for (long j = 0; j < 3; j++) {
                if (k[i][j] == '.') continue;
                if (k[i][j] != grid[rb + i][cb + j]) return false;
            }
        }

        return true;
    };

    bool flag = false;
    for (const auto& kernel : kernels) {
        flag = flag or match_kernel(kernel);
    }

    return flag;
}

long long solve(Matrix<char>& grid) {
    long long result = 0;
    const auto row_size = static_cast<ssize_t>(grid.size());
    const auto col_size = static_cast<ssize_t>(grid[0].size());

    std::array<Kernel<char, 3>, 4> kernels;
    kernels[0] = {{{{'M', '.', 'S'}}, {{'.', 'A', '.'}}, {{'M', '.', 'S'}}}};
    kernels[1] = rotate_right(kernels[0]);
    kernels[2] = rotate_right(kernels[1]);
    kernels[3] = rotate_right(kernels[2]);

    for (auto i = 1; i < row_size - 1; i++) {
        for (auto j = 1; j < col_size - 1; j++) {
            if (grid[i][j] == 'A') {
                result += (match(grid, i - 1, j - 1, kernels));
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
    std::cout << solve(grid) << "\n";
    return 0;
}
