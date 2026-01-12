# Development Guide

Guide for contributing to and extending the Quantum Circuit Simulator.

## Project Overview for Developers

### Directory Structure

```
quantum_simulator/
├── backend/
│   ├── src/                 # Core quantum simulation code
│   │   ├── main.cpp         # CLI entry point
│   │   ├── qubit_manager.h/cpp
│   │   ├── gate_engine.h/cpp
│   │   ├── circuit_manager.h/cpp
│   │   └── utils.h/cpp
│   └── tests/               # Unit tests
├── frontend/                # Qt6 GUI application
│   └── src/
├── docs/                    # Documentation
├── makefile                 # Build configuration
└── test_gates_manual.cpp    # Manual test suite
```

### Code Style Guidelines

Follow these conventions throughout the codebase:

#### Naming Conventions

**Classes and Types**:
```cpp
class QubitManager { };
struct GateOperation { };
using StateVector = Eigen::VectorXcd;
```

**Functions and Methods**:
```cpp
void applyPauliX(QubitManager& qubits, int target);
int getNumQubits() const;
void initializeZeroState();
```

**Variables**:
```cpp
int num_qubits;
double amplitude_value;
Eigen::VectorXcd quantum_state;
```

**Constants**:
```cpp
const double PI = 3.14159265359;
const int MAX_QUBITS = 5;
```

#### Code Formatting

Use consistent indentation (4 spaces):

```cpp
void GateEngine::applyHadamard(QubitManager& qubits, int target) {
    validateQubitIndex(qubits, target);
    Eigen::VectorXcd& state = qubits.getState();
    
    for (int i = 0; i < dimension; ++i) {
        // Code indented 4 spaces
    }
}
```

Line length: Keep under 100 characters when reasonable.

#### Comments

Use clear, concise comments:

```cpp
// Single-line comments for brief explanations

/* Multi-line comments for
   complex logic requiring
   detailed explanation */

/// Doxygen-style comments for public APIs
/// \param qubits Reference to quantum state
/// \param target Target qubit index
void applyPauliX(QubitManager& qubits, int target);
```

### Architecture Decisions

#### QubitManager Design

- **State Representation**: Eigen VectorXcd (complex vector)
- **Dimension**: 2^n for n qubits
- **Ordering**: Little-endian (qubit 0 = LSB)
- **Initialization**: Always starts in |00...0⟩ state

#### GateEngine Design

- **Pattern**: Iterate all basis states, apply transformations
- **Efficiency**: In-place operations where possible
- **Error Handling**: Validate indices, throw exceptions
- **Extensibility**: Each gate is separate method

#### CircuitManager Design

- **Storage**: Vector of GateOperation structs
- **Execution**: Iterate gates, apply in order
- **Orchestration**: Delegates to GateEngine
- **Simplicity**: Lightweight wrapper, no optimization

## Adding New Features

### Adding a New Single-Qubit Gate

Example: Adding the S gate (phase gate).

#### Step 1: Add to GateEngine Header

File: `backend/src/gate_engine.h`

```cpp
class GateEngine {
public:
    // ... existing gates ...
    void applyS(QubitManager& qubits, int target_qubit);
    // S gate: applies e^(iπ/2) phase to |1⟩ state
};
```

#### Step 2: Implement in GateEngine

File: `backend/src/gate_engine.cpp`

```cpp
void GateEngine::applyS(QubitManager& qubits, int target_qubit) {
    validateQubitIndex(qubits, target_qubit);
    Eigen::VectorXcd& state = qubits.getState();
    int dimension = state.size();

    // Apply phase e^(iπ/2) = i to |1⟩ states
    std::complex<double> phase(0, 1);  // i

    for (int i = 0; i < dimension; ++i) {
        if ((i >> target_qubit) & 1) {
            state(i) *= phase;
        }
    }
}
```

#### Step 3: Add to CircuitManager

File: `backend/src/circuit_manager.cpp`

In `executeCircuit()` method:

```cpp
else if (gate_name_upper == "S" || gate_name_upper == "PHASE") {
    gate_engine.applyS(qubits, gate.target_qubit);
}
```

#### Step 4: Add Tests

File: `test_gates_manual.cpp`

```cpp
void test_s_gate() {
    std::cout << "\n=== Testing S Gate ===" << std::endl;
    QubitManager qubits(1);
    GateEngine engine;
    
    // Set state to |1⟩
    engine.applyPauliX(qubits, 0);
    
    // Apply S: |1⟩ → i|1⟩
    engine.applyS(qubits, 0);
    
    // Verify: amplitude should be i
    if (close_enough(qubits.getState()(1), std::complex<double>(0, 1))) {
        std::cout << "✓ S Gate test PASSED" << std::endl;
    } else {
        std::cout << "✗ S Gate test FAILED" << std::endl;
    }
}
```

