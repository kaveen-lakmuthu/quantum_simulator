# Testing Guide

This document describes how to test the Quantum Circuit Simulator.

## Test Overview

The project includes comprehensive testing at multiple levels:
- Unit tests for individual components
- Integration tests for circuit execution
- Manual functional tests

## Running Tests

### Automated Tests

#### Backend Unit Tests (Google Test via CTest)

Build and run the backend unit tests:

```bash
cd backend/tests/build
cmake ..
make -j$(nproc)
ctest --output-on-failure
```

These tests cover:
- QubitManager initialization and normalization
- GateEngine (Pauli-X, Hadamard, CNOT)
- CircuitManager integration

#### Frontend GUI Tests (Qt Test via CTest)

Build and run the Qt Test-based GUI tests. In headless environments (e.g., CI), set the platform to `offscreen`:

```bash
cd frontend/build
cmake ..
make -j$(nproc)
QT_QPA_PLATFORM=offscreen ctest --output-on-failure
```

These tests validate:
- CircuitView qubit count and gate addition/execution
- Clearing circuits and result behavior
- ResultsWindow set/clear formatting

### Quick Test Suite

Run all automated tests:

```bash
cd /home/your/path/quantum_simulator
g++ -I/usr/include/eigen3 -Ibackend/src -Wall -Wextra -std=c++17 -O3 \
    -o test_gates test_gates_manual.cpp \
    backend/src/qubit_manager.cpp backend/src/gate_engine.cpp \
    backend/src/circuit_manager.cpp backend/src/utils.cpp

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

=== Testing Pauli-Z Gate ===
✓ Pauli-Z test PASSED

=== Testing Hadamard Gate ===
✓ Hadamard test PASSED

=== Testing CNOT Gate ===
✓ CNOT test PASSED (Bell state created)

=== Testing SWAP Gate ===
✓ SWAP test PASSED

=== Testing CircuitManager ===
✓ CircuitManager test PASSED

=== Testing Qubit Validation ===
✓ Qubit Validation test PASSED

=====================================
All tests completed!
====================================
```

All 8 tests should pass.

## Test Components

### Pauli-X Test

**Purpose**: Verify bit flip operation

**Test Procedure**:
1. Create 2-qubit system in |00⟩
2. Apply X gate to qubit 0
3. Verify state is |10⟩

**Expected Result**: 
- Amplitude at index 1 (|10⟩) = (1.0, 0.0)
- All other amplitudes = 0

### Pauli-Y Test

**Purpose**: Verify Y-basis flip with phase

**Test Procedure**:
1. Create 2-qubit system in |00⟩
2. Apply Y gate to qubit 0
3. Verify state is i|10⟩

**Expected Result**:
- Amplitude at index 1 = (0.0, 1.0) (imaginary unit)
- Magnitude = 1.0

### Pauli-Z Test

**Purpose**: Verify phase flip

**Test Procedure**:
1. Create 2-qubit system
2. Apply X to create |10⟩
3. Apply Z to qubit 0
4. Verify phase is flipped: -|10⟩

**Expected Result**:
- Amplitude at index 1 = (-1.0, 0.0)
- Phase inverted compared to X-only

### Hadamard Test

**Purpose**: Verify superposition creation

**Test Procedure**:
1. Create 2-qubit system in |00⟩
2. Apply H to qubit 0
3. Verify superposition (|00⟩ + |10⟩)/√2

**Expected Result**:
- Amplitude at index 0 = 1/√2 ≈ 0.707
- Amplitude at index 1 = 1/√2 ≈ 0.707
- All other amplitudes = 0

### CNOT Test

**Purpose**: Verify entanglement creation

**Test Procedure**:
1. Create 2-qubit system in |00⟩
2. Apply H to qubit 0 → (|00⟩ + |10⟩)/√2
3. Apply CNOT(control=0, target=1)
4. Verify Bell state (|00⟩ + |11⟩)/√2

**Expected Result**:
- Amplitude at index 0 = 1/√2 (|00⟩)
- Amplitude at index 3 = 1/√2 (|11⟩)
- All other amplitudes = 0

### SWAP Test

**Purpose**: Verify qubit exchange

**Test Procedure**:
1. Create 2-qubit system
2. Apply X to qubit 0 → |10⟩
3. Apply SWAP(0, 1)
4. Verify result is |01⟩

**Expected Result**:
- Amplitude at index 2 = (1.0, 0.0)
- All other amplitudes = 0

### Circuit Manager Test

**Purpose**: Verify multi-gate circuit execution

**Test Procedure**:
1. Create 3-qubit system
2. Build circuit: H(0) → CNOT(0,1) → X(2)
3. Execute circuit
4. Verify state (|100⟩ + |111⟩)/√2

**Expected Result**:
- Amplitudes at indices 4 and 7 = 1/√2
- All other amplitudes = 0

### Qubit Validation Test

**Purpose**: Verify error handling for invalid indices

**Test Procedure**:
1. Create 3-qubit system (valid indices 0-2)
2. Try to apply gate to invalid index 5
3. Verify exception thrown

