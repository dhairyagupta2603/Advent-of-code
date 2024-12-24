#include <chrono>
#include <deque>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

const std::pair<int, int> NUM_POSITIONS[10] = {{3, 1}, {2, 0}, {2, 1}, {2, 2}, {1, 0},
                                               {1, 1}, {1, 2}, {0, 0}, {0, 1}, {0, 2}};
const std::pair<int, int> NUM_A_POS = {3, 2};
const int NUM_GAP_I = 3;
const int NUM_GAP_J = 0;

const std::pair<int, int> DIR_U_POS = {0, 1};
const std::pair<int, int> DIR_L_POS = {1, 0};
const std::pair<int, int> DIR_D_POS = {1, 1};
const std::pair<int, int> DIR_R_POS = {1, 2};
const std::pair<int, int> DIR_A_POS = {0, 2};
const int DIR_GAP_I = 0;
const int DIR_GAP_J = 0;

std::deque<std::pair<int, int>> goals;
std::deque<std::pair<int, int>> next_goals;
std::pair<int, int> curr = NUM_A_POS;

void add_next_goals(bool moving_numeral) {
    for (const auto& goal : goals) {
        const auto& [i, j] = curr;
        const auto& [ni, nj] = goal;
        const int di = ni - i, dj = nj - j;

        bool already_processed = false;
        // treat separately in cases where the gap needs to be avoided
        if (moving_numeral) {
            if (ni == NUM_GAP_I && j == NUM_GAP_J) {  // from top-left to bottom-right
                // right
                for (int c = 0; c < dj; c++) next_goals.push_back(DIR_R_POS);
                // down
                for (int c = 0; c < di; c++) next_goals.push_back(DIR_D_POS);
                already_processed = true;
            }
            if (i == NUM_GAP_I && nj == NUM_GAP_J) {  // from bottom-right to top-left
                // up
                for (int c = 0; c < -di; c++) next_goals.push_back(DIR_U_POS);
                // left
                for (int c = 0; c < -dj; c++) next_goals.push_back(DIR_L_POS);
                already_processed = true;
            }
        } else {
            if (i == NUM_GAP_I && nj == NUM_GAP_J) {  // from top-right to bottom-left
                // down
                for (int c = 0; c < di; c++) next_goals.push_back(DIR_D_POS);
                // left
                for (int c = 0; c < -dj; c++) next_goals.push_back(DIR_L_POS);
                already_processed = true;
            }
            if (ni == NUM_GAP_I && j == NUM_GAP_J) {  // from bottom-left to top-right
                // right
                for (int c = 0; c < dj; c++) next_goals.push_back(DIR_R_POS);
                // up
                for (int c = 0; c < -di; c++) next_goals.push_back(DIR_U_POS);
                already_processed = true;
            }
        }

        // otherwise, prioritize to minimize moves
        if (!already_processed) {
            const bool going_right = dj > 0;
            const bool going_up = di < 0;

            // go to the ones farthest away from A first
            if (!going_right) {
                for (int c = 0; c < -dj; c++) next_goals.push_back(DIR_L_POS);
            }
            if (!going_up) {
                for (int c = 0; c < di; c++) next_goals.push_back(DIR_D_POS);
            }

            // then the ones closest to A
            if (going_right) {
                for (int c = 0; c < dj; c++) next_goals.push_back(DIR_R_POS);
            }
            if (going_up) {
                for (int c = 0; c < -di; c++) next_goals.push_back(DIR_U_POS);
            }
        }

        next_goals.push_back(DIR_A_POS);
        curr = goal;
    }
}

int main() {
    static_assert(__cplusplus >= 201700L, "C++ version is not 17 or above");
    std::fstream f("input.txt", std::ios::in);

    if (!f.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    std::string line;
    int answer = 0;
    while (std::getline(f, line)) {
        goals.clear();
        next_goals.clear();
        curr = NUM_A_POS;

        for (char c : line) goals.push_back((c == 'A') ? NUM_A_POS : NUM_POSITIONS[c - '0']);

        add_next_goals(true);

        for (int c = 0; c < 2; c++) {
            curr = DIR_A_POS;
            goals = next_goals;
            next_goals.clear();

            add_next_goals(false);
        }

        answer += next_goals.size() * stoi(line.substr(0, 3));
    }

    std::cout << answer << '\n';

    auto start = std::chrono::high_resolution_clock::now();
    // std::cout << solve(combination_keys) << "\n";
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    std::cout << "Elapsed time: " << duration << " ns\n";
}