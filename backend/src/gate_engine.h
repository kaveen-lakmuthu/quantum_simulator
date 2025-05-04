#ifndef GATE_ENGINE_H
#define GATE_ENGINE_H

#include "qubit_manager.h"
#include <stdexcept>

class GateEngine {
public:
    // Single-qubit gates
    void applyPauliX(QubitManager& qubits, int target_qubit);
    void applyPauliY(QubitManager& qubits, int target_qubit);
    void applyPauliZ(QubitManager& qubits, int target_qubit);
    void applyHadamard(QubitManager& qubits, int target_qubit);

    // Multi-qubit gates
    void applyCNOT(QubitManager& qubits, int control_qubit, int target_qubit);
    void applySWAP(QubitManager& qubits, int qubit1, int qubit2);
    void applyToffoli(QubitManager& qubits, int control1, int control2, int target_qubit);

private:
    void validateQubitIndex(const QubitManager& qubits, int qubit) const;
};

#endif