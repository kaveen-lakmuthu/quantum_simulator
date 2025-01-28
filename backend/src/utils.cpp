#include "utils.h"
#include <iostream>
#include <bitset>

void printState(const Eigen::VectorXcd& state) {
    int dimension = state.size();

    for (int i = 0; i < dimension; ++i) {
        if (std::abs(state(i)) > 1e-10) { // Skip zero amplitudes
            std::cout << "| " << std::bitset<5>(i) << " > : " << state(i) << std::endl;
        }
    }
}

void normalizeState(Eigen::VectorXcd& state) {
    state /= state.norm();
}
