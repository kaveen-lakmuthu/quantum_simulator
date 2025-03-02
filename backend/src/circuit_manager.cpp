#include "circuit_manager.h"
#include <iostream>

// Add a gate to the circuit
void CircuitManager::addGate(const std::string& gate_name, int target_qubit, int control_qubit1, int control_qubit2) {
    circuit.push_back({gate_name, target_qubit, control_qubit1, control_qubit2});
}

// Execute the entire circuit
void CircuitManager::executeCircuit(QubitManager& qubits) {
    for (const auto& gate : circuit) {
        if (gate.gate_name == "X") {
            gate_engine.applyPauliX(qubits, gate.target_qubit);
        } else if (gate.gate_name == "Y") {
            gate_engine.applyPauliY(qubits, gate.target_qubit);
        } else if (gate.gate_name == "Z") {
            gate_engine.applyPauliZ(qubits, gate.target_qubit);
        } else if (gate.gate_name == "H") {
            gate_engine.applyHadamard(qubits, gate.target_qubit);
        } else if (gate.gate_name == "CNOT") {
            gate_engine.applyCNOT(qubits, gate.control_qubit1, gate.target_qubit);
        } else if (gate.gate_name == "SWAP") {
            gate_engine.applySWAP(qubits, gate.control_qubit1, gate.target_qubit);
        } else if (gate.gate_name == "TOFFOLI") {
            gate_engine.applyToffoli(qubits, gate.control_qubit1, gate.control_qubit2, gate.target_qubit);
        }
    }
}

// Print the circuit
void CircuitManager::printCircuit() const {
    std::cout << "Quantum Circuit:\n";
    for (const auto& gate : circuit) {
        if (gate.gate_name == "CNOT" || gate.gate_name == "SWAP") {
            std::cout << gate.gate_name << " (Control: " << gate.control_qubit1 
                      << ", Target: " << gate.target_qubit << ")\n";
        } else if (gate.gate_name == "TOFFOLI") {
            std::cout << gate.gate_name << " (Control1: " << gate.control_qubit1 
                      << ", Control2: " << gate.control_qubit2 << ", Target: " << gate.target_qubit << ")\n";
        } else {
            std::cout << gate.gate_name << " (Qubit " << gate.target_qubit << ")\n";
        }
    }
}
