# Quantum Circuit Simulator

A high-performance quantum circuit simulator with a Qt-based graphical interface and a C++ backend for quantum gate simulation. This project provides both command-line and GUI tools for designing and simulating quantum circuits.

## Features

- Graphical quantum circuit editor for visual circuit design with modern QML interface
- Real-time quantum state visualization with Ket notation (| ψ ⟩ format)
- Visual circuit diagram showing qubit lines and gate symbols
- Backend simulation engine using Eigen library for efficient linear algebra operations
- Comprehensive gate support: Hadamard, Pauli-X/Y/Z, CNOT, SWAP, and Toffoli gates
- Multi-qubit operations and controlled gates
- Compiler optimizations (-O3) for improved performance
- Support for 1-5 qubit systems
- State initialization from binary strings
- Real-time circuit execution and quantum state visualization

## Prerequisites

### Required Dependencies
- C++17 compatible compiler (GCC 7+ or Clang 5+)
- CMake 3.10 or higher
- Qt6 development libraries
- Eigen3 library
- Google Test (optional, for unit tests)

### Installation on Debian/Ubuntu
```bash
sudo apt install build-essential cmake qt6-base-dev libeigen3-dev libgtest-dev
```

### Installation on Arch Linux
```bash
sudo pacman -S base-devel cmake qt6-base eigen gtest
```

## Building the Project

### Clone the Repository
```bash
git clone https://github.com/kaveen-lakmuthu/quantum_simulator.git
cd quantum_simulator
```

### Build Backend (Command-Line Interface)
```bash
make clean
make quantum_simulator
./quantum_simulator
```

### Build Frontend (GUI Application with Modern QML/Qt Quick)

**Dependencies**: Qt6 with declarative and quick modules
```bash
sudo apt install qt6-declarative-dev
```

**Build Instructions**:
```bash
cd frontend/build
cmake ..
make -j$(nproc)
./quantum_simulator_gui
```

The frontend features a modern dark-themed QML interface with:
- Professional Catppuccin-inspired color scheme
- Real-time quantum state visualization with ket notation
- Circuit visualization showing qubit lines and gate symbols
- Responsive three-panel layout (controls, circuit, state display)
- Integrated results with initial state, circuit definition, and final quantum state

### Build and Run Tests

Backend unit tests (Google Test via CTest):
```bash
cd backend/tests/build
cmake ..
make -j$(nproc)
ctest --output-on-failure
```

Frontend GUI tests (Qt Test via CTest):
```bash
cd frontend/build
cmake ..
make -j$(nproc)
QT_QPA_PLATFORM=offscreen ctest --output-on-failure
```

All backend and frontend tests should pass. Use the `offscreen` platform for headless environments (CI servers) to run Qt tests without a display.

## Usage

### Command-Line Interface
The backend provides an interactive CLI for quantum circuit design:

```bash
./quantum_simulator
# Follow prompts to:
# 1. Select number of qubits (1-5)
# 2. Add quantum gates
# 3. Execute circuit
# 4. View quantum state results
```

See [Usage Guide](docs/USAGE.md) for detailed examples.

### Graphical Interface (QML/Qt Quick)

1. Launch the modern QML-based GUI: `./quantum_simulator_gui`
2. **Header**: Select number of qubits (1-5 selector dropdown)
3. **Left Panel**: Gate operations organized by type
   - Single-qubit gates (H, X, Y, Z) with target qubit selector
   - Multi-qubit gates (CNOT, SWAP) with control/target selectors
4. **Center Panel**: 
   - Top: Visual circuit diagram showing qubit lines and gate symbols
   - Bottom: Scrollable gate sequence list
5. **Right Panel**: Complete execution flow showing:
   - Initial quantum state (| 00000 ⟩ format)
   - Defined quantum circuit (all added gates)
   - Execution status
   - Final quantum state after execution
6. Click **Execute** to run the circuit simulation
7. Click **Clear** to reset the circuit

See [Getting Started](docs/GETTING_STARTED.md) for installation help.

## Project Structure

```
quantum_simulator/
├── backend/                    # Quantum simulation logic
│   ├── src/                    # C++ source files
│   │   ├── main.cpp            # CLI entry point
│   │   ├── qubit_manager.cpp   # Quantum state management
│   │   ├── gate_engine.cpp     # Gate operation implementations
│   │   ├── circuit_manager.cpp # Circuit execution logic
│   │   └── utils.cpp           # Helper functions
│   └── tests/                  # Unit tests
│       ├── test_qubit_manager.cpp
│       ├── test_gate_engine.cpp
│       └── test_circuit_manager.cpp
├── frontend/                   # Qt6 GUI application (QML/Qt Quick)
│   ├── src/
│   │   ├── main_qml.cpp        # QML entry point
│   │   ├── MainWindow.qml      # Declarative UI layout
│   │   ├── backend_bridge.h/.cpp  # C++/QML property bridge
│   │   ├── circuit_painter.h/.cpp # Custom circuit visualization
│   │   └── resources.qrc       # QML resources
│   └── CMakeLists.txt
├── makefile                    # Build configuration
├── test_gates_manual.cpp       # Manual test suite
├── LICENSE                     # GNU GPL v3.0
└── README.md
```

