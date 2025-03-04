# Memory Access Pattern Benchmark

## Overview
This program measures the performance difference between row-major and column-major memory access patterns using a 2D vector (matrix). It demonstrates how memory access order impacts performance due to caching and memory locality effects.

## Features
- Accepts matrix dimensions as command-line arguments.
- Measures execution time for row-major and column-major traversal.
- Displays results in a formatted table.

## Dependencies
- C++ compiler supporting C++11 or later
- [`kaizen.h`](https://github.com/heinsaar/kaizen) for command-line argument parsing.

## Build and Run 

```git clone https://github.com/marybadalyan/row_column_Access```

# Go into the repository:

```cd row_column_Access```

# Generate the build files:

```cmake -DCMAKE_BUILD_TYPE=Release -S . -B build```

# Build the project:

```cmake --build build --config Release```

Run the executable generated in the build directory:

```./build/row_column_Access```

Once compiled, run the program:

```./row_column_access --cols [num] --rows [num]``` // num as in int 

## Example Output
```
+----------------------------------------+
|  Rows x Cols | Row Major  | Col Major  |
+----------------------------------------+
| 3000 x 3000  |        4ms |      28ms  |
+----------------------------------------+
```

## Explanation
- **Row-major traversal**: Iterates over rows first, benefiting from spatial locality.
- **Column-major traversal**: Iterates over columns first, causing more cache misses and potential slowdowns.
