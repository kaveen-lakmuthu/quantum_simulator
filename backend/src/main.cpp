#include <iostream>
#include "qubit_manager.h"
#include "gate_engine.h"
#include "circuit_manager.h"

int main() {
    QubitManager qubits(5);
    CircuitManager circuit;
    
    std::cout << "Initial Quantum State:\n";
    qubits.printState();

    // Define quantum circuit
    circuit.addGate("H", 0);        // Apply Hadamard to qubit 0
    circuit.addGate("CNOT", 1, 0);  // CNOT with control 0, target 1
    circuit.addGate("X", 2);        // Apply Pauli-X to qubit 2
    circuit.addGate("SWAP", 3, 4);  // Swap qubits 3 and 4
    
    // Print and execute circuit
    std::cout << "\nDefined Quantum Circuit:\n";
    circuit.printCircuit();

    std::cout << "\nExecuting Quantum Circuit...\n";
    circuit.executeCircuit(qubits);
    
    // Print final state
    qubits.printState();

    return 0;
}
