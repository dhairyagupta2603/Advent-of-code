#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

void print(const Matrix<char>& grid) {
    for (const auto& row : grid) {
        for (const auto& elem : row) {
            std::cout << elem;
        }
        std::cout << "\n";
    }
    std::cout << "---------------------------------------------\n";
}

struct Vec2D {
    int64_t i{0};
    int64_t j{0};

    [[nodiscard]] Vec2D next(const char& ch) {
        Vec2D next;
        if (ch == '<')
            next = Vec2D{i, j - 1};
        else if (ch == '^')
            next = Vec2D{i - 1, j};
        else if (ch == '>')
            next = Vec2D{i, j + 1};
        else if (ch == 'v')
            next = Vec2D{i + 1, j};

        // std::cout << ch << ": " << next;
        return next;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vec2D& vec) {
        os << "Vec2D(i=" << vec.i << ", j=" << vec.j << ")";
        return os;
    }
};

struct Bot {
    Vec2D pos;

    Bot(const int64_t& _i, const int64_t _j) {
        pos.i = _i;
        pos.j = _j;
    }

    void move(const int64_t& i, const int64_t j) {
        pos.i = i;
        pos.j = j;
    }
};

auto read_grid(std::fstream& f) {
    Matrix<char> grid;

    std::vector<char> row;
    char ch = '\0';
    while (f.get(ch)) {
        if (ch == '\n') {
            grid.emplace_back(row);
            row.clear();
            continue;
        }
        row.emplace_back(ch);
    }
    grid.emplace_back(row);

    return grid;
}

auto read_moveset(std::fstream& f) {
    std::vector<char> moves;
    char ch = '\0';
    while (f.get(ch)) {
        if (ch == '\n') continue;
        moves.emplace_back(ch);
    }

    return moves;
}

long solve(Matrix<char>& grid, const std::vector<char>& moves) {
    const auto rows = static_cast<ssize_t>(grid.size());
    const auto cols = static_cast<ssize_t>(grid[0].size());

    const auto get_bot = [&grid, &rows, &cols]() -> Bot {
        for (ssize_t i = 0; i < rows; i++) {
            for (ssize_t j = 0; j < cols; j++) {
                if (grid[i][j] != '@') continue;
                return Bot{i, j};
            }
        }
    };

    const auto move_bot = [&grid](Bot& bot, const int64_t ni, const int64_t nj) -> void {
        grid[bot.pos.i][bot.pos.j] = '.';
        bot.move(ni, nj);
        grid[ni][nj] = '@';
    };

    // get bot position
    Bot bot = get_bot();
    for (const auto& move : moves) {
        const auto next = bot.pos.next(move);
        // std::cout << "\t" << grid[next.i][next.j] << "\n";

        // wall
        if (grid[next.i][next.j] == '#') continue;

        // empty
        if (grid[next.i][next.j] == '.') {
            move_bot(bot, next.i, next.j);
            continue;
        }

        // box - check if boxes can be moved in grid
        int64_t ni = next.i;
        int64_t nj = next.j;
        while (grid[ni][nj] != '.' and grid[ni][nj] != '#') {
            Vec2D&& npos = Vec2D{ni, nj}.next(move);
            ni = npos.i;
            nj = npos.j;
        }

        if (grid[ni][nj] == '.') {
            grid[ni][nj] = 'O';
            move_bot(bot, next.i, next.j);
        }
    }

    // get gps
    int result{0};
    for (ssize_t i = 0; i < rows; i++) {
        for (ssize_t j = 0; j < cols; j++) {
            if (grid[i][j] == 'O') {
                result += (100 * i + j);
            }
        }
    }
    return result;
}

int main() {
    static_assert(__cplusplus >= 201700L, "C++ version is not 17 or above");
    std::fstream fa("inputa.txt", std::ios::in);
    if (!fa.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    std::fstream fb("inputb.txt", std::ios::in);
    if (!fb.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    auto grid = read_grid(fa);
    auto moves = read_moveset(fb);

    auto start = std::chrono::high_resolution_clock::now();
    std::cout << solve(grid, moves) << "\n";
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Elapsed time: " << duration << " μs\n";
    return 0;
}