In `main()`:
```cpp
test_s_gate();
```

#### Step 5: Rebuild and Test

```bash
make clean && make quantum_simulator
./test_gates
```

### Adding Multi-Qubit Gates

Similar process, but with multiple control/target qubits.

Example: Controlled-Z gate

```cpp
void GateEngine::applyCZ(QubitManager& qubits, 
                         int control_qubit, int target_qubit) {
    validateQubitIndex(qubits, control_qubit);
    validateQubitIndex(qubits, target_qubit);
    
    Eigen::VectorXcd& state = qubits.getState();
    int dimension = state.size();

    // Apply -1 phase if both qubits are |1⟩
    for (int i = 0; i < dimension; ++i) {
        if (((i >> control_qubit) & 1) && ((i >> target_qubit) & 1)) {
            state(i) = -state(i);
        }
    }
}
```

### Adding Measurement

Example: Add measurement functionality.

#### Step 1: Add Measurement Method

```cpp
// In QubitManager
public:
    int measureQubit(int qubit_index);  // Returns 0 or 1 with probabilities
    
    // Implementation
    int QubitManager::measureQubit(int qubit) {
        // Calculate probability of |1⟩
        double prob_one = 0;
        for (int i = 0; i < state.size(); ++i) {
            if ((i >> qubit) & 1) {
                prob_one += std::abs(state(i)) * std::abs(state(i));
            }
        }
        
        // Random number determines outcome
        if (random() < prob_one) return 1;
        else return 0;
    }
```

#### Step 2: Add Collapse Logic

```cpp
// After measurement, collapse state
// Set all amplitudes with different qubit value to 0
// Renormalize remaining amplitudes
```

#### Step 3: Add to GUI

Add "Measure" button to MainWindow that calls the measurement function.

## Extending the GUI

### Adding a New Gate Button

File: `frontend/src/mainwindow.cpp`

#### Step 1: Add Button Member

```cpp
class MainWindow : public QMainWindow {
private:
    QPushButton *sGateButton;  // New button
    void addSGate();           // Slot for button press
};
```

#### Step 2: Create Button in setupUI

```cpp
void MainWindow::setupGateButtons() {
    // ... existing buttons ...
    
    sGateButton = new QPushButton("S Gate");
    connect(sGateButton, &QPushButton::clicked, 
            this, &MainWindow::addSGate);
    layout->addWidget(sGateButton);
}
```

#### Step 3: Implement Slot

```cpp
void MainWindow::addSGate() {
    int target = qubitSelector->currentIndex();
    circuitView->addGate("S", target);
    updateCircuitDisplay();
}
```

### Adding Output Display

Extend `ResultsWindow` to show additional information:

```cpp
class ResultsWindow : public QWidget {
private:
    void displayProbabilities(const Eigen::VectorXcd& state);
    void displayBlochSphere(const Eigen::VectorXcd& state);
};
```

## Performance Optimization

### Profiling

Identify bottlenecks:

```bash
# Build with profiling
make CXXFLAGS="-O3 -pg" quantum_simulator

# Run and generate profiling data
./quantum_simulator

# Analyze results
gprof quantum_simulator gmon.out | head -20
```

### Optimization Strategies

1. **Reduce state iterations**: Use algebraic shortcuts
2. **Cache computations**: Store repeated calculations
3. **Use SIMD**: Leverage Eigen's vectorization
4. **Minimize allocations**: Reuse vectors

Example: Optimized Hadamard (current implementation)

```cpp
// Efficient: single pass, in-place where possible
for (int i = 0; i < dimension; ++i) {
    if ((i >> target) & 1) {
        new_state[i] = (state[i ^ (1 << target)] - state[i]) * sqrt2;
    } else {
        new_state[i] = (state[i] + state[i ^ (1 << target)]) * sqrt2;
    }
}
state = new_state;
```

## Testing Best Practices

### Unit Test Structure

```cpp
void test_feature() {
    // 1. Setup
    QubitManager qubits(n);
    GateEngine engine;
    
    // 2. Prepare state
    engine.applyGate(qubits, index);
    
    // 3. Execute operation
    engine.applyNewGate(qubits, params);
    
    // 4. Verify results
    if (condition) {
        std::cout << "✓ Test PASSED" << std::endl;
    } else {
        std::cout << "✗ Test FAILED" << std::endl;
        qubits.printState();  // Debug output
    }
}
```

