#include "qubit_manager.h"

QubitManager::QubitManager(int num_qubits) : num_qubits(num_qubits) {
    int dimension = 1 << num_qubits; // 2^num_qubits
    state = Eigen::VectorXcd(dimension);
    state.setZero();
}

void QubitManager::initializeZeroState() {
    state.setZero();
    state(0) = std::complex<double>(1.0, 0.0); // |000...0>
}

Eigen::VectorXcd& QubitManager::getState() {
    return state; // Return by reference
}

const Eigen::VectorXcd& QubitManager::getState() const {
    return state; // Return by reference (read-only)
}
