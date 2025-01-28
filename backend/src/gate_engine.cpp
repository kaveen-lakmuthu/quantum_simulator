#include "gate_engine.h"
#include <Eigen/Dense>
#include <iostream>

void GateEngine::applyPauliX(QubitManager& qubits, int target_qubit) {
    Eigen::VectorXcd& state = qubits.getState();
    int dimension = state.size();

    // Full Pauli-X gate logic
    for (int i = 0; i < dimension; ++i) {
        if ((i >> target_qubit) & 1) {
            int swap_index = i ^ (1 << target_qubit); // Flip the target bit
            std::swap(state(i), state(swap_index));
        }
    }
}
