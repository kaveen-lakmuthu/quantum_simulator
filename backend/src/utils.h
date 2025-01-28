#ifndef UTILS_H
#define UTILS_H

#include <Eigen/Dense>

void printState(const Eigen::VectorXcd& state);
void normalizeState(Eigen::VectorXcd& state);

#endif
