#include "qubit_manager.h"
#include <iostream>
#include <bitset>
#include <stdexcept>
#include <cmath>

// Constructor: Initializes quantum state to |00...0⟩
QubitManager::QubitManager(int numQubits) : num_qubits(numQubits) {
    if (numQubits < 1 || numQubits > MAX_QUBITS) {
        throw std::invalid_argument("Number of qubits must be between 1 and " + 
                                    std::to_string(MAX_QUBITS));
    }
    int dimension = 1 << numQubits;  // 2^numQubits
    state = Eigen::VectorXcd(dimension);
    initializeZeroState();
}

// Initializes state to |00...0⟩ (ground state)
void QubitManager::initializeZeroState() {
    state = Eigen::VectorXcd::Zero(1 << num_qubits);
    state(0) = std::complex<double>(1.0, 0.0);  // Set amplitude at |0...0⟩ to 1
}

// Returns a reference to the quantum state vector
Eigen::VectorXcd& QubitManager::getState() {
    return state;
}

// Returns a read-only reference to the quantum state vector
const Eigen::VectorXcd& QubitManager::getState() const {
    return state;
}

// Returns the number of qubits
int QubitManager::getNumQubits() const {
    return num_qubits;
}

// Prints quantum state amplitudes above threshold
void QubitManager::printState() const {
    for (int i = 0; i < state.size(); ++i) {
        // Only display amplitudes above threshold to avoid numerical noise
        if (std::abs(state(i)) > AMPLITUDE_THRESHOLD) {
            std::cout << "| " << std::bitset<5>(i) << " ⟩ : " << state(i) << std::endl;
        }
    }
}

// Sets quantum state from binary string (e.g., "00101")
void QubitManager::setInitialState(const std::string& stateString) {
    if (static_cast<int>(stateString.length()) != num_qubits) {
        throw std::invalid_argument("Initial state length (" + std::to_string(stateString.length()) +
                                    ") must match qubit count (" + std::to_string(num_qubits) + ")");
    }

    try {
        // Convert binary string to decimal index
        int index = std::stoi(stateString, nullptr, 2);
        if (index >= (1 << num_qubits)) {
            throw std::out_of_range("State index exceeds valid range for " +
                                    std::to_string(num_qubits) + " qubits");
        }
        state.setZero();
        state(index) = std::complex<double>(1.0, 0.0);
    } catch (const std::exception& e) {
        throw std::invalid_argument("Failed to parse initial state: " + std::string(e.what()));
    }
}
