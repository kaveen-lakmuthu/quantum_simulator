# Quantum Gates Reference

This document provides detailed mathematical and implementation information for all supported quantum gates.

## Single-Qubit Gates

### Hadamard Gate (H)

**Symbol**: H

**Matrix Representation**:
```
H = (1/√2) [ 1   1 ]
            [ 1  -1 ]
```

**Action**:
- |0⟩ → (|0⟩ + |1⟩)/√2
- |1⟩ → (|0⟩ - |1⟩)/√2

**Use Cases**:
- Create equal superposition
- Initialize Bell states
- Basis transformation

**Implementation**:
```cpp
void GateEngine::applyHadamard(QubitManager& qubits, int target) {
    // For each pair of basis states differing in target qubit
    // Apply H transformation: new_state = (state + state_flipped)/√2
}
```

**Example**:
```
Input:  |0⟩
Output: (|0⟩ + |1⟩)/√2  (equal superposition)
Amplitude: 1/√2 ≈ 0.707 for both |0⟩ and |1⟩
```

---

### Pauli-X Gate (X)

**Symbol**: X

**Matrix Representation**:
```
X = [ 0  1 ]
    [ 1  0 ]
```

**Action**:
- |0⟩ → |1⟩
- |1⟩ → |0⟩

**Use Cases**:
- Bit flip operation
- NOT gate in quantum computing
- Basis rotation

**Implementation**:
```cpp
void GateEngine::applyPauliX(QubitManager& qubits, int target) {
    // Swap amplitudes of states with target qubit 0 and 1
    int flipped_index = i ^ (1 << target);  // Flip bit at position target
    std::swap(state(i), state(flipped_index));
}
```

**Example**:
```
Input:  |1⟩
Output: |0⟩
```

---

### Pauli-Y Gate (Y)

**Symbol**: Y

**Matrix Representation**:
```
Y = [ 0  -i ]
    [ i   0 ]
```

**Action**:
- |0⟩ → i|1⟩
- |1⟩ → -i|0⟩

**Use Cases**:
- Rotation around Y-axis on Bloch sphere
- Complex phase manipulations
- Quantum algorithms requiring Y rotations

**Implementation**:
```cpp
void GateEngine::applyPauliY(QubitManager& qubits, int target) {
    // Swap amplitudes with phase modification
    // If target bit is 0: multiply by i
    // If target bit is 1: multiply by -i
    std::complex<double> i_phase(0, 1);  // i
    state(i) = -i_phase * state(flipped_index);
    state(flipped_index) = i_phase * temp;
}
```

**Example**:
```
Input:  |0⟩
Output: i|1⟩
Amplitude: i (imaginary unit)
```

---

### Pauli-Z Gate (Z)

**Symbol**: Z

**Matrix Representation**:
```
Z = [ 1   0 ]
    [ 0  -1 ]
```

**Action**:
- |0⟩ → |0⟩
- |1⟩ → -|1⟩

**Use Cases**:
- Phase flip operation
- Measure basis in computational basis
- Quantum error correction

**Implementation**:
```cpp
void GateEngine::applyPauliZ(QubitManager& qubits, int target) {
    // Apply phase -1 to states where target qubit is |1⟩
    if ((i >> target) & 1) {
        state(i) = -state(i);
    }
}
```

**Example**:
```
Input:  (|0⟩ + |1⟩)/√2
Output: (|0⟩ - |1⟩)/√2
Phase of |1⟩ component flipped
```

---

## Multi-Qubit Gates

### CNOT Gate (CX - Controlled-NOT)

**Symbol**: ⊕ with control line

**Matrix Representation** (for control=0, target=1):
```
CNOT = [ 1  0  0  0 ]
       [ 0  1  0  0 ]
       [ 0  0  0  1 ]
       [ 0  0  1  0 ]
```

**Action**:
- If control qubit = |0⟩: target unchanged
- If control qubit = |1⟩: apply X (bit flip) to target

**Use Cases**:
- Create entanglement (Bell states)
- Quantum error correction
- Quantum algorithms

**Implementation**:
```cpp
void GateEngine::applyCNOT(QubitManager& qubits, 
                           int control, int target) {
    // For each state where control bit is 1
    if ((i >> control) & 1) {
        // Flip target bit
        int target_index = i ^ (1 << target);
        std::swap(state(i), state(target_index));
    }
}
```

**Example - Bell State Creation**:
```
Input:       |00⟩
After H(0):  (|0⟩ + |1⟩)/√2 ⊗ |0⟩ = (|00⟩ + |10⟩)/√2
After CNOT:  (|00⟩ + |11⟩)/√2  (entangled Bell state)
```

