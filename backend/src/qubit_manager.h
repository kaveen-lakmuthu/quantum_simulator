#ifndef QUBIT_MANAGER_H
#define QUBIT_MANAGER_H

#include <Eigen/Dense>
#include <complex>

class QubitManager {
private:
    Eigen::VectorXcd state;  // Quantum state vector
    int num_qubits;          // Number of qubits

public:
    QubitManager(int num_qubits);
    void initializeZeroState();
    Eigen::VectorXcd& getState();              // Mutable access
    const Eigen::VectorXcd& getState() const;  // Read-only access
};

#endif
