#ifndef QUANTUM_GATE_H
#define QUANTUM_GATE_H

#include <string>

// Defines a quantum gate with a target qubit and optional control qubits
struct QuantumGate {
    std::string name;  // Name of the gate (Hadamard, CNOT, etc.)
    int target;        // The target qubit
    int control = -1;  // Control qubit (if applicable), default -1
    int control2 = -1; // Additional control for Toffoli, default -1
};

#endif // QUANTUM_GATE_H
