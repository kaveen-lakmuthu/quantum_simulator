#pragma once

#include "qubit_manager.h"
#include <complex>
#include <stdexcept>

/**
 * @class GateEngine
 * @brief Applies quantum gates to qubit states
 * 
 * Implements single-qubit and multi-qubit quantum gates with proper
 * state transformations. All operations are performed in-place on the
 * QubitManager's state vector.
 * 
 * @note All gate operations modify state in-place
 * @note Qubit indices are 0-based from least significant qubit
 */
class GateEngine {
public:
    // Single-qubit gates

    /**
     * @brief Applies Pauli-X gate (bit flip): X|0⟩ = |1⟩, X|1⟩ = |0⟩
     * @param qubits Reference to QubitManager
     * @param targetQubit Target qubit index (0-based)
     * @throws std::out_of_range if qubit index out of valid range
     * 
     * Equivalent to classical NOT gate.
     */
    void applyPauliX(QubitManager& qubits, int targetQubit);

    /**
     * @brief Applies Pauli-Y gate: Y|0⟩ = i|1⟩, Y|1⟩ = -i|0⟩
     * @param qubits Reference to QubitManager
     * @param targetQubit Target qubit index (0-based)
     * @throws std::out_of_range if qubit index out of valid range
     * 
     * Rotation around Y-axis with phase component.
     */
    void applyPauliY(QubitManager& qubits, int targetQubit);

    /**
     * @brief Applies Pauli-Z gate: Z|0⟩ = |0⟩, Z|1⟩ = -|1⟩
     * @param qubits Reference to QubitManager
     * @param targetQubit Target qubit index (0-based)
     * @throws std::out_of_range if qubit index out of valid range
     * 
     * Phase-flip gate. Adds -1 phase to |1⟩ state.
     */
    void applyPauliZ(QubitManager& qubits, int targetQubit);

    /**
     * @brief Applies Hadamard gate: H|0⟩ = (|0⟩+|1⟩)/√2, H|1⟩ = (|0⟩-|1⟩)/√2
     * @param qubits Reference to QubitManager
     * @param targetQubit Target qubit index (0-based)
     * @throws std::out_of_range if qubit index out of valid range
     * 
     * Creates equal superposition states. Self-inverse: H² = I.
     */
    void applyHadamard(QubitManager& qubits, int targetQubit);

    // Multi-qubit gates

    /**
     * @brief Applies CNOT gate (controlled-NOT)
     * @param qubits Reference to QubitManager
     * @param controlQubit Control qubit index (0-based)
     * @param targetQubit Target qubit index (0-based)
     * @throws std::out_of_range if qubit indices out of valid range
     * @throws std::invalid_argument if control == target
     * 
     * Flips target qubit if control qubit is |1⟩. Creates entanglement (Bell states).
     */
    void applyCNOT(QubitManager& qubits, int controlQubit, int targetQubit);

    /**
     * @brief Applies SWAP gate
     * @param qubits Reference to QubitManager
     * @param qubit1 First qubit index (0-based)
     * @param qubit2 Second qubit index (0-based)
     * @throws std::out_of_range if qubit indices out of valid range
     * @throws std::invalid_argument if qubit1 == qubit2
     * 
     * Exchanges quantum states of two qubits.
     */
    void applySWAP(QubitManager& qubits, int qubit1, int qubit2);

    /**
     * @brief Applies Toffoli gate (controlled-controlled-NOT)
     * @param qubits Reference to QubitManager
     * @param control1 First control qubit index (0-based)
     * @param control2 Second control qubit index (0-based)
     * @param targetQubit Target qubit index (0-based)
     * @throws std::out_of_range if qubit indices out of valid range
     * @throws std::invalid_argument if any qubits are identical
     * 
     * Flips target if both control qubits are |1⟩. Universal for computation.
     */
    void applyToffoli(QubitManager& qubits, int control1, int control2, int targetQubit);

private:
    /// Unit imaginary number (0 + 1i)
    static constexpr std::complex<double> IMAGINARY_UNIT{0.0, 1.0};
    
    /// Reciprocal of square root 2 (1/√2 ≈ 0.707)
    static constexpr double INVERSE_SQRT2 = 0.7071067811865475;

    /**
     * @brief Validates qubit index is within valid range
     * @param qubits Reference to QubitManager
     * @param qubit Qubit index to validate
     * @throws std::out_of_range if qubit index >= num_qubits or < 0
     */
    void validateQubitIndex(const QubitManager& qubits, int qubit) const;
};