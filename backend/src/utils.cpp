#include "utils.h"
#include <iostream>
#include <bitset>

// Normalize the quantum state vector
void normalizeState(Eigen::VectorXcd& state) {
    state /= state.norm();
}

// Print the quantum state in a readable format
void printState(const Eigen::VectorXcd& state) {
    int dimension = state.size();
    for (int i = 0; i < dimension; ++i) {
        if (std::abs(state(i)) > 1e-10) { // Ignore zero amplitudes
            std::cout << "| " << std::bitset<5>(i) << " ⟩ : " << state(i) << std::endl;
        }
    }
}
