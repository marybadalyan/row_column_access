#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <numeric>
#include <algorithm>
#include "kaizen.h"

void row_major(const std::vector<int>& v,size_t row_count,size_t col_count) {
    volatile int sum = 0;       // Prevent loop optimization for benchmarking
    for(size_t i = 0; i < row_count;++i){
        for (size_t j = 0; j < col_count; ++j) {  // First iterate columns
        sum += v[i*col_count + j];  // Column-wise access
        }
    }
}

void col_major(const std::vector<int>& v,size_t row_count,size_t col_count) {
    volatile int sum = 0;   // Prevent loop optimization for benchmarking
    for(size_t j = 0; j < col_count;++j){
        for (size_t i = 0; i < row_count; ++i) {  // First iterate columns
        sum += v[i*col_count + j];  // Column-wise access
        }
    }
}
    
std::pair<size_t,size_t> process_args(int argc, char* argv[]) {
    zen::cmd_args args(argv, argc);
 
    auto col_options = args.get_options("--cols");
    auto row_options = args.get_options("--rows");

    if (col_options.empty() || row_options.empty()) {
        zen::log("Error: --cols and --rows arguments are abscent using default 3000x3000!");
        return {3000, 3000};
    }

    return {static_cast<size_t>(std::atoi(col_options[0].c_str())),static_cast<size_t>(std::atoi(row_options[0].c_str()))};

}
int main(int argc, char* argv[]) {
    
    auto [row_count,col_count] = process_args(argc, argv);
  
    std::vector<int> matrix(row_count * col_count, 1);

    auto start = std::chrono::high_resolution_clock::now();
    row_major(matrix,row_count,col_count);
    auto end = std::chrono::high_resolution_clock::now();
    auto row_major_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    col_major(matrix,row_count,col_count);
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
