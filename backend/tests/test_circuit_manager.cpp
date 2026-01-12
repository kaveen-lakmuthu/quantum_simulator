#include "circuit_manager.h"
#include "qubit_manager.h"
#include <gtest/gtest.h>

// Test Quantum Circuit Execution
TEST(CircuitManagerTest, CircuitExecution) {
    QubitManager qubits(3);
    CircuitManager circuit;

    circuit.addGate("H", 0);
    // CNOT with control=0, target=1 (creates Bell state)
    circuit.addGate("CNOT", 1, 0);  // targetQubit=1, controlQubit1=0
    circuit.executeCircuit(qubits);

    EXPECT_NEAR(std::abs(qubits.getState()(0)), 1.0 / std::sqrt(2), 1e-6);
    EXPECT_NEAR(std::abs(qubits.getState()(3)), 1.0 / std::sqrt(2), 1e-6);
}
