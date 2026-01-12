# API Reference

Complete C++ API documentation for the Quantum Circuit Simulator backend.

## QubitManager

**Header**: `backend/src/qubit_manager.h`

**Purpose**: Manages quantum state representation and initialization.

### Constructor

```cpp
QubitManager(int num_qubits)
```

Creates a quantum system with specified number of qubits, initialized to |00...0⟩ state.

**Parameters**:
- `num_qubits` (int): Number of qubits in the system (1-5)

**Throws**: None

**Example**:
```cpp
QubitManager qubits(3);  // Create 3-qubit system |000⟩
```

### Methods

#### initializeZeroState

```cpp
void initializeZeroState()
```

Resets the quantum state to |00...0⟩.

**Returns**: void

**Side Effects**: Modifies state to all zeros with first element = 1.0

**Example**:
```cpp
qubits.initializeZeroState();  // Reset to ground state
```

#### getState

```cpp
Eigen::VectorXcd& getState()
Eigen::VectorXcd& getState() const
```

Returns reference to the quantum state vector.

**Returns**: 
- Mutable: `Eigen::VectorXcd&` - can modify state
- Const: `const Eigen::VectorXcd&` - read-only access

**Note**: State vector has dimension 2^n where n = num_qubits

**Example**:
```cpp
Eigen::VectorXcd state = qubits.getState();
std::complex<double> amplitude = state(0);  // Get first amplitude
```

#### getNumQubits

```cpp
int getNumQubits() const
```

Returns the number of qubits in the system.

**Returns**: int - number of qubits

**Example**:
```cpp
int n = qubits.getNumQubits();  // Returns 5 for 5-qubit system
int dimension = (1 << n);        // Calculate state dimension: 2^n
```

#### setInitialState

```cpp
void setInitialState(const std::string& stateStr)
```

Sets the quantum state to a specific basis state from a binary string.

**Parameters**:
- `stateStr` (const std::string&): Binary string (e.g., "00101")

**Throws**: None (prints error to stderr)

**Note**: String length must match number of qubits

**Example**:
```cpp
QubitManager qubits(3);
qubits.setInitialState("101");  // Set to |101⟩
```

#### printState

```cpp
void printState() const
```

Prints all non-zero amplitudes in the quantum state to stdout.

**Format**:
```
| binary_state ⟩ : amplitude
```

**Example Output**:
```
| 00 ⟩ : (0.707107,0)
| 11 ⟩ : (0.707107,0)
```

---

## GateEngine

**Header**: `backend/src/gate_engine.h`

**Purpose**: Implements quantum gate operations on quantum states.

### Single-Qubit Gates

#### applyPauliX

```cpp
void applyPauliX(QubitManager& qubits, int target_qubit)
```

Applies Pauli-X (bit flip) gate to target qubit.

**Parameters**:
- `qubits`: Reference to QubitManager
- `target_qubit` (int): Target qubit index (0 to n-1)

**Throws**: `std::out_of_range` if qubit index invalid

**Matrix**:
```
X = [ 0  1 ]
    [ 1  0 ]
```

**Example**:
```cpp
GateEngine engine;
engine.applyPauliX(qubits, 0);  // Flip qubit 0
```

#### applyPauliY

```cpp
void applyPauliY(QubitManager& qubits, int target_qubit)
```

Applies Pauli-Y gate to target qubit.

**Parameters**:
- `qubits`: Reference to QubitManager
- `target_qubit` (int): Target qubit index

**Throws**: `std::out_of_range` if qubit index invalid

**Matrix**:
```
Y = [ 0  -i ]
    [ i   0 ]
```

**Example**:
```cpp
engine.applyPauliY(qubits, 1);  // Y gate on qubit 1
```

#### applyPauliZ

```cpp
void applyPauliZ(QubitManager& qubits, int target_qubit)
```

Applies Pauli-Z (phase flip) gate to target qubit.

**Parameters**:
- `qubits`: Reference to QubitManager
- `target_qubit` (int): Target qubit index

**Throws**: `std::out_of_range` if qubit index invalid

**Matrix**:
```
Z = [ 1   0 ]
    [ 0  -1 ]
```

