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
    /// Gate identifier ("H", "X", "Y", "Z", "CNOT", "SWAP", "TOFFOLI", "MEASURE")
    std::string gate_name;
    
    /// Primary target qubit index
    int target_qubit;
    
    /// First control qubit index (-1 if unused)
    int control_qubit1;
    
    /// Second control qubit index (-1 if unused)
    int control_qubit2;
    
    /// Measurement result (-1 if not yet measured, 0 or 1 if measured)
    mutable int measurement_result = -1;
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
     * @param gateName Gate identifier ("H", "X", "Y", "Z", "CNOT", "SWAP", "TOFFOLI", "MEASURE")
     * @param targetQubit Target qubit index (0-based, required for all gates)
     * @param controlQubit1 First control qubit (default: -1 = unused)
     * @param controlQubit2 Second control qubit (default: -1 = unused)
     * @throws std::invalid_argument if gate_name unknown or target_qubit < 0
     * 
     * Single-qubit gates: use targetQubit only (H, X, Y, Z, MEASURE)
     * Two-qubit gates: use controlQubit1 and targetQubit (CNOT, SWAP)
     * Three-qubit gates: use controlQubit1, controlQubit2, and targetQubit (TOFFOLI)
     */
    void addGate(const std::string& gateName, int targetQubit, 
                 int controlQubit1 = -1, int controlQubit2 = -1);

    /**
     * @brief Removes a gate from the circuit at specified index
     * @param index Index of gate to remove (0-based)
     * @throws std::out_of_range if index >= circuit size
     * 
     * Removes the gate at the specified position and shifts remaining gates.
     */
    void removeGate(int index);

    /**
     * @brief Reorders a gate to a new position in the circuit
     * @param fromIndex Current index of gate to move (0-based)
     * @param toIndex Desired index position (0-based)
     * @throws std::out_of_range if either index >= circuit size
     * 
     * Moves the gate from fromIndex to toIndex, shifting adjacent gates accordingly.
     * For example: reorderGates(2, 0) moves gate at position 2 to position 0.
     */
    void reorderGates(int fromIndex, int toIndex);

    /**
     * @brief Gets number of gates in circuit
     * @return Number of gates currently in circuit
     */
    int getCircuitSize() const { return circuit.size(); }

    /**
     * @brief Gets gate at specified index
     * @param index Gate index (0-based)
     * @return Const reference to GateOperation at index
     * @throws std::out_of_range if index >= circuit size
     */
    const GateOperation& getGate(int index) const;

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