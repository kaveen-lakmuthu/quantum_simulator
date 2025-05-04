#include "qubit_manager.h"
#include <iostream>
#include <bitset>
#include <stdexcept>

// Constructor: Initializes the quantum state
QubitManager::QubitManager(int num_qubits) : num_qubits(num_qubits) {
    int dimension = 1 << num_qubits; // 2^num_qubits (e.g., 32 for 5 qubits)
    state = Eigen::VectorXcd(dimension);
    initializeZeroState();
}

// Initializes the state to |00000⟩
void QubitManager::initializeZeroState() {
    state = Eigen::VectorXcd::Zero(1 << num_qubits);  // Ensure full allocation
    state(0) = std::complex<double>(1.0, 0.0);  // Set initial state to |00000⟩
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

// Sets an initial quantum state from a binary string (e.g., "00011")
void QubitManager::setInitialState(const std::string &stateString) {
    if (static_cast<int>(stateString.length()) != num_qubits) {
        std::cerr << "Error: Invalid initial state length! Expected " << num_qubits << " bits.\n";
        return;
    }

    try {
        int index = std::stoi(stateString, nullptr, 2);
        if (index >= (1 << num_qubits)) {  // Ensure index is within range
            throw std::out_of_range("Initial state index is out of range.");
        }
        state.setZero();
        state(index) = std::complex<double>(1.0, 0.0);
    } catch (const std::exception &e) {
        std::cerr << "Error: Invalid initial state format! " << e.what() << std::endl;
    }
}
