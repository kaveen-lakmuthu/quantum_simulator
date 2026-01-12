# Building and Compilation Guide

Detailed instructions for building the Quantum Circuit Simulator from source.

## Quick Build

For the impatient:

```bash
cd /path/to/quantum_simulator
make quantum_simulator
./quantum_simulator
```

## Detailed Build Instructions

### Prerequisites Check

Verify all required tools are installed:

```bash
# Check C++ compiler
g++ --version
# Should show: g++ (GCC) 7.0 or higher

# Check CMake
cmake --version
# Should show: cmake version 3.10 or higher

# Check Eigen3
pkg-config --modversion eigen3
# Should show: version 3.x.x
```

### Building Backend (CLI)

#### Step 1: Navigate to Project Root

```bash
cd /path/to/quantum_simulator
```

#### Step 2: Clean Previous Builds

```bash
make clean
```

This removes old object files and executables.

#### Step 3: Build Simulator

```bash
make quantum_simulator
```

Or build with custom compiler:

```bash
CXX=clang++ make quantum_simulator
```

#### Step 4: Verify Build

Check for executable:

```bash
ls -la quantum_simulator
# Should show executable with current date
```

Run sanity check:

```bash
./quantum_simulator
# Should display quantum state output without errors
```

### Building Frontend (GUI)

#### Prerequisites for GUI

Ensure Qt6 is installed:

```bash
pkg-config --modversion Qt6Core
pkg-config --modversion Qt6Gui
pkg-config --modversion Qt6Widgets
```

If not installed, install first:
```bash
# Ubuntu/Debian
sudo apt install qt6-base-dev

# Arch
sudo pacman -S qt6-base
```

#### Building GUI Steps

```bash
# Navigate to frontend
cd frontend

# Create build directory
mkdir -p build
cd build

# Generate build files with CMake
cmake ..

# Build with maximum parallel jobs
make -j$(nproc)

# Verify executable exists
./quantum_simulator_gui
```

#### Custom CMake Configuration

For non-standard installations:

```bash
cmake .. -DCMAKE_PREFIX_PATH=/path/to/qt6/installation
```

### Building Test Suite

#### Simple Test Build (Manual)

```bash
cd /path/to/quantum_simulator
g++ -I/usr/include/eigen3 -Ibackend/src \
    -Wall -Wextra -std=c++17 -O3 \
    -o test_gates test_gates_manual.cpp \
    backend/src/qubit_manager.cpp \
    backend/src/gate_engine.cpp \
    backend/src/circuit_manager.cpp \
    backend/src/utils.cpp

./test_gates
```

#### Backend Unit Tests (Google Test via CTest)

```bash
cd backend/tests/build
cmake ..
make -j$(nproc)
ctest --output-on-failure
```

#### Frontend GUI Tests (Qt Test via CTest)

```bash
cd frontend/build
cmake ..
make -j$(nproc)
QT_QPA_PLATFORM=offscreen ctest --output-on-failure
```

## Makefile Targets

The makefile provides several targets:

### target (quantum_simulator)

```bash
make quantum_simulator
# or
make
```

Builds the CLI executable.

### run_tests

```bash
make run_tests
```

Builds and links unit tests using Google Test framework.

**Requirements**:
- Google Test library installed
- gtest headers in standard path

### clean

```bash
make clean
```

Removes all build artifacts:
- Executables
- Object files
- Temporary files

### all (default)

```bash
make all
```

Builds all targets (quantum_simulator).

## Build Flags and Optimization

### Compiler Flags Explained

```makefile
CXXFLAGS = -I/usr/include/eigen3 \
           -I/usr/include/gtest \
           -Ibackend/src \
           -Wall \
           -Wextra \
           -std=c++17 \
           -O3
```

| Flag | Purpose |
|------|---------|
| `-I/usr/include/eigen3` | Include Eigen3 headers |
| `-I/usr/include/gtest` | Include Google Test headers |
| `-Ibackend/src` | Include project headers |
| `-Wall` | Enable all common warnings |
| `-Wextra` | Enable extra warnings |
| `-std=c++17` | Use C++17 standard |
| `-O3` | Maximum optimization (slower compile, faster runtime) |

### Custom Build Flags

For debugging (slower but more info):

```bash
make CXXFLAGS="-I/usr/include/eigen3 -Ibackend/src -Wall -Wextra -std=c++17 -g -O0"
```

Flags:
- `-g`: Include debugging symbols
- `-O0`: No optimization (debug friendly)

For profiling:

```bash
make CXXFLAGS="-I/usr/include/eigen3 -Ibackend/src -Wall -Wextra -std=c++17 -O3 -pg"
```

Then run with profiler:
```bash
./quantum_simulator
gprof quantum_simulator gmon.out
```

## Dependency Management

### Eigen3

**Location**: `/usr/include/eigen3`

**Verification**:
```bash
find /usr/include -name "Eigen" -type d
# Should show: /usr/include/eigen3/Eigen
```

**Installation**:
```bash
# Ubuntu/Debian
sudo apt install libeigen3-dev

# Arch
sudo pacman -S eigen

# macOS
brew install eigen
```

### Google Test

**Location**: `/usr/include/gtest`

**Verification**:
```bash
find /usr/include -name "gtest.h" -type f
# Should show path to gtest.h
```

