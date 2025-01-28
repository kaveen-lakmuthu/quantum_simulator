#ifndef GATE_ENGINE_H
#define GATE_ENGINE_H

#include "qubit_manager.h"

class GateEngine {
public:
    void applyPauliX(QubitManager& qubits, int target_qubit);
};

#endif
