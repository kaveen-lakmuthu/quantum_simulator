# Usage Guide

Comprehensive guide to using the Quantum Circuit Simulator.

## Table of Contents

1. [Command-Line Interface](#command-line-interface)
2. [Graphical Interface](#graphical-interface)
3. [Creating Circuits](#creating-circuits)
4. [Understanding Output](#understanding-output)
5. [Examples](#examples)

## Command-Line Interface

### Running the CLI

```bash
./quantum_simulator
```

The CLI executes a pre-defined circuit. To modify the circuit, edit `backend/src/main.cpp`.

### Default Circuit

The default circuit demonstrates all gate types:

```cpp
QubitManager qubits(5);           // Create 5-qubit system
CircuitManager circuit;

circuit.addGate("H", 0);          // Hadamard on qubit 0
circuit.addGate("CNOT", 1, 0);    // CNOT: control=0, target=1
circuit.addGate("X", 2);          // Pauli-X on qubit 2
circuit.addGate("SWAP", 3, 4);    // Swap qubits 3 and 4

circuit.executeCircuit(qubits);
qubits.printState();
```

### Modifying the Circuit

Edit `backend/src/main.cpp` to create custom circuits:

```cpp
#include "qubit_manager.h"
#include "gate_engine.h"
#include "circuit_manager.h"
#include <iostream>

int main() {
    // 1. Create quantum system (change number for different qubit counts)
    QubitManager qubits(3);  // 3-qubit system
    
    // 2. Create circuit
    CircuitManager circuit;
    
    // 3. Add gates (modify as needed)
    circuit.addGate("H", 0);                    // Superposition
    circuit.addGate("CNOT", 0, 1);              // Entanglement
    
    // 4. Print and execute
    std::cout << "Circuit:\n";
    circuit.printCircuit();
    
    std::cout << "\nInitial state:\n";
    qubits.printState();
    
    circuit.executeCircuit(qubits);
    
    std::cout << "\nFinal state:\n";
    qubits.printState();
    
    return 0;
}
```

Then rebuild:
```bash
make clean
make quantum_simulator
./quantum_simulator
```

## Graphical Interface

### Launching the GUI

```bash
cd frontend/build
./quantum_simulator_gui
```

### Main Window Layout

The GUI provides:
- **Qubit Selector**: Choose number of qubits (1-5)
- **Gate Buttons**: Click to add gates (Hadamard, X, Y, Z, CNOT, SWAP)
- **Target/Control Selectors**: Choose qubits for gate operations
- **Execute Button**: Run the circuit simulation
- **Results Display**: View quantum state amplitudes
- **Clear Button**: Reset circuit for new design

### Step-by-Step Walkthrough

#### 1. Select Qubit Count

```
┌─ Qubit Count ──────────┐
│  [▼ 3 qubits]          │
└────────────────────────┘
```

Click the dropdown and select number of qubits (1-5).

#### 2. Add Gates

Click any gate button to add it to the circuit:

```
┌─ Available Gates ──────────────────────────┐
│ [Hadamard] [Pauli-X] [Pauli-Y] [Pauli-Z] │
│ [CNOT]     [SWAP]    [Toffoli]            │
└────────────────────────────────────────────┘
```

#### 3. Select Target Qubit

For single-qubit gates (H, X, Y, Z):

```
┌─ Target Qubit ─────────┐
│ [▼ Qubit 0]            │
└────────────────────────┘
```

#### 4. Select Control Qubit (Multi-Qubit Gates)

For multi-qubit gates (CNOT, SWAP, TOFFOLI):

```
┌─ Control Qubit ────────┐
│ [▼ Qubit 0]            │
└────────────────────────┘

┌─ Target Qubit ─────────┐
│ [▼ Qubit 1]            │
└────────────────────────┘
```

#### 5. Execute Circuit

Click "Execute Circuit" button:

```
┌──────────────────────────┐
│   Execute Circuit        │
└──────────────────────────┘
```

#### 6. View Results

Results appear in output window:

```
Quantum State Results:
| 000 ⟩ : 0.7071 + 0.0000i
| 011 ⟩ : 0.7071 + 0.0000i

Probabilities:
|000⟩: 50.00%
|011⟩: 50.00%
```

## Creating Circuits

### Circuit Design Tips

#### 1. Single-Qubit Gates

Apply gates to individual qubits:

```cpp
circuit.addGate("H", 0);   // Hadamard on qubit 0
circuit.addGate("X", 1);   // Pauli-X on qubit 1
circuit.addGate("Z", 2);   // Pauli-Z on qubit 2
```

#### 2. Creating Superposition

Use Hadamard gates for superposition:

```cpp
circuit.addGate("H", 0);
circuit.addGate("H", 1);
// Result: (|00⟩ + |01⟩ + |10⟩ + |11⟩)/2
```

#### 3. Creating Entanglement

Use CNOT for Bell states:

```cpp
circuit.addGate("H", 0);              // Create superposition
circuit.addGate("CNOT", 0, 1);        // Entangle with qubit 1
// Result: (|00⟩ + |11⟩)/√2 (Bell state)
```

#### 4. Multi-Gate Sequences

Combine gates for complex operations:

```cpp
// GHZ State (3-qubit entanglement)
circuit.addGate("H", 0);
circuit.addGate("CNOT", 0, 1);
circuit.addGate("CNOT", 0, 2);
// Result: (|000⟩ + |111⟩)/√2
```

#### 5. State Swapping

Use SWAP gate to rearrange qubits:

```cpp
circuit.addGate("X", 0);              // Set qubit 0 to |1⟩
circuit.addGate("SWAP", 0, 2);        // Swap qubits 0 and 2
// Result: |100⟩ becomes |001⟩
```

## Understanding Output

### State Vector Format

Each amplitude is displayed as:

```
| binary_state ⟩ : real_part + imaginary_part i
```

Example:
```
| 01 ⟩ : 0.7071 + 0.0000i
```

Means: amplitude at state |01⟩ is 0.7071 (real) + 0i (imaginary)

### Non-Zero Amplitudes

Only amplitudes with magnitude > 1e-10 are displayed. This filters numerical noise.

### Probability Calculation

Probability of measuring a state is:

```
P(|state⟩) = |amplitude|²
```

Example:
```
Amplitude: 0.7071
Probability: (0.7071)² ≈ 0.5 = 50%
```

### Complex Numbers

Amplitudes can have imaginary parts (from Pauli-Y and other gates):

```
| 10 ⟩ : 0.0000 + 0.7071i

This means: amplitude = 0 + 0.7071i
Magnitude: |0.7071i| = 0.7071
Probability: (0.7071)² ≈ 0.5 = 50%
```

## Examples

### Example 1: Simple Hadamard

**Goal**: Create equal superposition on single qubit

**Circuit**:
```cpp
QubitManager qubits(1);
CircuitManager circuit;
circuit.addGate("H", 0);
circuit.executeCircuit(qubits);
```

**Expected Output**:
```
| 0 ⟩ : 0.7071 + 0.0000i
| 1 ⟩ : 0.7071 + 0.0000i
```

**Interpretation**: 50% chance of measuring 0, 50% chance of measuring 1

---

### Example 2: Bell State (Entanglement)

**Goal**: Create maximally entangled 2-qubit state

**Circuit**:
```cpp
QubitManager qubits(2);
CircuitManager circuit;
circuit.addGate("H", 0);              // Superposition
circuit.addGate("CNOT", 0, 1);        // Entanglement
circuit.executeCircuit(qubits);
```

**Expected Output**:
```
| 00 ⟩ : 0.7071 + 0.0000i
| 11 ⟩ : 0.7071 + 0.0000i
```

**Interpretation**: Qubits are entangled. Measuring qubit 0 determines qubit 1.

---

### Example 3: Three-Qubit GHZ State

**Goal**: Create three-qubit entanglement

**Circuit**:
```cpp
QubitManager qubits(3);
CircuitManager circuit;
circuit.addGate("H", 0);              // Superposition on qubit 0
circuit.addGate("CNOT", 0, 1);        // Entangle qubit 1
circuit.addGate("CNOT", 0, 2);        // Entangle qubit 2
circuit.executeCircuit(qubits);
```

**Expected Output**:
```
| 000 ⟩ : 0.7071 + 0.0000i
| 111 ⟩ : 0.7071 + 0.0000i
```

**Interpretation**: All three qubits are correlated.

---

### Example 4: Pauli-Y with Phase

**Goal**: Observe phase from Pauli-Y gate

**Circuit**:
```cpp
QubitManager qubits(1);
CircuitManager circuit;
circuit.addGate("Y", 0);
circuit.executeCircuit(qubits);
```

**Expected Output**:
```
| 1 ⟩ : 0.0000 + 1.0000i
```

**Interpretation**: Amplitude is purely imaginary (i = √(-1))

---

### Example 5: SWAP Gate Demo

**Goal**: Exchange qubit states

**Circuit**:
```cpp
QubitManager qubits(2);
qubits.setInitialState("10");         // Set to |10⟩
CircuitManager circuit;
circuit.addGate("SWAP", 0, 1);        // Swap qubits 0 and 1
circuit.executeCircuit(qubits);
```

**Expected Output**:
```
| 01 ⟩ : 1.0000 + 0.0000i
```

**Interpretation**: |10⟩ was swapped to |01⟩

---

### Example 6: Quantum Interference

**Goal**: Observe destructive interference

**Circuit**:
```cpp
QubitManager qubits(1);
CircuitManager circuit;
circuit.addGate("H", 0);              // H: |0⟩ → (|0⟩+|1⟩)/√2
circuit.addGate("X", 0);              // X: flips qubits
circuit.addGate("H", 0);              // H again
circuit.executeCircuit(qubits);
```

**Expected Output**:
```
| 1 ⟩ : 1.0000 + 0.0000i
```

**Interpretation**: Amplitudes cancel through interference, leaving only |1⟩

---

## Troubleshooting

### Empty Output
- No amplitudes displayed (all near zero)
- **Cause**: State collapse or cancellation
- **Solution**: Verify circuit logic and initial state

### Unexpected Phase
- Complex (imaginary) amplitudes when expecting real
- **Cause**: Pauli-Y gate or complex phase accumulation
- **Solution**: Check gate sequence; Y gates introduce imaginary parts

### Sum of Probabilities ≠ 100%
- Usually means numerical error
- **Cause**: Floating-point rounding errors
- **Solution**: Normal for |error| < 0.001; re-run if significant

### Invalid Qubit Index
- Error message about qubit out of range
- **Cause**: Selected qubit index ≥ number of qubits
- **Solution**: Verify qubit count matches selected indices

## Tips and Tricks

1. **Reset circuits**: Clear and rebuild to test variations
2. **Print intermediate states**: Add debug `printState()` calls
3. **Verify math**: Cross-check outputs with quantum theory
4. **Experiment**: Try different gate combinations to learn
5. **Document circuits**: Add comments explaining circuit purpose

## Next Steps

- Explore [Quantum Gates Reference](QUANTUM_GATES.md) for more gate details
- Read [System Architecture](ARCHITECTURE.md) to understand implementation
- Check [Testing Guide](TESTING.md) to validate simulations
- See [Development Guide](DEVELOPMENT.md) to extend the simulator
