#include "gate_engine.h"
#include <Eigen/Dense>
#include <iostream>
#include <stdexcept>
#include <cmath>

void GateEngine::validateQubitIndex(const QubitManager& qubits, int qubit) const {
    if (qubit < 0 || qubit >= qubits.getState().size()) {
        throw std::out_of_range("Qubit index out of range: " + std::to_string(qubit));
    }
}

void GateEngine::applyPauliX(QubitManager& qubits, int target_qubit) {
    validateQubitIndex(qubits, target_qubit);
    Eigen::VectorXcd& state = qubits.getState();
    int dimension = state.size();

    for (int i = 0; i < dimension; ++i) {
        int flipped_index = i ^ (1 << target_qubit);
        if (flipped_index > i) {
            std::swap(state(i), state(flipped_index));
        }
    }
}

void GateEngine::applyPauliY(QubitManager& qubits, int target_qubit) {
    validateQubitIndex(qubits, target_qubit);
    Eigen::VectorXcd& state = qubits.getState();
    int dimension = state.size();

    for (int i = 0; i < dimension; ++i) {
        if ((i >> target_qubit) & 1) {
            int swap_index = i ^ (1 << target_qubit);
            std::complex<double> phase(0, 1);
            std::swap(state(i), state(swap_index));
            state(swap_index) *= phase;
            state(i) *= -phase;
        }
    }
}

void GateEngine::applyPauliZ(QubitManager& qubits, int target_qubit) {
    validateQubitIndex(qubits, target_qubit);
    Eigen::VectorXcd& state = qubits.getState();
    int dimension = state.size();

    for (int i = 0; i < dimension; ++i) {
        if ((i >> target_qubit) & 1) {
            state(i) = -state(i);
        }
    }
}

void GateEngine::applyHadamard(QubitManager& qubits, int target_qubit) {
    validateQubitIndex(qubits, target_qubit);
    Eigen::VectorXcd& state = qubits.getState();
    int dimension = state.size();
    const double sqrt2 = 1.0 / std::sqrt(2.0);

    Eigen::VectorXcd new_state = Eigen::VectorXcd::Zero(dimension);

    for (int i = 0; i < dimension; ++i) {
        if ((i >> target_qubit) & 1) {
            new_state[i] = (state[i ^ (1 << target_qubit)] - state[i]) * sqrt2;
        } else {
            new_state[i] = (state[i] + state[i ^ (1 << target_qubit)]) * sqrt2;
        }
    }
    state = new_state;
}

// void GateEngine::applyCNOT(QubitManager& qubits, int control_qubit, int target_qubit) {
//     validateQubitIndex(qubits, control_qubit);
//     validateQubitIndex(qubits, target_qubit);
//     if (control_qubit == target_qubit) {
//         throw std::invalid_argument("Control and target qubits must be different");
//     }

//     Eigen::VectorXcd& state = qubits.getState();
//     int dimension = state.size();

//     for (int i = 0; i < dimension; ++i) {
//         if ((i >> control_qubit) & 1) {
//             int target_index = i ^ (1 << target_qubit);
//             std::swap(state[i], state[target_index]);
//         }
//     }
// }

// void GateEngine::applyCNOT(QubitManager& qubits, int control_qubit, int target_qubit) {
//     validateQubitIndex(qubits, control_qubit);
//     validateQubitIndex(qubits, target_qubit);
//     if (control_qubit == target_qubit) {
//         throw std::invalid_argument("Control and target qubits must be different");
//     }

//     Eigen::VectorXcd& state = qubits.getState();
//     int dimension = state.size();
//     Eigen::VectorXcd new_state = state; // Copy the current state

//     for (int i = 0; i < dimension; ++i) {
//         if ((i >> control_qubit) & 1) {  // If control qubit is |1⟩
//             int target_index = i ^ (1 << target_qubit);  // Flip target qubit
//             new_state[target_index] = state[i];  // Move amplitude
//             new_state[i] = state[target_index];  // Swap amplitudes
//         }
//     }

//     state = new_state;  // Update the state
// }

void GateEngine::applyCNOT(QubitManager& qubits, int control_qubit, int target_qubit) {
    validateQubitIndex(qubits, control_qubit);
    validateQubitIndex(qubits, target_qubit);
    if (control_qubit == target_qubit) return;

    Eigen::VectorXcd& state = qubits.getState();
    int dimension = state.size();
    Eigen::VectorXcd new_state = state;

    for (int i = 0; i < dimension; ++i) {
        if ((i >> control_qubit) & 1) {  // If control qubit is |1⟩
            int target_index = i ^ (1 << target_qubit);  // Flip target qubit
            new_state[target_index] = state[i];
            new_state[i] = state[target_index];
        }
    }

    state = new_state;
}

// void GateEngine::applySWAP(QubitManager& qubits, int qubit1, int qubit2) {
//     validateQubitIndex(qubits, qubit1);
//     validateQubitIndex(qubits, qubit2);
//     if (qubit1 == qubit2) return;

//     Eigen::VectorXcd& state = qubits.getState();
//     int dimension = state.size();

//     for (int i = 0; i < dimension; ++i) {
//         int swapped_index = (i ^ (1 << qubit1)) ^ (1 << qubit2);
//         if (swapped_index > i) {
//             std::swap(state(i), state(swapped_index));
//         }
//     }
// }

void GateEngine::applySWAP(QubitManager& qubits, int qubit1, int qubit2) {
    validateQubitIndex(qubits, qubit1);
    validateQubitIndex(qubits, qubit2);
    if (qubit1 == qubit2) return;

    Eigen::VectorXcd& state = qubits.getState();
    int dimension = state.size();

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

void GateEngine::applyToffoli(QubitManager& qubits, int control1, int control2, int target_qubit) {
    validateQubitIndex(qubits, control1);
    validateQubitIndex(qubits, control2);
    validateQubitIndex(qubits, target_qubit);
    if (control1 == control2 || control1 == target_qubit || control2 == target_qubit) {
        throw std::invalid_argument("Toffoli gate requires distinct qubits");
    }

    Eigen::VectorXcd& state = qubits.getState();
    int dimension = state.size();

    for (int i = 0; i < dimension; ++i) {
        if (((i >> control1) & 1) && ((i >> control2) & 1)) {
            int target_index = i ^ (1 << target_qubit);
            std::swap(state(i), state(target_index));
        }
    }
}