### Test Coverage

Aim for:
- All public methods tested
- Edge cases covered
- Error conditions verified
- Integration tests for circuits

### Running Tests

```bash
./test_gates              # Manual tests
make run_tests            # Google Test (if installed)
./quantum_simulator       # CLI functional test
cd frontend/build && ./quantum_simulator_gui  # GUI test
```

## Documentation

### Code Documentation

Add Doxygen comments to public APIs:

```cpp
/// Applies Hadamard gate to create superposition
/// 
/// Transforms |0⟩ to (|0⟩+|1⟩)/√2 and |1⟩ to (|0⟩-|1⟩)/√2
/// 
/// \param qubits Quantum system to modify
/// \param target_qubit Target qubit index (0 to n-1)
/// \throws std::out_of_range If qubit index is invalid
/// 
/// \see applyPauliX, applyPauliY, applyPauliZ
void applyHadamard(QubitManager& qubits, int target_qubit);
```

### Update Documentation Files

When adding features, update:
- [docs/API_REFERENCE.md](API_REFERENCE.md)
- [docs/QUANTUM_GATES.md](QUANTUM_GATES.md)
- [docs/ARCHITECTURE.md](ARCHITECTURE.md)
- Main [README.md](../README.md)

## Git Workflow

### Commit Guidelines

```bash
# Create feature branch
git checkout -b feature/add-s-gate

# Make changes and test
# ... edit code ...
./test_gates

# Commit with clear message
git add backend/src/gate_engine.h backend/src/gate_engine.cpp
git commit -m "Add S gate (phase gate) implementation

- Implements S gate: applies e^(iπ/2) phase to |1⟩
- Added unit test verification
- Updated API documentation"

# Push and create pull request
git push origin feature/add-s-gate
```

### Code Review

Before merging:
1. All tests pass
2. Documentation updated
3. Code follows style guidelines
4. No regressions introduced

## Common Pitfalls

### 1. Off-by-One Errors

State indices are 0-based; qubits are numbered 0 to n-1.

```cpp
// CORRECT: qubits numbered 0 to n-1
for (int q = 0; q < num_qubits; ++q) {
    // ...
}

// WRONG: starts at 1
for (int q = 1; q < num_qubits; ++q) {
    // ...
}
```

### 2. Forgotten Normalization

Complex operations may require normalization:

```cpp
// After custom state modifications
normalizeState(state);  // Ensure ||state|| = 1
```

### 3. State Not Updated

Remember to update state reference:

```cpp
// CORRECT: update state
state = new_state;

// WRONG: new_state lost, state unchanged
Eigen::VectorXcd new_state = state;
// ... modify new_state ...
// (forgot: state = new_state;)
```

### 4. Qubit Index Validation

Always validate indices:

```cpp
// CORRECT
validateQubitIndex(qubits, target);

// WRONG: forgot validation
Eigen::VectorXcd& state = qubits.getState();
// Could crash if invalid index used
```

## Debugging Tips

### Enable Debug Output

```cpp
// In gate operations
std::cout << "Before gate:\n";
qubits.printState();

engine.applyGate(qubits, index);

std::cout << "After gate:\n";
qubits.printState();
```

### Use GDB Debugger

```bash
make CXXFLAGS="-I/usr/include/eigen3 -Ibackend/src -Wall -Wextra -std=c++17 -g"
gdb ./quantum_simulator

(gdb) break gate_engine.cpp:50
(gdb) run
(gdb) print state
(gdb) next
```

### Valgrind Memory Check

```bash
valgrind --leak-check=full ./quantum_simulator
```

## Building Documentation

Generate API documentation from code:

```bash
doxygen Doxyfile
# Generates HTML docs in docs/html/
firefox docs/html/index.html
```

## Release Checklist

Before releasing new version:

- [ ] All tests pass
- [ ] No compiler warnings
- [ ] Memory leaks checked (valgrind)
- [ ] Documentation updated
- [ ] README reflects current state
- [ ] Version number updated
- [ ] Changelog updated
- [ ] Code reviewed

## Next Steps

- Pick an issue from GitHub issues
- Create feature branch
- Implement with tests
- Submit pull request
- Engage in code review

## Resources

- [Architecture Guide](ARCHITECTURE.md)
- [API Reference](API_REFERENCE.md)
- [Quantum Gates](QUANTUM_GATES.md)
- [Testing Guide](TESTING.md)
- [Eigen Documentation](https://eigen.tuxfamily.org)
- [Qt Documentation](https://doc.qt.io)

## Questions?

Contact the maintainer: kaveensalakmuthu@gmail.com
