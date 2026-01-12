#pragma once

#include <Eigen/Dense>
#include <complex>
#include <string>

/**
 * @class QubitManager
 * @brief Manages quantum state vectors and qubit operations
 * 
 * Handles initialization, storage, and retrieval of quantum states
 * using Eigen's complex vector representation. Supports states up to 5 qubits.
 * 
 * @note Thread-safe for read operations; not thread-safe for state modifications
 */
class QubitManager {
public:
    /// Maximum supported qubits
    static constexpr int MAX_QUBITS = 5;
    
    /// Maximum state dimension (2^MAX_QUBITS)
    static constexpr int MAX_STATE_DIMENSION = 32;

    /**
     * @brief Constructs QubitManager with specified number of qubits
     * @param numQubits Number of qubits (1-5)
     * @throws std::invalid_argument if numQubits out of valid range
     */
    explicit QubitManager(int numQubits);

    /**
     * @brief Initializes quantum state to |00...0⟩ (ground state)
     */
    void initializeZeroState();

    /**
     * @brief Returns mutable reference to quantum state vector
     * @return Reference to state vector with complex amplitudes
     */
    Eigen::VectorXcd& getState();

    /**
     * @brief Returns immutable reference to quantum state vector
     * @return Const reference to state vector
     */
    const Eigen::VectorXcd& getState() const;

    /**
     * @brief Gets number of qubits in this manager
     * @return Number of qubits (1-5)
     */
    int getNumQubits() const;

    /**
     * @brief Prints all non-zero amplitudes to stdout
     * 
     * Format: | binary_state ⟩ : amplitude
     * Only amplitudes with magnitude > 1e-10 are displayed
     */
    void printState() const;

    /**
     * @brief Sets quantum state from binary string representation
     * @param stateString Binary string (e.g., "00101")
     * @throws std::invalid_argument if string length != num_qubits
     * @throws std::invalid_argument if string contains non-binary characters
     * 
     * Example: setInitialState("101") creates state |101⟩
     */
    void setInitialState(const std::string& stateString);

private:
    /// Quantum state vector with complex amplitudes
    Eigen::VectorXcd state;
    
    /// Number of qubits managed (1-5)
    int num_qubits;
    
    /// Amplitude threshold for display (1e-10)
    static constexpr double AMPLITUDE_THRESHOLD = 1e-10;
};
