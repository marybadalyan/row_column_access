#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <numeric>
#include <algorithm>
#include "kaizen.h"

void row_major(const std::vector<int>& v, size_t row_count, size_t col_count) {
    volatile int sum = 0;  // Prevent compiler optimizations
    for (size_t i = 0; i < row_count; ++i) {
        for (size_t j = 0; j < col_count; ++j) {
            sum += v[i * col_count + j];
        }
    }
}

void col_major(const std::vector<int>& v, size_t row_count, size_t col_count) {
    volatile int sum = 0;  // Prevent compiler optimizations
    for (size_t j = 0; j < col_count; ++j) {
        for (size_t i = 0; i < row_count; ++i) {
            sum += v[i * col_count + j];
        }
    }
}

std::pair<size_t, size_t> process_args(int argc, char* argv[]) {
    zen::cmd_args args(argv, argc);

    auto col_options = args.get_options("--cols");
    auto row_options = args.get_options("--rows");

    if (col_options.empty() || row_options.empty()) {
        zen::log("Error: --cols and --rows arguments are absent, using default 3000x3000!");
        return {3000, 3000};
    }

    return {static_cast<size_t>(std::atoi(row_options[0].c_str())),
            static_cast<size_t>(std::atoi(col_options[0].c_str()))};
}

void benchmark(size_t row_count, size_t col_count, const std::string& label) {
    std::vector<int> matrix(row_count * col_count, 1);

    const int iter = 20;
    auto row_major_time = 0LL;
    auto col_major_time = 0LL;

    for (int i = 0; i < iter; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        row_major(matrix, row_count, col_count);
        auto end = std::chrono::high_resolution_clock::now();
        row_major_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

        start = std::chrono::high_resolution_clock::now();
        col_major(matrix, row_count, col_count);
        end = std::chrono::high_resolution_clock::now();
        col_major_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    }

    // Print the benchmark results
    std::cout << "| " << std::setw(10) << label
              << " | " << std::setw(5) << row_count << " x " << std::setw(5) << col_count
              << " | " << std::setw(8) << row_major_time / iter << " ns"
              << " | " << std::setw(8) << col_major_time / iter << " ns  |\n";
}

int main(int argc, char* argv[]) {
    auto [default_rows, default_cols] = process_args(argc, argv);

    std::cout << "\n+----------------------------------------------------------+\n";
    std::cout << "|      Type      |  Rows x Cols  |  Row Major   |  Col Major   |\n";
    std::cout << "+----------------------------------------------------------+\n";

    // Default benchmark (user-defined or default size)
    benchmark(default_rows, default_cols, "User-Defined");

    // Non-cache-aligned benchmark (not a multiple of 4)
    benchmark(1025, 1025, "Non-Aligned");
    // Cache-aligned benchmark (multiples of 4)
    benchmark(1024, 1024, "Cache-Aligned");
    

    std::cout << "+----------------------------------------------------------+\n";
    return 0;
}
