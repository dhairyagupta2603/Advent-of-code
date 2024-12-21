#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

struct Button {
    int32_t cost{0};
    int64_t x_move{0};
    int64_t y_move{0};
};

struct Prize {
    int64_t x{0};
    int64_t y{0};
};

struct Game {
    Button a;
    Button b;
    Prize prize;

    friend std::ostream& operator<<(std::ostream& os, const Game& game) {
        os << "Button A: x_move=" << game.a.x_move << ", y_move=" << game.a.y_move << "\n";
        os << "Button B: x_move=" << game.b.x_move << ", y_move=" << game.b.y_move << "\n";
        os << "Prize: x=" << game.prize.x << ", y=" << game.prize.y << "\n";
        return os;
    }
};

auto read(std::fstream& f) {
    std::vector<Game> games;
    std::string line;
    while (std::getline(f, line)) {
        std::istringstream iss(line);
        Game game;
        if (line.find("Button A:") != std::string::npos) {
            iss.ignore(10, 'X');
            iss.ignore(1, '+');
            iss >> game.a.x_move;
            iss.ignore(3, 'Y');
            iss.ignore(1, '+');
            iss >> game.a.y_move;

            game.a.cost = 3;

            std::getline(f, line);
            iss.clear();
            iss.str(line);
            iss.ignore(10, 'X');
            iss.ignore(1, '+');
            iss >> game.b.x_move;
            iss.ignore(3, 'Y');
            iss.ignore(1, '+');
            iss >> game.b.y_move;

            game.b.cost = 3;

            std::getline(f, line);

            iss.clear();
            iss.str(line);
            iss.ignore(7, 'X');
            iss.ignore(1, '=');

            std::string prize_x;
            std::getline(iss, prize_x, ',');
            game.prize.x = std::stoll(prize_x);

            iss.ignore(2, 'Y');
            iss.ignore(1, '=');

            std::string prize_y;
            std::getline(iss, prize_y);
            game.prize.y = std::stoll(prize_y);
            games.push_back(game);
        }
    }
    return games;
}

std::pair<double, double> crammer(const Game& game) {
    const double ax = game.a.x_move;
    const double bx = game.b.x_move;
    const double ay = game.a.y_move;
    const double by = game.b.y_move;
    const double px = game.prize.x + 10'000'000'000'000;
    const double py = game.prize.y + 10'000'000'000'000;

    const double D = ax * by - ay * bx;
    const double Dx = px * by - bx * py;
    const double Dy = ax * py - px * ay;

    return {Dx / D, Dy / D};
}

uint64_t play(const Game& game) {
    const auto& [x, y] = crammer(game);
    std::cout << "x=" << x << ", y=" << y << "\n";

    if (static_cast<uint64_t>(x) == x and static_cast<uint64_t>(y) == y) {
        return 3 * static_cast<uint64_t>(x) + static_cast<uint64_t>(y);
    }
    return 0;
}

uint64_t solve(const std::vector<Game>& games) {
    uint64_t num_tokens{0};

    for (const auto& game : games) {
        std::cout << game;
        const uint64_t tokens = play(game);
        std::cout << "Play tokens: " << tokens << "\n-------------------------------------\n";
        if (tokens == 0) {
            continue;
        }
        num_tokens += tokens;
    }

    return num_tokens;
}

int main() {
    static_assert(__cplusplus >= 201700L, "C++ version is not 17 or above");
    std::fstream f("input.txt", std::ios::in);

    if (!f.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    auto games = read(f);
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << solve(games) << "\n";
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Elapsed time: " << duration << " μs\n";
    return 0;
}