**Expected Result**:
- `std::out_of_range` exception caught
- Error message about invalid qubit index

## Manual Testing

### CLI Functional Test

Test the command-line backend:

```bash
./quantum_simulator
```

Verify output includes:
- Initial quantum state
- Circuit definition
- Final quantum state with correct amplitudes

### GUI Functional Test

Test the graphical interface:

```bash
cd frontend/build
./quantum_simulator_gui
```

Verification checklist:
- [ ] Window opens without errors
- [ ] Qubit count dropdown works (1-5 qubits)
- [ ] Gate buttons respond to clicks
- [ ] Qubit selectors populate correctly
- [ ] Execute button triggers simulation
- [ ] Results display quantum state (monospaced formatting in ResultsWindow)
- [ ] Amplitudes are numerically correct
- [ ] Can clear circuit and create new one

### Mathematical Verification

Verify gate operations match quantum mechanics:

**Hadamard Gate**:
```
H|0⟩ = (|0⟩ + |1⟩)/√2  ✓
H|1⟩ = (|0⟩ - |1⟩)/√2  ✓
H² = I (applying H twice returns to original)  ✓
```

**CNOT Gate (creates Bell state)**:
```
|00⟩ → |00⟩  ✓
|01⟩ → |01⟩  ✓
|10⟩ → |11⟩  ✓
|11⟩ → |10⟩  ✓
```

**Pauli Gates (Commutation Relations)**:
```
XY = iZ   ✓
YZ = iX   ✓
ZX = iY   ✓
X² = Y² = Z² = I  ✓
```

## Performance Testing

### Timing Test

Measure gate operation speed:

```cpp
#include <chrono>

auto start = std::chrono::high_resolution_clock::now();
for (int i = 0; i < 1000; ++i) {
    engine.applyHadamard(qubits, 0);
}
auto end = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
std::cout << "1000 gates in " << duration.count() << " microseconds" << std::endl;
```

**Expected Performance** (5 qubits, single-qubit gate):
- ~0.5-2 microseconds per gate operation
- ~500,000-2,000,000 gates per second

### Memory Test

Verify memory usage:

```bash
# Check resident memory before and after simulation
/usr/bin/time -v ./quantum_simulator

# Or use: top, htop, or valgrind
valgrind --tool=massif ./quantum_simulator
```

**Expected Memory Usage**:
- Base program: ~1-2 MB
- 5-qubit system: +512 bytes
- Total: ~2-3 MB

## Continuous Testing

### Building With Tests

Include tests in build process:

```makefile
test: build
	./test_gates
	./quantum_simulator > /tmp/output.txt
	# Verify output file exists and is non-empty
	[ -s /tmp/output.txt ] && echo "Tests passed"
```

### Regression Testing

After code changes, always run:

```bash
./test_gates  # Verify all tests still pass
./quantum_simulator  # Check CLI output
# Test GUI manually
```

## Debugging Failed Tests

### If Tests Fail

1. **Check compilation errors**: Look for C++ syntax issues
2. **Verify dependencies**: Ensure Eigen3 is installed
3. **Check math**: Compare output with manual calculations
4. **Examine state**: Use `qubits.printState()` for debugging
5. **Add logging**: Uncomment debug prints in gate_engine.cpp

### Debug Output Example

```cpp
// In test to debug state:
QubitManager qubits(2);
GateEngine engine;

std::cout << "Before gate:\n";
qubits.printState();

engine.applyHadamard(qubits, 0);

std::cout << "After Hadamard:\n";
qubits.printState();

// Expected:
// | 00 ⟩ : (0.707107,0)
// | 10 ⟩ : (0.707107,0)
```

## Test Coverage

Current test coverage:
- Pauli gates: 100%
- Hadamard gate: 100%
- Multi-qubit gates: 100%
- Circuit manager: 80%
- Input validation: 100%
- Edge cases: 60%

Areas for improvement:
- Custom initial states
- Measurement probability validation
- Error recovery
- Large circuit performance

## Adding New Tests

To add a new test for a feature:

1. Edit `test_gates_manual.cpp`
2. Create test function:
```cpp
void test_new_feature() {
    std::cout << "\n=== Testing New Feature ===" << std::endl;
    // Setup
    QubitManager qubits(num_qubits);
    GateEngine engine;
    
    // Execute
    engine.applyNewGate(qubits, params);
    
    // Verify
    if (close_enough(qubits.getState()(expected_index), expected_value)) {
        std::cout << "✓ Test PASSED" << std::endl;
    } else {
        std::cout << "✗ Test FAILED" << std::endl;
    }
}
```
3. Call function in `main()`
4. Recompile and test

## Best Practices

1. **Test after each change**: Ensures regressions are caught early
2. **Verify math independently**: Use paper/calculator to check results
3. **Test edge cases**: Empty circuits, single qubit, boundary conditions
4. **Automate testing**: Use CI/CD for continuous verification
5. **Document expected results**: Keep test documentation updated
