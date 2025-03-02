#ifndef UTILS_H
#define UTILS_H

#include <Eigen/Dense>

// Normalize the quantum state vector
void normalizeState(Eigen::VectorXcd& state);

// Print the quantum state
void printState(const Eigen::VectorXcd& state);

#endif