## Implementation Details

### Quantum State Representation
- Uses Eigen library's complex vector type (`VectorXcd`)
- State vectors have dimension 2^n for n qubits
- Little-endian qubit ordering convention

### Supported Quantum Gates

**Single-Qubit Gates:**
- Hadamard (H): Creates superposition states
- Pauli-X: Bit flip gate
- Pauli-Y: Bit flip with phase
- Pauli-Z: Phase flip gate

**Multi-Qubit Gates:**
- CNOT: Controlled-NOT gate for entanglement
- SWAP: Exchanges two qubit states
- Toffoli: Three-qubit controlled-controlled-NOT gate

### Performance Optimizations
- Compiler optimizations (-O3) enable auto-vectorization and inlining
- Eigen library provides highly optimized SIMD operations
- In-place state vector transformations to minimize memory allocations
- Efficient bitwise operations for qubit indexing

**Note**: For small-scale quantum simulations (≤5 qubits), hand-written assembly provides no benefit due to:
- Overhead of data marshalling between C++ and assembly
- Eigen's already-optimized template code with SIMD intrinsics
- Modern compiler optimizations that match or exceed hand-tuned assembly
- Small problem size where setup costs dominate computation time

## Testing

The project includes comprehensive automated and manual testing:

### Backend Unit Tests (Google Test)
Located under `backend/tests` and executed via CTest.
```bash
cd backend/tests/build
cmake .. && make -j$(nproc)
ctest --output-on-failure
```

### Frontend GUI Tests (Qt Test)
Qt Test-based GUI tests are built in the frontend CMake and executed via CTest. Headless environments should set the platform to `offscreen`.
```bash
cd frontend/build
cmake .. && make -j$(nproc)
QT_QPA_PLATFORM=offscreen ctest --output-on-failure
```

### Manual Gate Sanity Tests (Optional)
For quick manual verification, a simple harness is provided.
```bash
make test_gates
./test_gates
```
Expected outputs and procedures are documented in [Testing Guide](docs/TESTING.md).

## Code Quality

This project follows strict code standards and best practices:

- **Header Guards**: Uses `#pragma once` for cleaner include protection
- **Documentation**: Comprehensive Doxygen comments for all public APIs
- **Naming**: Consistent camelCase parameter naming (targetQubit, controlQubit)
- **Constants**: All magic numbers replaced with semantic constants
- **Error Handling**: Descriptive error messages with context
- **Compilation**: Zero warnings with `-Wall -Wextra -std=c++17 -O3`

For detailed information, see [Code Standards Compliance Report](CODE_STANDARDS_COMPLIANCE.md).

## Documentation

Comprehensive documentation is available in the `docs/` directory:
- [Quick Start Guide](docs/GETTING_STARTED.md)
- [Usage Examples](docs/USAGE.md)
- [System Architecture](docs/ARCHITECTURE.md)
- [Quantum Gates](docs/QUANTUM_GATES.md)
- [Quantum Computing Theory](docs/THEORY.md)
- [API Reference](docs/API_REFERENCE.md)
- [Development Guide](docs/DEVELOPMENT.md)
- [Testing Methodology](docs/TESTING.md)
- [Build Instructions](docs/BUILDING.md)

## Development Roadmap

### Planned Features
- Quantum measurement operations with probabilistic outcomes
- Phase gates (S, T) for more complex algorithms
- Circuit save/load functionality (JSON/XML format)
- Noise models for realistic quantum simulation
- Performance profiling and benchmarking tools
- Extended qubit support (6-10 qubits)
- Visualization of Bloch sphere representation
- Support for custom gate definitions

## Known Limitations

- Maximum of 5 qubits in current implementation (32 state dimensions)
- No decoherence or noise modeling
- Limited to unitary gate operations
- No support for partial measurements

## Contributing

Contributions are welcome. Please follow these guidelines:
- Ensure code follows the existing style conventions
- Add unit tests for new features
- Update documentation as needed
- Test on both CLI and GUI interfaces

## License

This project is licensed under the GNU General Public License v3.0 (GPL-3.0).
See the LICENSE file for full details or visit: https://www.gnu.org/licenses/gpl-3.0.html

## Author

**Kaveen Lakmuthu**
- Email: kaveensalakmuthu@gmail.com
- GitHub: https://github.com/kaveen-lakmuthu

## Acknowledgments

- Eigen library for linear algebra operations
- Qt framework for GUI components
- Google Test framework for unit testing
