#include "qubit_manager.h"
#include "utils.h"  // Include utils.h for normalizeState
#include <gtest/gtest.h>

// Test Initialization of QubitManager
TEST(QubitManagerTest, Initialization) {
    QubitManager qubits(5);
    Eigen::VectorXcd state = qubits.getState();
    
    // Verify |00000⟩ state is correctly initialized
    EXPECT_EQ(state.size(), 32);
    EXPECT_EQ(state(0), std::complex<double>(1.0, 0.0));
}

// Test State Normalization
TEST(QubitManagerTest, Normalization) {
    QubitManager qubits(5);
    Eigen::VectorXcd& state = qubits.getState();
    
    // Modify state arbitrarily
    state(0) = std::complex<double>(0.5, 0.5);
    state(1) = std::complex<double>(0.5, -0.5);
    
    // Normalize state
    normalizeState(state);

    // Check that sum of squared magnitudes equals 1
    double norm = state.squaredNorm();
    EXPECT_NEAR(norm, 1.0, 1e-6);
}
