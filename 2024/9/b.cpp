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

struct Chunk {
    std::vector<int> data;
    ssize_t free_start;

    explicit Chunk(const std::vector<int>&& _data) : Chunk{std::move(_data), true} {}
    Chunk(const std::vector<int>&& _data, const bool _empty) : data(_data) {
        free_start = _empty ? 0 : static_cast<ssize_t>(data.size());
    }

    [[nodiscard]] ssize_t ssize() const { return static_cast<ssize_t>(data.size()); }
    [[nodiscard]] ssize_t free_ssize() const { return ssize() - free_start; }
    [[nodiscard]] bool is_empty() const { return free_start < ssize(); }
};

auto read(std::fstream& f) {
    char ch = '\0';
    std::vector<int> disk_map;
    while (f.get(ch)) {
        disk_map.emplace_back(static_cast<int>(ch - '0'));
    }
    return disk_map;
}

std::vector<int> compact(std::vector<Chunk>& chunks) {
    const auto insert_to = [&chunks](const long dest, const long source) -> void {
        for (auto i = 0; i < chunks[source].ssize(); i++) {
            std::swap(chunks[dest].data[chunks[dest].free_start + i], chunks[source].data[i]);
        }
        chunks[dest].free_start += chunks[source].ssize();
        chunks[source].free_start = 0;
    };

    const auto num_chunks = static_cast<ssize_t>(chunks.size());
    for (auto i = 0; i < num_chunks; i++) {
        if (!chunks[i].is_empty()) continue;

        // if free block available, check backwords until filled
        for (auto j = num_chunks - 1; j > i; j--) {
            if (chunks[j].is_empty()) continue;

            if (chunks[j].ssize() > chunks[i].free_ssize()) continue;
            insert_to(i, j);
        }
    }

    std::vector<int> compacted;
    for (const auto& chunk : chunks) {
        for (const auto elem : chunk.data) {
            if (elem == -1) {
                compacted.emplace_back(0);
                continue;
            }
            compacted.emplace_back(elem);
        }
    }

    return compacted;
}

long long solve(const std::vector<int>& disk_map) {
    const auto len = static_cast<ssize_t>(disk_map.size());

    // expanded disk chunked map
    std::vector<Chunk> chunked_map;
    long file_idx = 0;
    for (auto i = 0; i < len; i++) {
        std::vector<int> expanded_map;
        const auto block_size = disk_map[i];

        if (i % 2 == 0) {  // if even, file
            for (auto j = 0; j < block_size; j++) expanded_map.emplace_back(file_idx);
            file_idx++;
            if (expanded_map.size() > 0) chunked_map.emplace_back(Chunk{std::move(expanded_map), false});
        } else {  // if odd, free space
            for (auto j = 0; j < block_size; j++) expanded_map.emplace_back(-1);
            if (expanded_map.size() > 0) chunked_map.emplace_back(Chunk{std::move(expanded_map), true});
        }
    }

    long long result = 0;
    const auto compacted_disk = compact(chunked_map);
    for (auto i = 0; i < static_cast<ssize_t>(compacted_disk.size()); i++) {
        result += (i * compacted_disk[i]);
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

    auto disk_map = read(f);

    auto start = std::chrono::high_resolution_clock::now();
    std::cout << solve(disk_map) << "\n";
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Elapsed time: " << duration << " ms\n";
    return 0;
}
