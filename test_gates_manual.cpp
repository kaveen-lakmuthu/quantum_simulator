#include "backend/src/qubit_manager.h"
#include "backend/src/gate_engine.h"
#include "backend/src/circuit_manager.h"
#include <iostream>
#include <cmath>
#include <complex>

bool close_enough(double a, double b, double epsilon = 1e-6) {
    return std::abs(a - b) < epsilon;
}

bool close_enough(std::complex<double> a, std::complex<double> b, double epsilon = 1e-6) {
    return std::abs(a - b) < epsilon;
}

void test_pauli_x() {
    std::cout << "\n=== Testing Pauli-X Gate ===" << std::endl;
    QubitManager qubits(2);
    GateEngine engine;
    
    // Apply X to qubit 0: |00⟩ → |10⟩ (state index 0 → 1)
    engine.applyPauliX(qubits, 0);
    
    if (close_enough(qubits.getState()(1), std::complex<double>(1.0, 0.0))) {
        std::cout << "✓ Pauli-X test PASSED" << std::endl;
    } else {
        std::cout << "✗ Pauli-X test FAILED" << std::endl;
        qubits.printState();
    }
}

void test_pauli_y() {
    std::cout << "\n=== Testing Pauli-Y Gate ===" << std::endl;
    QubitManager qubits(2);
    GateEngine engine;
    
    // Apply Y to qubit 0: |00⟩ → i|10⟩
    engine.applyPauliY(qubits, 0);
    
    if (close_enough(qubits.getState()(1), std::complex<double>(0.0, 1.0))) {
        std::cout << "✓ Pauli-Y test PASSED" << std::endl;
    } else {
        std::cout << "✗ Pauli-Y test FAILED" << std::endl;
        std::cout << "Expected: (0, 1), Got: " << qubits.getState()(1) << std::endl;
        qubits.printState();
    }
}

void test_pauli_z() {
    std::cout << "\n=== Testing Pauli-Z Gate ===" << std::endl;
    QubitManager qubits(2);
    GateEngine engine;
    
    // First set to |10⟩
    engine.applyPauliX(qubits, 0);
    // Apply Z: |10⟩ → -|10⟩
    engine.applyPauliZ(qubits, 0);
    
    if (close_enough(qubits.getState()(1), std::complex<double>(-1.0, 0.0))) {
        std::cout << "✓ Pauli-Z test PASSED" << std::endl;
    } else {
        std::cout << "✗ Pauli-Z test FAILED" << std::endl;
        qubits.printState();
    }
}

void test_hadamard() {
    std::cout << "\n=== Testing Hadamard Gate ===" << std::endl;
    QubitManager qubits(2);
    GateEngine engine;
    
    // Apply H to qubit 0: |00⟩ → (|00⟩ + |10⟩)/√2
    engine.applyHadamard(qubits, 0);
    
    double expected = 1.0 / std::sqrt(2.0);
    if (close_enough(std::abs(qubits.getState()(0)), expected) &&
        close_enough(std::abs(qubits.getState()(1)), expected)) {
        std::cout << "✓ Hadamard test PASSED" << std::endl;
    } else {
        std::cout << "✗ Hadamard test FAILED" << std::endl;
        qubits.printState();
    }
}

void test_cnot() {
    std::cout << "\n=== Testing CNOT Gate ===" << std::endl;
    QubitManager qubits(2);
    GateEngine engine;
    
    // Create superposition on control: |00⟩ → (|00⟩ + |10⟩)/√2
    engine.applyHadamard(qubits, 0);
    
    // Apply CNOT(0,1): (|00⟩ + |10⟩)/√2 → (|00⟩ + |11⟩)/√2
    engine.applyCNOT(qubits, 0, 1);
    
    double expected = 1.0 / std::sqrt(2.0);
    // Should have amplitude at |00⟩ (index 0) and |11⟩ (index 3)
    if (close_enough(std::abs(qubits.getState()(0)), expected) &&
        close_enough(std::abs(qubits.getState()(3)), expected) &&
        close_enough(std::abs(qubits.getState()(1)), 0.0) &&
        close_enough(std::abs(qubits.getState()(2)), 0.0)) {
        std::cout << "✓ CNOT test PASSED (Bell state created)" << std::endl;
    } else {
        std::cout << "✗ CNOT test FAILED" << std::endl;
        qubits.printState();
    }
}

void test_swap() {
    std::cout << "\n=== Testing SWAP Gate ===" << std::endl;
    QubitManager qubits(2);
    GateEngine engine;
    
    // Set state to |10⟩
    engine.applyPauliX(qubits, 0);
    
    // Apply SWAP(0,1): |10⟩ → |01⟩
    engine.applySWAP(qubits, 0, 1);
    
    // |01⟩ is at index 2 in 2-qubit system
    if (close_enough(qubits.getState()(2), std::complex<double>(1.0, 0.0))) {
        std::cout << "✓ SWAP test PASSED" << std::endl;
    } else {
        std::cout << "✗ SWAP test FAILED" << std::endl;
        qubits.printState();
    }
}

void test_circuit_manager() {
    std::cout << "\n=== Testing CircuitManager ===" << std::endl;
    QubitManager qubits(3);
    CircuitManager circuit;
    
    // Create a simple circuit
    circuit.addGate("H", 0);
    circuit.addGate("CNOT", 1, 0);  // control=0, target=1
    circuit.addGate("X", 2);
    
    circuit.executeCircuit(qubits);
    
    // Expected: (|100⟩ + |111⟩)/√2
    double expected = 1.0 / std::sqrt(2.0);
    if (close_enough(std::abs(qubits.getState()(4)), expected) &&  // |100⟩
        close_enough(std::abs(qubits.getState()(7)), expected)) {  // |111⟩
        std::cout << "✓ CircuitManager test PASSED" << std::endl;
    } else {
        std::cout << "✗ CircuitManager test FAILED" << std::endl;
        qubits.printState();
    }
}

void test_qubit_validation() {
    std::cout << "\n=== Testing Qubit Validation ===" << std::endl;
    QubitManager qubits(3);  // 3 qubits (indices 0-2)
    GateEngine engine;
    
    bool caught_error = false;
    try {
        engine.applyPauliX(qubits, 5);  // Invalid index
    } catch (const std::out_of_range& e) {
        caught_error = true;
    }
    
    if (caught_error) {
        std::cout << "✓ Qubit validation test PASSED" << std::endl;
    } else {
        std::cout << "✗ Qubit validation test FAILED (no exception thrown)" << std::endl;
    }
}

int main() {
    std::cout << "=====================================" << std::endl;
    std::cout << "Quantum Gate Implementation Tests" << std::endl;
    std::cout << "=====================================" << std::endl;
    
    test_pauli_x();
    test_pauli_y();
    test_pauli_z();
    test_hadamard();
    test_cnot();
    test_swap();
    test_circuit_manager();
    test_qubit_validation();
    
    std::cout << "\n=====================================" << std::endl;
    std::cout << "All tests completed!" << std::endl;
    std::cout << "=====================================" << std::endl;
    
    return 0;
}
