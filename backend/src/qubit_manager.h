#ifndef QUBIT_MANAGER_H
#define QUBIT_MANAGER_H

#include <Eigen/Dense>
#include <complex>

class QubitManager {
private:
    Eigen::VectorXcd state;  // Quantum state vector
    int num_qubits;          // Number of qubits

public:
    QubitManager(int num_qubits);  // Constructor
    void initializeZeroState();    // Sets state to |00000⟩
    Eigen::VectorXcd& getState();  // Returns reference to state vector
    const Eigen::VectorXcd& getState() const;  // Read-only access
    void printState() const;       // Prints the quantum state
};

#endif
