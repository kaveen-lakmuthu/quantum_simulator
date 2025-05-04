#ifndef CIRCUIT_MANAGER_H
#define CIRCUIT_MANAGER_H

#include "qubit_manager.h"
#include "gate_engine.h"
#include <vector>
#include <string>
#include <stdexcept>

struct GateOperation {
    std::string gate_name;
    int target_qubit;
    int control_qubit1;
    int control_qubit2;
};

class CircuitManager {
private:
    std::vector<GateOperation> circuit;
    GateEngine gate_engine;

public:
    void addGate(const std::string& gate_name, int target_qubit, int control_qubit1 = -1, int control_qubit2 = -1);
    void executeCircuit(QubitManager& qubits);
    void printCircuit() const;
};

#endif