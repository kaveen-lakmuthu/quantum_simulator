#include "gate_engine.h"
#include "qubit_manager.h"
#include <gtest/gtest.h>

// Test Pauli-X Gate
TEST(GateEngineTest, PauliX) {
    QubitManager qubits(3);
    GateEngine gateEngine;
    
    gateEngine.applyPauliX(qubits, 0);
    EXPECT_EQ(qubits.getState()(1), std::complex<double>(1, 0));
}

// Test Hadamard Gate
TEST(GateEngineTest, Hadamard) {
    QubitManager qubits(3);
    GateEngine gateEngine;
    
    gateEngine.applyHadamard(qubits, 1);
    EXPECT_NEAR(std::abs(qubits.getState()(0)), 1.0 / std::sqrt(2), 1e-6);
    EXPECT_NEAR(std::abs(qubits.getState()(2)), 1.0 / std::sqrt(2), 1e-6);
}

// Test CNOT Gate
TEST(GateEngineTest, CNOT) {
    QubitManager qubits(3);
    GateEngine gateEngine;

    gateEngine.applyHadamard(qubits, 0);
    gateEngine.applyCNOT(qubits, 0, 1);

    EXPECT_NEAR(std::abs(qubits.getState()(0)), 1.0 / std::sqrt(2), 1e-6);
    EXPECT_NEAR(std::abs(qubits.getState()(3)), 1.0 / std::sqrt(2), 1e-6);
}
