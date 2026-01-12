# Quantum Computing Fundamentals

This document provides the theoretical background for understanding the quantum circuit simulator.

## Table of Contents
1. [Qubits and Superposition](#qubits-and-superposition)
2. [Quantum State Representation](#quantum-state-representation)
3. [Quantum Gates](#quantum-gates)
4. [Entanglement](#entanglement)
5. [Measurement](#measurement)
6. [References](#references)

## Qubits and Superposition

### Classical vs Quantum Bits

A **classical bit** is binary: either 0 or 1.

A **quantum bit (qubit)** can exist in a **superposition** of both states simultaneously:

$$|\psi\rangle = \alpha|0\rangle + \beta|1\rangle$$

where:
- $|\psi\rangle$ is the quantum state (ket notation)
- $\alpha$ and $\beta$ are complex amplitudes
- $|0\rangle$ and $|1\rangle$ are basis states
- $|\alpha|^2 + |\beta|^2 = 1$ (normalization condition)

### Measurement

When a qubit is measured:
- Probability of measuring |0⟩ = $|\alpha|^2$
- Probability of measuring |1⟩ = $|\beta|^2$
- The state **collapses** to the measured value

**Example**: Qubit in state $\frac{1}{\sqrt{2}}|0\rangle + \frac{1}{\sqrt{2}}|1\rangle$
- 50% chance of measuring 0
- 50% chance of measuring 1

## Quantum State Representation

### Multi-Qubit Systems

For n qubits, the quantum state is a vector in a 2^n dimensional Hilbert space:

$$|\psi\rangle = \sum_{i=0}^{2^n-1} c_i |i\rangle$$

where $c_i$ are complex amplitudes and $\sum_i |c_i|^2 = 1$.

### Example: 2-Qubit State

$$|\psi\rangle = \frac{1}{2}|00\rangle + \frac{1}{2}|01\rangle + \frac{1}{2}|10\rangle + \frac{1}{2}|11\rangle$$

This represents equal superposition of all 4 possible states.

### Implementation in This Simulator

The simulator stores quantum states as dense vectors using Eigen's `VectorXcd`:
- 1 qubit: 2 complex amplitudes
- 2 qubits: 4 complex amplitudes
- 3 qubits: 8 complex amplitudes
- 5 qubits: 32 complex amplitudes

This is efficient for small systems (≤ 5 qubits) but scales poorly for larger systems (2^20+ amplitudes).

## Quantum Gates

Quantum gates are unitary transformations that manipulate quantum states. All gates preserve normalization.

### Single-Qubit Gates

#### Pauli-X (NOT Gate)
Bit flip: maps |0⟩ → |1⟩ and |1⟩ → |0⟩

$$X = \begin{pmatrix} 0 & 1 \\ 1 & 0 \end{pmatrix}$$

Applied to $\alpha|0\rangle + \beta|1\rangle$ gives $\beta|0\rangle + \alpha|1\rangle$

#### Pauli-Y
Phase and bit flip combined:

$$Y = \begin{pmatrix} 0 & -i \\ i & 0 \end{pmatrix}$$

Applied to $|0\rangle$ gives $i|1\rangle$, and to $|1\rangle$ gives $-i|0\rangle$

#### Pauli-Z
Phase flip:

$$Z = \begin{pmatrix} 1 & 0 \\ 0 & -1 \end{pmatrix}$$

Applied to $|0\rangle$ gives $|0\rangle$, and to $|1\rangle$ gives $-|1\rangle$

#### Hadamard Gate
Creates superposition:

$$H = \frac{1}{\sqrt{2}}\begin{pmatrix} 1 & 1 \\ 1 & -1 \end{pmatrix}$$

Applied to $|0\rangle$ gives $\frac{1}{\sqrt{2}}(|0\rangle + |1\rangle)$ (equal superposition)

Applied to $|1\rangle$ gives $\frac{1}{\sqrt{2}}(|0\rangle - |1\rangle)$

### Multi-Qubit Gates

#### CNOT (Controlled-NOT)
Conditionally applies X gate:
- If control qubit is |0⟩: target unchanged
- If control qubit is |1⟩: apply X to target

$$\text{CNOT} = \begin{pmatrix} 1 & 0 & 0 & 0 \\ 0 & 1 & 0 & 0 \\ 0 & 0 & 0 & 1 \\ 0 & 0 & 1 & 0 \end{pmatrix}$$

**Creating Bell States**: H on qubit 0, then CNOT(0→1) creates:
$$\frac{1}{\sqrt{2}}(|00\rangle + |11\rangle)$$

This is a maximally entangled state.

#### SWAP
Exchanges two qubits:

$$\text{SWAP} = \begin{pmatrix} 1 & 0 & 0 & 0 \\ 0 & 0 & 1 & 0 \\ 0 & 1 & 0 & 0 \\ 0 & 0 & 0 & 1 \end{pmatrix}$$

#### Toffoli (CCNOT - Controlled-Controlled-NOT)
Three-qubit gate: applies X to target if both control qubits are |1⟩

Useful for quantum arithmetic and computing classical reversible functions.

## Entanglement

**Entanglement** occurs when qubits cannot be described independently.

### Example: Bell State

The state $\frac{1}{\sqrt{2}}(|00\rangle + |11\rangle)$ is entangled because:
- Cannot write as product: $|\psi_1\rangle \otimes |\psi_2\rangle$
- Measuring qubit 0 instantly determines qubit 1's state
- Correlations exist even for distant qubits

### Creating Entanglement

```
Initial: |00⟩
Apply H to qubit 0: (|0⟩ + |1⟩)/√2 ⊗ |0⟩ = (|00⟩ + |10⟩)/√2
Apply CNOT(0→1): (|00⟩ + |11⟩)/√2   [Entangled!]
```

### Measurement of Entangled States

Measuring one qubit collapses the entire entangled state:
- Measure qubit 0 as |0⟩ → other qubit must be |0⟩
- Measure qubit 0 as |1⟩ → other qubit must be |1⟩

## Measurement

### Collapse to Basis States

Measurement forces a quantum state into one of the computational basis states with probability determined by the amplitudes.

### Repeated Measurements

Once measured, measuring again gives the same result (state was already collapsed).

### Multiple Qubit Measurement

For n-qubit systems, measurement returns an n-bit string with probability = $|c_i|^2$ where i corresponds to that bit string.

## Circuit Execution Model

The simulator executes circuits as follows:

1. **Initialize** quantum state (default: |00...0⟩)
2. **Apply gates** sequentially
3. **Normalize** state if needed
4. **Display** amplitudes above threshold (1e-10)

### Qubit Indexing (Little-Endian)

In this simulator, qubit 0 is the **least significant bit**:

```
State |q2 q1 q0⟩ with index = q0 * 2^0 + q1 * 2^1 + q2 * 2^2

|000⟩ = index 0
|001⟩ = index 1  (qubit 0 = 1)
|010⟩ = index 2  (qubit 1 = 1)
|100⟩ = index 4  (qubit 2 = 1)
```

This ordering is chosen for computational efficiency in gate operations.

## Key Quantum Properties

### Reversibility
All quantum gates are unitary (reversible). Applying gate twice returns original state.

**Exception**: Measurement is irreversible (collapses superposition).

### Superposition Principle
Linear combinations of quantum states are valid states:

$$|\psi\rangle = a|\phi\rangle + b|\chi\rangle$$

### Quantum Interference
Amplitudes can add (constructive) or cancel (destructive):

$$(\alpha_1|0\rangle + \alpha_2|1\rangle) + (\beta_1|0\rangle + \beta_2|1\rangle) = (\alpha_1+\beta_1)|0\rangle + (\alpha_2+\beta_2)|1\rangle$$

Quantum algorithms exploit interference to amplify correct answers and cancel wrong ones.

## Computational Complexity

### Exponential Growth
- Classical: n bits = 2^n possible values (store one at a time)
- Quantum: n qubits = 2^n amplitudes (process all simultaneously)

This is the basis of quantum computing's potential speedup.

### Practical Limits
This simulator uses dense state vectors (2^n amplitudes), practical for:
- ✓ 1-5 qubits (2-32 amplitudes)
- ✗ 20+ qubits (>1 million amplitudes)
- ✗ 300 qubits (> atoms in universe)

Larger systems require sparse representations or specialized algorithms.

## Further Reading

### Recommended Resources
1. **"Quantum Computing in the NISQ era and beyond"** - Preskill, 2018
2. **"Quantum Computation and Quantum Information"** - Nielsen & Chuang
3. **IBM Quantum Learning** - https://quantum-computing.ibm.com/composer/docs/iqx/guide/

### Concepts to Explore Next
- Quantum algorithms (Deutsch, Grover, Shor)
- Quantum error correction
- Variational quantum algorithms (VQE, QAOA)
- Quantum machine learning

## References

- Dirac notation and quantum mechanics fundamentals
- Quantum Computing: An Overview Across the Computing Stack - Bichsel et al., 2021
- The Quirk quantum circuit simulator - https://algassert.com/quirk
