#include "circuit_manager.h"
#include "qubit_manager.h"
#include <gtest/gtest.h>

// Test Quantum Circuit Execution
TEST(CircuitManagerTest, CircuitExecution) {
    QubitManager qubits(3);
    CircuitManager circuit;

    circuit.addGate("H", 0);
    circuit.addGate("CNOT", 0, 1);
    circuit.executeCircuit(qubits);

    EXPECT_NEAR(std::abs(qubits.getState()(0)), 1.0 / std::sqrt(2), 1e-6);
    EXPECT_NEAR(std::abs(qubits.getState()(3)), 1.0 / std::sqrt(2), 1e-6);
}
