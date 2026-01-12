#include "circuit_manager.h"
#include <iostream>
#include <stdexcept>

// Adds a gate operation to the circuit queue
// @param gateName Name of the gate (X, Y, Z, H, CNOT, SWAP, TOFFOLI)
// @param targetQubit Index of target qubit
// @param controlQubit1 Index of first control qubit (for multi-qubit gates)
// @param controlQubit2 Index of second control qubit (for TOFFOLI)
void CircuitManager::addGate(const std::string& gateName, int targetQubit, 
                             int controlQubit1, int controlQubit2) {
    if (targetQubit < 0) {
        throw std::invalid_argument("Target qubit index cannot be negative");
    }
    circuit.push_back({gateName, targetQubit, controlQubit1, controlQubit2});
}

/// Removes a gate from the circuit at specified index
void CircuitManager::removeGate(int index) {
    if (index < 0 || index >= static_cast<int>(circuit.size())) {
        throw std::out_of_range("Gate index out of range: " + std::to_string(index));
    }
    circuit.erase(circuit.begin() + index);
}

/// Reorders a gate to a new position in the circuit
void CircuitManager::reorderGates(int fromIndex, int toIndex) {
    if (fromIndex < 0 || fromIndex >= static_cast<int>(circuit.size())) {
        throw std::out_of_range("From index out of range: " + std::to_string(fromIndex));
    }
    if (toIndex < 0 || toIndex >= static_cast<int>(circuit.size())) {
        throw std::out_of_range("To index out of range: " + std::to_string(toIndex));
    }
    
    if (fromIndex == toIndex) {
        return;  // No-op if source and destination are the same
    }
    
    // Extract the gate to move
    GateOperation gate = circuit[fromIndex];
    
    // Remove from current position
    circuit.erase(circuit.begin() + fromIndex);
    
    // Adjust toIndex if needed (because we removed an element)
    int adjustedToIndex = toIndex;
    if (fromIndex < toIndex) {
        adjustedToIndex--;
    }
    
    // Insert at new position
    circuit.insert(circuit.begin() + adjustedToIndex, gate);
}

/// Gets gate at specified index
const GateOperation& CircuitManager::getGate(int index) const {
    if (index < 0 || index >= static_cast<int>(circuit.size())) {
        throw std::out_of_range("Gate index out of range: " + std::to_string(index));
    }
    return circuit[index];
}

// Executes all gates in the circuit sequentially on the quantum state
// @param qubits Reference to QubitManager containing the quantum state
// @throws std::invalid_argument if gate name is invalid or required qubits missing
void CircuitManager::executeCircuit(QubitManager& qubits) {
    for (auto& gate : circuit) {
        try {
            // Convert gate name to uppercase for case-insensitive comparison
            std::string gateNameUpper;
            std::transform(gate.gate_name.begin(), gate.gate_name.end(),
                         std::back_inserter(gateNameUpper), ::toupper);
            
            // Single-qubit gates
            if (gateNameUpper == "X" || gateNameUpper == "PAULI-X") {
                gate_engine.applyPauliX(qubits, gate.target_qubit);
            }
            else if (gateNameUpper == "Y" || gateNameUpper == "PAULI-Y") {
                gate_engine.applyPauliY(qubits, gate.target_qubit);
            }
            else if (gateNameUpper == "Z" || gateNameUpper == "PAULI-Z") {
                gate_engine.applyPauliZ(qubits, gate.target_qubit);
            }
            else if (gateNameUpper == "H" || gateNameUpper == "HADAMARD") {
                gate_engine.applyHadamard(qubits, gate.target_qubit);
            }
            else if (gateNameUpper == "MEASURE") {
                gate.measurement_result = gate_engine.measureQubit(qubits, gate.target_qubit);
            }
            // Two-qubit gates
            else if (gateNameUpper == "CNOT") {
                if (gate.control_qubit1 < 0) {
                    throw std::invalid_argument("CNOT gate requires a control qubit");
                }
                gate_engine.applyCNOT(qubits, gate.control_qubit1, gate.target_qubit);
            }
            else if (gateNameUpper == "SWAP") {
                if (gate.control_qubit1 < 0) {
                    throw std::invalid_argument("SWAP gate requires two qubits");
                }
                gate_engine.applySWAP(qubits, gate.control_qubit1, gate.target_qubit);
            }
            // Three-qubit gates
            else if (gateNameUpper == "TOFFOLI") {
                if (gate.control_qubit1 < 0 || gate.control_qubit2 < 0) {
                    throw std::invalid_argument("TOFFOLI gate requires two control qubits");
                }
                gate_engine.applyToffoli(qubits, gate.control_qubit1, gate.control_qubit2, gate.target_qubit);
            }
            else {
                throw std::invalid_argument("Unknown gate: " + gate.gate_name);
            }
        } catch (const std::exception& e) {
            std::cerr << "Error executing gate " << gate.gate_name << ": " << e.what() << std::endl;
            throw;
        }
    }
}

// Prints the circuit structure with gate names and qubit indices
void CircuitManager::printCircuit() const {
    std::cout << "Quantum Circuit:\n";
    for (const auto& gate : circuit) {
        if (gate.gate_name == "CNOT") {
            // Two-qubit controlled gate
            std::cout << gate.gate_name << " (Control: " << gate.control_qubit1 
                      << ", Target: " << gate.target_qubit << ")\n";
        } else if (gate.gate_name == "SWAP") {
            // Qubit exchange operation
            std::cout << gate.gate_name << " (Qubits: " << gate.control_qubit1 
                      << " <-> " << gate.target_qubit << ")\n";
        } else if (gate.gate_name == "TOFFOLI") {
            // Three-qubit controlled gate
            std::cout << gate.gate_name << " (Controls: " << gate.control_qubit1 
                      << ", " << gate.control_qubit2 << ", Target: " << gate.target_qubit << ")\n";
        } else if (gate.gate_name == "MEASURE") {
            // Measurement operation
            std::cout << gate.gate_name << " (Qubit " << gate.target_qubit << ")";
            if (gate.measurement_result != -1) {
                std::cout << " -> Result: " << gate.measurement_result;
            }
            std::cout << "\n";
        } else {
            // Single-qubit gates
            std::cout << gate.gate_name << " (Qubit " << gate.target_qubit << ")\n";
        }
    }
}