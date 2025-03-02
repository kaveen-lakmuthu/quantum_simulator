#include "gate_engine.h"
#include <Eigen/Dense>
#include <iostream>

// Assembly function declaration
extern "C" {
    void parallel_apply_gate(double* real, double* imag, int qubit, int size);
}

// Helper function to convert `Eigen::VectorXcd` to raw double arrays
void convertToRaw(const Eigen::VectorXcd& state, std::vector<double>& real, std::vector<double>& imag) {
    int size = state.size();
    for (int i = 0; i < size; i++) {
        real[i] = state[i].real();
        imag[i] = state[i].imag();
    }
}

// Helper function to convert raw double arrays back to `Eigen::VectorXcd`
void convertFromRaw(Eigen::VectorXcd& state, const std::vector<double>& real, const std::vector<double>& imag) {
    int size = state.size();
    for (int i = 0; i < size; i++) {
        state[i] = std::complex<double>(real[i], imag[i]);
    }
}

// Apply Pauli-X (Bit Flip) Gate using Assembly
void GateEngine::applyPauliX(QubitManager& qubits, int target_qubit) {
    Eigen::VectorXcd& state = qubits.getState();
    int dimension = state.size();

    for (int i = 0; i < dimension; ++i) {
        int flipped_index = i ^ (1 << target_qubit);  // Flip the target qubit
        if (flipped_index > i) { 
            std::swap(state(i), state(flipped_index));
        }
    }
}


// Apply Pauli-Y Gate
void GateEngine::applyPauliY(QubitManager& qubits, int target_qubit) {
    int dimension = qubits.getState().size();
    Eigen::VectorXcd& state = qubits.getState();

    for (int i = 0; i < dimension; ++i) {
        if ((i >> target_qubit) & 1) {
            int swap_index = i ^ (1 << target_qubit);
            std::complex<double> phase(0, 1); // i (imaginary unit)
            state(swap_index) = phase * state(i);
            state(i) = -phase * state(swap_index);
        }
    }
}

// Apply Pauli-Z Gate
void GateEngine::applyPauliZ(QubitManager& qubits, int target_qubit) {
    int dimension = qubits.getState().size();
    Eigen::VectorXcd& state = qubits.getState();

    for (int i = 0; i < dimension; ++i) {
        if ((i >> target_qubit) & 1) {
            state(i) = -state(i); // Apply phase shift of -1
        }
    }
}

// Apply Hadamard Gate (Superposition)
void GateEngine::applyHadamard(QubitManager& qubits, int target_qubit) {
    int dimension = qubits.getState().size();
    Eigen::VectorXcd& state = qubits.getState();

    for (int i = 0; i < dimension; ++i) {
        if (!((i >> target_qubit) & 1)) { // If target qubit is 0
            int target_index = i ^ (1 << target_qubit);
            std::complex<double> temp = state(i);
            state(i) = (state(i) + state[target_index]) / std::sqrt(2);
            state(target_index) = (temp - state[target_index]) / std::sqrt(2);
        }
    }
}

// Controlled-NOT (CNOT) Gate: Flips target qubit if control qubit is |1⟩
void GateEngine::applyCNOT(QubitManager& qubits, int control_qubit, int target_qubit) {
    Eigen::VectorXcd& state = qubits.getState();
    int dimension = state.size();

    for (int i = 0; i < dimension; ++i) {
        if (((i >> control_qubit) & 1) == 1) {  // Control qubit is |1⟩
            int target_index = i ^ (1 << target_qubit);  // Flip target qubit
            std::swap(state(i), state(target_index));
        }
    }
}


// SWAP Gate: Swaps two qubits
void GateEngine::applySWAP(QubitManager& qubits, int qubit1, int qubit2) {
    if (qubit1 == qubit2) return;  // No need to swap if they're the same
    int dimension = qubits.getState().size();
    Eigen::VectorXcd& state = qubits.getState();

    for (int i = 0; i < dimension; ++i) {
        int swapped_index = (i ^ (1 << qubit1)) ^ (1 << qubit2);  // Flip both bits
        if (swapped_index > i) {
            std::swap(state(i), state(swapped_index));
        }
    }
}

// Toffoli (CCNOT) Gate: Flips target qubit if both control qubits are |1⟩
void GateEngine::applyToffoli(QubitManager& qubits, int control1, int control2, int target_qubit) {
    int dimension = qubits.getState().size();
    Eigen::VectorXcd& state = qubits.getState();

    for (int i = 0; i < dimension; ++i) {
        if (((i >> control1) & 1) == 1 && ((i >> control2) & 1) == 1) {  // If both control qubits are 1
            int target_index = i ^ (1 << target_qubit);  // Flip target qubit
            std::swap(state(i), state(target_index));
        }
    }
}