#pragma once

#include "qubit_manager.h"
#include "gate_engine.h"
#include <vector>
#include <string>
#include <stdexcept>

/**
 * @struct GateOperation
 * @brief Represents a single gate operation in a quantum circuit
 */
struct GateOperation {
    /// Gate identifier ("H", "X", "Y", "Z", "CNOT", "SWAP", "TOFFOLI")
    std::string gate_name;
    
    /// Primary target qubit index
    int target_qubit;
    
    /// First control qubit index (-1 if unused)
    int control_qubit1;
    
    /// Second control qubit index (-1 if unused)
    int control_qubit2;
};

/**
 * @class CircuitManager
 * @brief Builds and executes quantum circuits
 * 
 * Manages a sequence of quantum gate operations and coordinates
 * their execution through the GateEngine. Provides circuit building,
 * execution, and state inspection.
 * 
 * @note Gates are executed in the order they are added
 * @note Quantum state is modified in-place during execution
 */
class CircuitManager {
private:
    /// Sequence of gates to execute
    std::vector<GateOperation> circuit;
    
    /// Gate execution engine (mutable for const execution)
    mutable GateEngine gate_engine;

public:
    /**
     * @brief Adds a gate to the circuit
     * @param gateName Gate identifier ("H", "X", "Y", "Z", "CNOT", "SWAP", "TOFFOLI")
     * @param targetQubit Target qubit index (0-based, required for all gates)
     * @param controlQubit1 First control qubit (default: -1 = unused)
     * @param controlQubit2 Second control qubit (default: -1 = unused)
     * @throws std::invalid_argument if gate_name unknown or target_qubit < 0
     * 
     * Single-qubit gates: use targetQubit only
     * Two-qubit gates: use controlQubit1 and targetQubit
     * Three-qubit gates: use controlQubit1, controlQubit2, and targetQubit
     */
    void addGate(const std::string& gateName, int targetQubit, 
                 int controlQubit1 = -1, int controlQubit2 = -1);

    /**
     * @brief Executes all gates in circuit in sequence
     * @param qubits Reference to QubitManager (state will be modified)
     * @throws std::runtime_error if execution fails
     * @throws std::invalid_argument if gate or qubit invalid
     * 
     * Gates are applied in the order they were added.
     */
    void executeCircuit(QubitManager& qubits);

    /**
     * @brief Prints circuit information to stdout
     * 
     * Shows all gates in execution order with parameters.
     */
    void printCircuit() const;
};