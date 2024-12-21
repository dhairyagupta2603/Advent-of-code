#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>

const int32_t Y_MAX = 103;
const int32_t X_MAX = 101;

struct Robot {
    struct Vec2D {
        int64_t x;
        int64_t y;
    };

    Vec2D p;
    Vec2D v;

    [[nodiscard]] std::pair<int64_t, int64_t> teleport(const int64_t time) const {
        auto wrap = [](int64_t value, int64_t max) -> int64_t {
            return ((value % max) + max) % max;  // Ensure result is within [0, max)
        };

        int64_t new_x = wrap(p.x + v.x * time, X_MAX);
        int64_t new_y = wrap(p.y + v.y * time, Y_MAX);

        return {new_x, new_y};
    }

    friend std::ostream& operator<<(std::ostream& os, const Robot& robot) {
        os << "Robot: pos=(" << robot.p.x << ", " << robot.p.y << "), ";
        os << "vel=(" << robot.v.x << ", " << robot.v.y << ")";
        return os;
    }
};

struct Quadrant {
    int32_t min_x;
    int32_t max_x;
    int32_t min_y;
    int32_t max_y;
    std::string name;

    Quadrant(int32_t x0, int32_t x1, int32_t y0, int32_t y1, std::string _name)
        : min_x(x0), max_x(x1), min_y(y0), max_y(y1), name{_name} {}

    [[nodiscard]] bool is_inside(const int64_t px, const int64_t py) const {
        return px >= min_x and px < max_x and py >= min_y and py < max_y;
    }
};

auto read(std::fstream& f) {
    std::vector<Robot> robots;
    std::string line;
    while (std::getline(f, line)) {
        std::istringstream iss(line);
        Robot robot;
        if (line.find("p=") != std::string::npos) {
            iss.ignore(2, 'p');
            iss.ignore(1, '=');
            iss >> robot.p.x;
            iss.ignore(1, ',');
            iss >> robot.p.y;

            iss.ignore(2, 'v');
            iss.ignore(1, '=');
            iss >> robot.v.x;
            iss.ignore(1, ',');
            iss >> robot.v.y;

            robots.push_back(robot);
        }
    }
    return robots;
}

int64_t solve(const std::vector<Robot>& robots) {
    std::function<bool(const Quadrant&, const Quadrant&)> equal = [](const Quadrant& a, const Quadrant& b) {
        return a.min_x == b.min_x and a.max_x == b.max_x and a.min_y == b.min_y and a.max_y == b.max_y;
    };

    std::function<std::size_t(const Quadrant&)> hash = [](const Quadrant& q) {
        std::size_t h1 = std::hash<long>()(q.min_x);
        std::size_t h2 = std::hash<long>()(q.max_x);
        std::size_t h3 = std::hash<long>()(q.min_y);
        std::size_t h4 = std::hash<long>()(q.max_y);
        return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
    };

    std::unordered_map<Quadrant, int32_t, decltype(hash), decltype(equal)> qf(0, hash, equal);
    qf = {
        {Quadrant(0, (X_MAX - 1) / 2, 0, (Y_MAX - 1) / 2, "first"), 0},
        {Quadrant((X_MAX + 1) / 2, X_MAX, 0, (Y_MAX - 1) / 2, "second"), 0},
        {Quadrant(0, (X_MAX - 1) / 2, (Y_MAX + 1) / 2, Y_MAX, "third"), 0},
        {Quadrant((X_MAX + 1) / 2, X_MAX, (Y_MAX + 1) / 2, Y_MAX, "fourth"), 0},
    };

    const int64_t simulate_time = 100;  // in sec
    for (const auto& robot : robots) {
        const auto [px, py] = robot.teleport(simulate_time);
        for (auto& [quad, _] : qf) {
            if (quad.is_inside(px, py)) {
                qf[quad]++;
            }
        }
    }

    int64_t result = 1;
    for (const auto& [q, freq] : qf) {
        result *= freq;
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

    auto robots = read(f);
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << solve(robots) << "\n";
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Elapsed time: " << duration << " μs\n";
    return 0;
}