**Installation**:
```bash
# Ubuntu/Debian
sudo apt install libgtest-dev

# Arch
sudo pacman -S gtest

# Build from source
cd /tmp
git clone https://github.com/google/googletest.git
cd googletest
mkdir build && cd build
cmake ..
make
sudo make install
```

### Qt6 (For GUI Only)

**Location**: `/usr/lib/cmake/Qt6`

**Verification**:
```bash
pkg-config --list-all | grep Qt6
```

**Installation**:
```bash
# Ubuntu/Debian
sudo apt install qt6-base-dev

# Arch
sudo pacman -S qt6-base

# macOS
brew install qt6

# From source
# See: https://www.qt.io/product/development-tools
```

## Troubleshooting Builds

### Error: "eigen3/Eigen/Dense: No such file or directory"

**Cause**: Eigen3 not installed or not in default include path

**Solution**:
```bash
# Install Eigen3
sudo apt install libeigen3-dev

# Or build with custom path
make CXXFLAGS="-I/custom/path/to/eigen3 ..."
```

### Error: "g++: command not found"

**Cause**: GCC not installed

**Solution**:
```bash
# Ubuntu/Debian
sudo apt install build-essential

# Arch
sudo pacman -S base-devel
```

### Error: "cmake: command not found"

**Cause**: CMake not installed

**Solution**:
```bash
# Ubuntu/Debian
sudo apt install cmake

# Arch
sudo pacman -S cmake
```

### Error: "gtest/gtest.h: No such file or directory"

**Cause**: Google Test not installed

**Solution**:
```bash
sudo apt install libgtest-dev
# Then rebuild tests
make clean
make run_tests
```

### Error: "Qt6 CMake file not found" (GUI build)

**Cause**: Qt6 not installed or CMake can't find it

**Solution**:
```bash
# Install Qt6
sudo apt install qt6-base-dev

# Help CMake find Qt6
cd frontend/build
cmake .. -DCMAKE_PREFIX_PATH=/usr/lib/cmake/Qt6
```

### Warning: "comparison between signed and unsigned integers"

**Cause**: Code warning (not an error)

**Solution**: Can be ignored or fixed in source code
- Doesn't prevent compilation
- Indicates potential type mismatch

## Advanced Build Options

### Parallel Build

Use multiple cores for faster compilation:

```bash
make -j4 quantum_simulator   # Use 4 cores
make -j$(nproc)              # Use all cores
```

### Static Linking

Link libraries statically (larger binary, no runtime dependencies):

```bash
g++ -I/usr/include/eigen3 -Ibackend/src -std=c++17 -O3 \
    -static-libgcc -static-libstdc++ \
    -o quantum_simulator_static \
    backend/src/main.cpp \
    backend/src/qubit_manager.cpp \
    backend/src/gate_engine.cpp \
    backend/src/circuit_manager.cpp \
    backend/src/utils.cpp
```

### Cross-Compilation

Build for different architecture:

```bash
# For ARM (e.g., Raspberry Pi)
arm-linux-gnueabihf-g++ -I/path/to/eigen3 ...
```

Requires ARM cross-compilation toolchain installed.

### Sanitizer Builds

Enable runtime error detection:

```bash
make CXXFLAGS="-I/usr/include/eigen3 -Ibackend/src -std=c++17 -O1 -g -fsanitize=address"
./quantum_simulator
# Shows memory errors and undefined behavior
```

## Build Configuration Files

### Makefile Structure

```makefile
# Variables (configurable)
CXX = g++
CXXFLAGS = ...
LDFLAGS = ...

# Targets (what to build)
TARGET = quantum_simulator
TEST_TARGET = run_tests

# Build rules
$(TARGET): $(SRC)
    $(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
    rm -f $(TARGET) $(TEST_TARGET)
```

### CMakeLists.txt (Frontend)

Located in `frontend/CMakeLists.txt`

Key variables:
- `CMAKE_CXX_STANDARD`: C++ version (17)
- `Qt6_COMPONENTS`: Qt modules needed
- `target_link_libraries`: Libraries to link

## Installation

After building, optionally install system-wide:

```bash
# Copy executable to /usr/local/bin
sudo cp quantum_simulator /usr/local/bin/

# Copy GUI executable
sudo cp frontend/build/quantum_simulator_gui /usr/local/bin/

# Now run from anywhere
quantum_simulator
quantum_simulator_gui
```

## Docker Build (Optional)

Create containerized build environment:

```dockerfile
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential cmake git \
    libeigen3-dev libgtest-dev qt6-base-dev

WORKDIR /app
COPY . .

RUN make quantum_simulator
CMD ["./quantum_simulator"]
```

Build:
```bash
docker build -t quantum_simulator .
docker run quantum_simulator
```

## Performance Testing

After building with `-O3`:

```bash
# Time the execution
time ./quantum_simulator

# Should show:
# real    0m0.015s
# user    0m0.012s
# sys     0m0.003s
```

For profiling:

```bash
./quantum_simulator &
# View resource usage
top -p $!
```

## Clean Build

Remove all build artifacts and rebuild from scratch:

```bash
make clean
rm -rf frontend/build/
make quantum_simulator
cd frontend && mkdir -p build && cd build
cmake .. && make
```

## Next Steps

- Run tests: `./test_gates`
- Launch GUI: `cd frontend/build && ./quantum_simulator_gui`
- Explore code: See [Architecture](ARCHITECTURE.md)
- Learn gates: See [Quantum Gates](QUANTUM_GATES.md)
