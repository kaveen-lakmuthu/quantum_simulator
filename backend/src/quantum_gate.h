#ifndef QUBIT_MANAGER_H
#define QUBIT_MANAGER_H

#include <Eigen/Dense>
#include <complex>
#include <string>

class QubitManager {
private:
    Eigen::VectorXcd state;
    int num_qubits;

public:
    explicit QubitManager(int num_qubits);
    void initializeZeroState();
    Eigen::VectorXcd& getState();
    const Eigen::VectorXcd& getState() const;
    void printState() const;
    void setInitialState(const std::string &stateStr);
    int getNumQubits() const { return num_qubits; }
};

#endif