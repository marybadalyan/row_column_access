#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>
#include <iomanip>
#include <numeric>
#include <algorithm>
#include "kaizen.h"

void row_major(const std::vector<std::vector<int>>& v) {
    volatile int sum = 0;
    for (size_t i = 0; i < v.size(); i++) {
        for (size_t j = 0; j < v[i].size(); j++) {
            sum += v[i][j];  // Row-wise access
        }
    }
}

void col_major(const std::vector<std::vector<int>>& v) {
    volatile int sum = 0;
    if (v.empty() || v[0].empty()) return;  // Avoid out-of-bounds errors

    for (size_t j = 0; j < v[0].size(); ++j) {  // First iterate columns
        for (size_t i = 0; i < v.size(); ++i) { // Then iterate rows
            sum += v[i][j];  // Column-wise access
        }
    }
}

int main(int argc, char* argv[]) {
    zen::cmd_args args(argv, argc);

    auto col_options = args.get_options("--cols");
    auto row_options = args.get_options("--rows");

    if (col_options.empty() || row_options.empty()) {
        std::cerr << "Error: --cols and --rows arguments are required!" << std::endl;
        return 1;
    }

    size_t col_count = static_cast<size_t>(std::atoi(col_options[0].c_str()));
    size_t row_count = static_cast<size_t>(std::atoi(row_options[0].c_str()));

    std::vector<std::vector<int>> matrix(row_count, std::vector<int>(col_count, 1));

    auto start = std::chrono::high_resolution_clock::now();
    row_major(matrix);
    auto end = std::chrono::high_resolution_clock::now();
    auto row_major_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    col_major(matrix);
    end = std::chrono::high_resolution_clock::now();
    auto col_major_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // Print the result in table format
    std::cout << "\n+----------------------------------------+\n";
    std::cout << "|  Rows x Cols | Row Major  | Col Major  |\n";
    std::cout << "+----------------------------------------+\n";
    std::cout << "| " << std::setw(4) << row_count << " x " << std::setw(4) << col_count
              << "  | " << std::setw(8) << row_major_time << "ms"
              << " | " << std::setw(7) << col_major_time << "ms  |\n";
    std::cout << "+----------------------------------------+\n";

    return 0;
}