**Example**:
```cpp
engine.applyPauliZ(qubits, 2);  // Z gate on qubit 2
```

#### applyHadamard

```cpp
void applyHadamard(QubitManager& qubits, int target_qubit)
```

Applies Hadamard gate to target qubit, creating superposition.

**Parameters**:
- `qubits`: Reference to QubitManager
- `target_qubit` (int): Target qubit index

**Throws**: `std::out_of_range` if qubit index invalid

**Matrix**:
```
H = (1/√2) [ 1   1 ]
            [ 1  -1 ]
```

**Example**:
```cpp
engine.applyHadamard(qubits, 0);  // Create superposition on qubit 0
```

### Multi-Qubit Gates

#### applyCNOT

```cpp
void applyCNOT(QubitManager& qubits, int control_qubit, int target_qubit)
```

Applies CNOT (Controlled-NOT) gate. Flips target qubit if control qubit is |1⟩.

**Parameters**:
- `qubits`: Reference to QubitManager
- `control_qubit` (int): Control qubit index
- `target_qubit` (int): Target qubit index

**Throws**: 
- `std::out_of_range` if qubit indices invalid
- `std::invalid_argument` if control == target

**Use Case**: Creating Bell states, entanglement

**Example**:
```cpp
engine.applyCNOT(qubits, 0, 1);  // Control on 0, target on 1
```

#### applySWAP

```cpp
void applySWAP(QubitManager& qubits, int qubit1, int qubit2)
```

Exchanges the states of two qubits.

**Parameters**:
- `qubits`: Reference to QubitManager
- `qubit1` (int): First qubit index
- `qubit2` (int): Second qubit index

**Throws**: `std::out_of_range` if qubit indices invalid

**Example**:
```cpp
engine.applySWAP(qubits, 2, 4);  // Swap qubits 2 and 4
```

#### applyToffoli

```cpp
void applyToffoli(QubitManager& qubits, int control1, int control2, int target_qubit)
```

Applies Toffoli (CCX) gate. Flips target if both control qubits are |1⟩.

**Parameters**:
- `qubits`: Reference to QubitManager
- `control1` (int): First control qubit index
- `control2` (int): Second control qubit index
- `target_qubit` (int): Target qubit index

**Throws**:
- `std::out_of_range` if qubit indices invalid
- `std::invalid_argument` if any qubits are the same

**Example**:
```cpp
engine.applyToffoli(qubits, 0, 1, 2);  // Controls on 0,1; target on 2
```

### Private Methods

#### validateQubitIndex

```cpp
void validateQubitIndex(const QubitManager& qubits, int qubit) const
```

Validates that qubit index is within valid range (0 to num_qubits-1).

**Parameters**:
- `qubits`: Reference to QubitManager
- `qubit` (int): Qubit index to validate

**Throws**: `std::out_of_range` with descriptive message

---

## CircuitManager

**Header**: `backend/src/circuit_manager.h`

**Purpose**: Manages and executes sequences of quantum gates.

### Data Structures

#### GateOperation

```cpp
struct GateOperation {
    std::string gate_name;      // Gate identifier ("H", "X", "CNOT", etc.)
    int target_qubit;           // Primary qubit operand
    int control_qubit1;         // First control qubit (-1 if unused)
    int control_qubit2;         // Second control qubit (-1 if unused)
};
```

### Methods

#### addGate

```cpp
void addGate(const std::string& gate_name, 
             int target_qubit, 
             int control_qubit1 = -1, 
             int control_qubit2 = -1)
```

Adds a gate to the circuit sequence.

**Parameters**:
- `gate_name` (string): Gate name ("H", "X", "Y", "Z", "CNOT", "SWAP", "TOFFOLI")
- `target_qubit` (int): Target qubit index
- `control_qubit1` (int): First control qubit (default: -1 = unused)
- `control_qubit2` (int): Second control qubit (default: -1 = unused)

**Throws**: `std::invalid_argument` if target_qubit is negative

**Example**:
```cpp
CircuitManager circuit;
circuit.addGate("H", 0);              // Hadamard on qubit 0
circuit.addGate("CNOT", 1, 0);        // CNOT: control=0, target=1
circuit.addGate("TOFFOLI", 2, 0, 1);  // Toffoli: controls=0,1, target=2
```

