#include "utils.h"
#include <iostream>
#include <bitset>

// Normalizes quantum state vector to unit norm
// @param state Reference to quantum state vector to normalize
void normalizeState(Eigen::VectorXcd& state) {
    double norm = state.norm();
    if (norm > NORM_TOLERANCE) {
        state /= norm;
    }
}

// Prints quantum state amplitudes above threshold in ket notation
// @param state Reference to const quantum state vector to display
void printState(const Eigen::VectorXcd& state) {
    int dimension = state.size();
    for (int i = 0; i < dimension; ++i) {
        // Only display amplitudes above noise threshold to improve readability
        if (std::abs(state(i)) > AMPLITUDE_DISPLAY_THRESHOLD) {
            std::cout << "| " << std::bitset<5>(i) << " ⟩ : " << state(i) << std::endl;
        }
    }
}
