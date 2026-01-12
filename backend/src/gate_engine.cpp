#include "gate_engine.h"
#include <Eigen/Dense>
#include <iostream>
#include <stdexcept>
#include <cmath>

void GateEngine::validateQubitIndex(const QubitManager& qubits, int qubit) const {
    if (qubit < 0 || qubit >= qubits.getNumQubits()) {
        throw std::out_of_range("Qubit index out of range: " + std::to_string(qubit));
    }
}

void GateEngine::applyPauliX(QubitManager& qubits, int targetQubit) {
    validateQubitIndex(qubits, targetQubit);
    Eigen::VectorXcd& state = qubits.getState();
    int dimension = state.size();

    // Pauli-X (bit flip): swap amplitudes of basis states differing in target qubit
    for (int i = 0; i < dimension; ++i) {
        int flipped_index = i ^ (1 << targetQubit);
        if (flipped_index > i) {
            std::swap(state(i), state(flipped_index));
        }
    }
}

void GateEngine::applyPauliY(QubitManager& qubits, int targetQubit) {
    validateQubitIndex(qubits, targetQubit);
    Eigen::VectorXcd& state = qubits.getState();
    int dimension = state.size();

    // Pauli-Y gate: |0⟩ -> i|1⟩, |1⟩ -> -i|0⟩
    for (int i = 0; i < dimension; ++i) {
        int flipped_index = i ^ (1 << targetQubit);
        if (flipped_index > i) {  // Process each pair once
            std::complex<double> temp = state(i);
            state(i) = -IMAGINARY_UNIT * state(flipped_index);
            state(flipped_index) = IMAGINARY_UNIT * temp;
        }
    }
}

void GateEngine::applyPauliZ(QubitManager& qubits, int targetQubit) {
    validateQubitIndex(qubits, targetQubit);
    Eigen::VectorXcd& state = qubits.getState();
    int dimension = state.size();

    // Pauli-Z gate: applies -1 phase to |1⟩ states
    for (int i = 0; i < dimension; ++i) {
        if ((i >> targetQubit) & 1) {
            state(i) = -state(i);
        }
    }
}

void GateEngine::applyHadamard(QubitManager& qubits, int targetQubit) {
    validateQubitIndex(qubits, targetQubit);
    Eigen::VectorXcd& state = qubits.getState();
    int dimension = state.size();

    // Hadamard gate: creates superposition. H|0⟩ = (|0⟩+|1⟩)/√2, H|1⟩ = (|0⟩-|1⟩)/√2
    Eigen::VectorXcd new_state = Eigen::VectorXcd::Zero(dimension);

    for (int i = 0; i < dimension; ++i) {
        if ((i >> targetQubit) & 1) {
            new_state[i] = (state[i ^ (1 << targetQubit)] - state[i]) * INVERSE_SQRT2;
        } else {
            new_state[i] = (state[i] + state[i ^ (1 << targetQubit)]) * INVERSE_SQRT2;
        }
    }
    state = new_state;
}

void GateEngine::applyCNOT(QubitManager& qubits, int controlQubit, int targetQubit) {
    validateQubitIndex(qubits, controlQubit);
    validateQubitIndex(qubits, targetQubit);
    if (controlQubit == targetQubit) {
        throw std::invalid_argument("Control and target qubits must be different");
    }

    Eigen::VectorXcd& state = qubits.getState();
    int dimension = state.size();

    // CNOT gate: if control qubit is |1⟩, flip the target qubit
    for (int i = 0; i < dimension; ++i) {
        if ((i >> controlQubit) & 1) {
            int target_index = i ^ (1 << targetQubit);
            if (target_index > i) {  // Only swap once per pair
                std::swap(state(i), state(target_index));
            }
        }
    }
}

void GateEngine::applySWAP(QubitManager& qubits, int qubit1, int qubit2) {
    validateQubitIndex(qubits, qubit1);
    validateQubitIndex(qubits, qubit2);
    if (qubit1 == qubit2) {
        throw std::invalid_argument("SWAP gate requires distinct qubits");
    }

    Eigen::VectorXcd& state = qubits.getState();
    int dimension = state.size();

    // SWAP gate: exchange states of qubit1 and qubit2
    for (int i = 0; i < dimension; ++i) {
        // Only process if qubit1 and qubit2 have different values
        if (((i >> qubit1) & 1) != ((i >> qubit2) & 1)) {
            int swapped_index = (i ^ (1 << qubit1)) ^ (1 << qubit2);
            if (swapped_index > i) {  // Only swap once per pair
                std::swap(state(i), state(swapped_index));
            }
        }
    }
}

void GateEngine::applyToffoli(QubitManager& qubits, int control1, int control2, int targetQubit) {
    validateQubitIndex(qubits, control1);
    validateQubitIndex(qubits, control2);
    validateQubitIndex(qubits, targetQubit);
    if (control1 == control2 || control1 == targetQubit || control2 == targetQubit) {
        throw std::invalid_argument("Toffoli gate requires distinct qubits");
    }

    Eigen::VectorXcd& state = qubits.getState();
    int dimension = state.size();

    // Toffoli (CCX) gate: flip target if both controls are |1⟩
    for (int i = 0; i < dimension; ++i) {
        if (((i >> control1) & 1) && ((i >> control2) & 1)) {
            int target_index = i ^ (1 << targetQubit);
            std::swap(state(i), state(target_index));
        }
    }
}