#### executeCircuit

```cpp
void executeCircuit(QubitManager& qubits)
```

Executes all queued gates in sequence on the quantum state.

**Parameters**:
- `qubits`: Reference to QubitManager (state is modified)

**Throws**: 
- `std::invalid_argument` if gate name unknown or parameters invalid
- `std::out_of_range` if qubit indices out of range

**Side Effects**: Modifies quantum state in `qubits`

**Example**:
```cpp
QubitManager qubits(3);
CircuitManager circuit;
circuit.addGate("H", 0);
circuit.addGate("X", 1);
circuit.executeCircuit(qubits);  // Execute both gates

qubits.printState();  // View result
```

#### printCircuit

```cpp
void printCircuit() const
```

Prints all gates in the circuit to stdout.

**Format**:
```
Gate_Name (Qubit info)
```

**Example Output**:
```
Quantum Circuit:
H (Qubit 0)
CNOT (Control: 0, Target: 1)
X (Qubit 2)
SWAP (Qubits: 2 <-> 3)
```

---

## Utility Functions

**Header**: `backend/src/utils.h`

#### normalizeState

```cpp
void normalizeState(Eigen::VectorXcd& state)
```

Normalizes quantum state so sum of probability amplitudes = 1.

**Parameters**:
- `state`: Reference to state vector to normalize

**Example**:
```cpp
Eigen::VectorXcd state = qubits.getState();
normalizeState(state);  // Ensure ||state|| = 1
```

#### printState

```cpp
void printState(const Eigen::VectorXcd& state)
```

Prints state vector to stdout.

**Parameters**:
- `state`: State vector to print

---

## Common Usage Patterns

### Creating and Executing a Circuit

```cpp
#include "qubit_manager.h"
#include "gate_engine.h"
#include "circuit_manager.h"

int main() {
    // Create system
    QubitManager qubits(3);
    CircuitManager circuit;
    
    // Build circuit
    circuit.addGate("H", 0);
    circuit.addGate("CNOT", 1, 0);
    
    // Execute
    circuit.executeCircuit(qubits);
    
    // Display results
    qubits.printState();
    
    return 0;
}
```

### Working with State Directly

```cpp
QubitManager qubits(2);
GateEngine engine;

// Get state vector
Eigen::VectorXcd& state = qubits.getState();

// Apply gate
engine.applyHadamard(qubits, 0);

// Access amplitude
std::complex<double> amp = state(0);
std::cout << "Amplitude at |0⟩: " << amp << std::endl;

// Check probability
double prob = std::abs(amp) * std::abs(amp);
std::cout << "Probability: " << prob << std::endl;
```

### Error Handling

```cpp
try {
    engine.applyPauliX(qubits, 10);  // Invalid index
} catch (const std::out_of_range& e) {
    std::cerr << "Error: " << e.what() << std::endl;
}

try {
    circuit.addGate("UNKNOWN_GATE", 0);
} catch (const std::invalid_argument& e) {
    std::cerr << "Error: " << e.what() << std::endl;
}
```

---

## Type Definitions

```cpp
// Complex number type (for amplitudes)
using Complex = std::complex<double>;

// State vector type (dimension 2^n)
using StateVector = Eigen::VectorXcd;

// Gate matrix type
using GateMatrix = Eigen::MatrixXcd;
```

---

## Thread Safety

**Note**: The current implementation is NOT thread-safe. Do not share QubitManager or CircuitManager instances across threads without external synchronization.

For multi-threaded use:
- Create separate QubitManager instances per thread
- Synchronize access if sharing CircuitManager

---

## Performance Notes

- Single-qubit gates: O(2^n) time complexity
- Multi-qubit gates: O(2^n) time complexity
- Circuit execution: O(m × 2^n) where m = number of gates
- Memory usage: O(2^n) for state vector (16 bytes per amplitude)

For n=5 qubits:
- Time per gate: ~0.5-2 microseconds
- Memory: ~512 bytes
- Peak throughput: ~500,000-2,000,000 gates/second
