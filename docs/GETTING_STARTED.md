# Getting Started with Quantum Circuit Simulator

This guide will help you install and run the Quantum Circuit Simulator in minutes.

## Prerequisites

### System Requirements
- Linux-based OS (Debian, Ubuntu, Arch, Fedora, etc.)
- At least 2GB RAM
- 500MB free disk space

### Required Software
- GCC 7+ or Clang 5+ (C++17 support)
- CMake 3.10+
- Git

### Optional
- Qt6 development libraries (for GUI)
- Google Test (for running unit tests)

## Installation

### Step 1: Install Dependencies

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install build-essential cmake git libeigen3-dev libgtest-dev
```

**For GUI support:**
```bash
sudo apt install qt6-base-dev
```

**Arch Linux:**
```bash
sudo pacman -S base-devel cmake git eigen gtest
sudo pacman -S qt6-base  # Optional, for GUI
```

### Step 2: Clone the Repository
```bash
git clone https://github.com/kaveen-lakmuthu/quantum_simulator.git
cd quantum_simulator
```

### Step 3: Build Backend (Command-Line)
```bash
make clean
make quantum_simulator
```

### Step 4: Run the Backend
```bash
./quantum_simulator
```

You should see output like:
```
Initial Quantum State:
| 00000 ⟩ : (1,0)

Defined Quantum Circuit:
Quantum Circuit:
H (Qubit 0)
CNOT (Control: 0, Target: 1)
X (Qubit 2)
SWAP (Qubits: 4 <-> 3)

Executing Quantum Circuit...

Final Quantum State:
| 00100 ⟩ : (0.707107,0)
| 00111 ⟩ : (0.707107,0)
```

## Building the GUI (Optional)

If you want to use the graphical interface:

```bash
cd frontend
mkdir -p build
cd build
cmake ..
make -j$(nproc)
./quantum_simulator_gui
```

## Running Tests

To verify everything works correctly:

```bash
cd /home/your/path/to/quantum_simulator
g++ -I/usr/include/eigen3 -Ibackend/src -Wall -Wextra -std=c++17 -O3 \
    -o test_gates test_gates_manual.cpp \
    backend/src/qubit_manager.cpp backend/src/gate_engine.cpp \
    backend/src/circuit_manager.cpp backend/src/utils.cpp

./test_gates
```

All tests should pass (8/8).

## Troubleshooting

### "gtest/gtest.h: No such file or directory"
Install Google Test:
```bash
sudo apt install libgtest-dev  # Ubuntu/Debian
sudo pacman -S gtest           # Arch
```

### "eigen3/Eigen/Dense: No such file or directory"
Install Eigen3:
```bash
sudo apt install libeigen3-dev  # Ubuntu/Debian
sudo pacman -S eigen            # Arch
```

### "Qt6 not found"
Install Qt6 development libraries:
```bash
sudo apt install qt6-base-dev   # Ubuntu/Debian
sudo pacman -S qt6-base         # Arch
```

### CMake configuration fails
Clear the build directory and try again:
```bash
cd frontend
rm -rf build
mkdir build && cd build
cmake ..
make
```

## Next Steps

- Read the [Usage Guide](USAGE.md) to learn how to create circuits
- Explore [Architecture](ARCHITECTURE.md) to understand the codebase
- Check [Quantum Gates](QUANTUM_GATES.md) for detailed gate information
- See [Development Guide](DEVELOPMENT.md) if you want to contribute

## Verify Installation

To confirm everything is working, run the test suite:

```bash
./test_gates
```

Expected output:
```
=====================================
Quantum Gate Implementation Tests
=====================================

=== Testing Pauli-X Gate ===
✓ Pauli-X test PASSED

=== Testing Pauli-Y Gate ===
✓ Pauli-Y test PASSED

... (more tests) ...

====================================
All tests completed!
====================================
```

If you see all tests passed, the installation is complete and working correctly.
