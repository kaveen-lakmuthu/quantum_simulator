#include "qubit_manager.h"
#include <iostream>
#include <bitset>

// Constructor: Initializes the quantum state
QubitManager::QubitManager(int num_qubits) : num_qubits(num_qubits) {
    int dimension = 1 << num_qubits; // 2^num_qubits (32 for 5 qubits)
    state = Eigen::VectorXcd(dimension);
    initializeZeroState();
}

// Initializes the state to |00000⟩
void QubitManager::initializeZeroState() {
    state.setZero();  // Set all elements to 0
    state(0) = std::complex<double>(1.0, 0.0); // |00000⟩ state
}

// Returns a reference to the quantum state vector
Eigen::VectorXcd& QubitManager::getState() {
    return state;
}

// Returns a read-only reference to the quantum state vector
const Eigen::VectorXcd& QubitManager::getState() const {
    return state;
}

// Prints the quantum state in binary format
void QubitManager::printState() const {
    for (int i = 0; i < state.size(); ++i) {
        if (std::abs(state(i)) > 1e-10) { // Ignore near-zero amplitudes
            std::cout << "| " << std::bitset<5>(i) << " ⟩ : " << state(i) << std::endl;
        }
    }
}
