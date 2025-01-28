#include <iostream>
#include "qubit_manager.h"
#include "gate_engine.h"
#include "utils.h"

int main() {
    // Initialize a 5-qubit state
    QubitManager qubits(5);
    qubits.initializeZeroState();

    // Print the initial state
    std::cout << "Initial State:" << std::endl;
    printState(qubits.getState());

    // Apply Pauli-X gate to qubit 2
    GateEngine gates;
    gates.applyPauliX(qubits, 2);

    // Print the final state
    std::cout << "Final State after Pauli-X on qubit 2:" << std::endl;
    printState(qubits.getState());

    return 0;
}
