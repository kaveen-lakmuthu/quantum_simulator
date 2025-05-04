#include "circuit_manager.h"
#include <iostream>
#include <stdexcept>

void CircuitManager::addGate(const std::string& gate_name, int target_qubit, int control_qubit1, int control_qubit2) {
    if (target_qubit < 0) {
        throw std::invalid_argument("Target qubit index cannot be negative");
    }
    circuit.push_back({gate_name, target_qubit, control_qubit1, control_qubit2});
}

// void CircuitManager::executeCircuit(QubitManager& qubits) {
//     for (const auto& gate : circuit) {
//         try {
//             if (gate.gate_name == "X") {
//                 gate_engine.applyPauliX(qubits, gate.target_qubit);
//             } else if (gate.gate_name == "Y") {
//                 gate_engine.applyPauliY(qubits, gate.target_qubit);
//             } else if (gate.gate_name == "Z") {
//                 gate_engine.applyPauliZ(qubits, gate.target_qubit);
//             } else if (gate.gate_name == "H") {
//                 gate_engine.applyHadamard(qubits, gate.target_qubit);
//             } else if (gate.gate_name == "CNOT") {
//                 if (gate.control_qubit1 < 0) {
//                     throw std::invalid_argument("CNOT gate requires a control qubit");
//                 }
//                 gate_engine.applyCNOT(qubits, gate.control_qubit1, gate.target_qubit);
//             } else if (gate.gate_name == "SWAP") {
//                 gate_engine.applySWAP(qubits, gate.control_qubit1, gate.target_qubit);
//             } else if (gate.gate_name == "TOFFOLI") {
//                 gate_engine.applyToffoli(qubits, gate.control_qubit1, gate.control_qubit2, gate.target_qubit);
//             } else {
//                 throw std::invalid_argument("Unknown gate: " + gate.gate_name);
//             }
//         } catch (const std::exception& e) {
//             std::cerr << "Error executing gate " << gate.gate_name << ": " << e.what() << std::endl;
//             throw;
//         }
//     }
// }

void CircuitManager::executeCircuit(QubitManager& qubits) {
    for (const auto& gate : circuit) {
        try {
            std::string gate_name_upper;
            std::transform(gate.gate_name.begin(), gate.gate_name.end(),
                         std::back_inserter(gate_name_upper), ::toupper);
            
            if (gate_name_upper == "X" || gate_name_upper == "PAULI-X") {
                gate_engine.applyPauliX(qubits, gate.target_qubit);
            }
            else if (gate_name_upper == "Y" || gate_name_upper == "PAULI-Y") {
                gate_engine.applyPauliY(qubits, gate.target_qubit);
            }
            else if (gate_name_upper == "Z" || gate_name_upper == "PAULI-Z") {
                gate_engine.applyPauliZ(qubits, gate.target_qubit);
            }
            else if (gate_name_upper == "H" || gate_name_upper == "HADAMARD") {
                gate_engine.applyHadamard(qubits, gate.target_qubit);
            }
        } catch (const std::exception& e) {
            std::cerr << "Error executing gate " << gate.gate_name << ": " << e.what() << std::endl;
            throw;
        }
    }
}

void CircuitManager::printCircuit() const {
    std::cout << "Quantum Circuit:\n";
    for (const auto& gate : circuit) {
        if (gate.gate_name == "CNOT") {
            std::cout << gate.gate_name << " (Control: " << gate.control_qubit1 
                      << ", Target: " << gate.target_qubit << ")\n";
        } else if (gate.gate_name == "SWAP") {
            std::cout << gate.gate_name << " (Qubits: " << gate.control_qubit1 
                      << " <-> " << gate.target_qubit << ")\n";
        } else if (gate.gate_name == "TOFFOLI") {
            std::cout << gate.gate_name << " (Controls: " << gate.control_qubit1 
                      << ", " << gate.control_qubit2 << ", Target: " << gate.target_qubit << ")\n";
        } else {
            std::cout << gate.gate_name << " (Qubit " << gate.target_qubit << ")\n";
        }
    }
}