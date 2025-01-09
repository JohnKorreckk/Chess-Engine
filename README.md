# Chess Engine

This repository contains a chess engine implemented in C++. It is designed for efficient move generation, board evaluation, and support for various chess functionalities. Below is a breakdown of its key features.

## Features
- **Functional GUI**: Chess Board GUI that allows drag and drop of pieces and piece captures.
- **Move Generation**: Efficient algorithms for generating all legal moves from any given position. (Working progress)
- **Board Evaluation**: Static evaluation functions to assess the strength of a position. (Working progress)
- **AI Logic**: Basic AI capabilities using Minimax and Alpha-Beta pruning. (Working progress)
- **Testing**: Unit tests to validate key functionalities.

## Getting Started

### Prerequisites
Ensure you have the following installed:
- A compatible C++ compiler (e.g., GCC, Clang, MSVC)

### Building the Project
1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd Chess-Engine-main
2. Create a build directory:
   ```bash
   mkdir build && cd build
3. Run CMake to configure the project:
   ```bash
   cmake ..
4. Build the project:
   ```bash
   make

### Building the Project
Execute the compiled binary from the build directory:
```bash
./chess_engine