---

### SWAP Gate

**Symbol**: ×

**Matrix Representation**:
```
SWAP = [ 1  0  0  0 ]
       [ 0  0  1  0 ]
       [ 0  1  0  0 ]
       [ 0  0  0  1 ]
```

**Action**:
- Exchange state of qubit1 and qubit2
- |qubit1, qubit2⟩ → |qubit2, qubit1⟩

**Use Cases**:
- Rearrange qubit order
- Circuit compilation
- Quantum algorithms

**Implementation**:
```cpp
void GateEngine::applySWAP(QubitManager& qubits, 
                           int qubit1, int qubit2) {
    // Check if qubits have different values
    if (((i >> qubit1) & 1) != ((i >> qubit2) & 1)) {
        // Swap both bits
        int swapped = (i ^ (1 << qubit1)) ^ (1 << qubit2);
        std::swap(state(i), state(swapped));
    }
}
```

**Example**:
```
Input:  |10⟩
Output: |01⟩
(qubit 0=1, qubit 1=0) → (qubit 0=0, qubit 1=1)
```

---

### Toffoli Gate (CCX - Controlled-Controlled-NOT)

**Symbol**: ⊕ with two control lines

**Action**:
- If both control qubits = |1⟩: apply X to target
- Otherwise: target unchanged

**Use Cases**:
- Universal quantum computation
- Complex quantum circuits
- Quantum algorithms

**Implementation**:
```cpp
void GateEngine::applyToffoli(QubitManager& qubits, 
                              int ctrl1, int ctrl2, int target) {
    // Check both control qubits
    if (((i >> ctrl1) & 1) && ((i >> ctrl2) & 1)) {
        int target_index = i ^ (1 << target);
        std::swap(state(i), state(target_index));
    }
}
```

**Example**:
```
Input:  |111⟩ (both controls and target = 1)
Output: |110⟩ (target flipped because both controls = 1)

Input:  |101⟩ (only one control = 1)
Output: |101⟩ (unchanged because not both controls = 1)
```

---

## Gate Properties

| Gate | Type | Qubits | Unitary | Invertible | Hermitian |
|------|------|--------|---------|------------|-----------|
| Hadamard | Single | 1 | Yes | Yes | Yes |
| Pauli-X | Single | 1 | Yes | Yes | Yes |
| Pauli-Y | Single | 1 | Yes | Yes | Yes |
| Pauli-Z | Single | 1 | Yes | Yes | Yes |
| CNOT | Multi | 2 | Yes | Yes | No |
| SWAP | Multi | 2 | Yes | Yes | Yes |
| Toffoli | Multi | 3 | Yes | Yes | No |

---

## Mathematical Notation

### Dirac Notation
- |0⟩: ground state (qubit = 0)
- |1⟩: excited state (qubit = 1)
- |ψ⟩: general quantum state
- ⟨ψ|φ⟩: inner product of states

### Superposition
- (|0⟩ + |1⟩)/√2: equal superposition of 0 and 1
- Amplitude 1/√2 means 50% probability for each state

### Entanglement
- (|00⟩ + |11⟩)/√2: Bell state (maximally entangled)
- Measurement of one qubit determines the other

### Phase
- e^(iθ): complex phase rotation
- i = √(-1): imaginary unit
- Global phases don't matter; relative phases do

---

## Common Circuits

### Bell State Creation
Create maximally entangled 2-qubit state:
```
1. H on qubit 0
2. CNOT with control=0, target=1
Result: (|00⟩ + |11⟩)/√2
```

### GHZ State (3-qubit entanglement)
```
1. H on qubit 0
2. CNOT control=0, target=1
3. CNOT control=0, target=2
Result: (|000⟩ + |111⟩)/√2
```

### Quantum Teleportation Setup
```
1. Prepare state to teleport on qubit 0
2. H on qubit 1
3. CNOT control=1, target=2
4. CNOT control=0, target=1
5. H on qubit 0
```

---

## Future Gates

Planned gate additions:
- **Phase Gate (S)**: Adds π/2 phase to |1⟩
- **T Gate**: Adds π/4 phase to |1⟩
- **Controlled-Z (CZ)**: Conditional phase flip
- **Rotation gates (RX, RY, RZ)**: Parameterized rotations
- **Measurement**: Collapse state to basis state

---

## References

- Nielsen & Chuang: "Quantum Computation and Quantum Information"
- Preskill: Quantum Computing course notes
- IBM Quantum: Gate definitions
