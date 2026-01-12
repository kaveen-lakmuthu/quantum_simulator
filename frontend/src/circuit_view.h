#pragma once

#include <QWidget>
#include <vector>
#include <string>
#include "../backend/src/circuit_manager.h"

/**
 * @struct QuantumGate
 * @brief Represents a quantum gate operation in the circuit
 */
struct QuantumGate {
    std::string name;      ///< Gate name (e.g., "Hadamard", "CNOT")
    int target;            ///< Target qubit index (0-based)
    int control;           ///< Control qubit index (-1 if unused)
};

/**
 * @class CircuitView
 * @brief Visualizes and manages quantum circuits
 *
 * Provides circuit construction, gate management, and execution capabilities.
 * Displays quantum circuit visually and communicates with the backend simulator.
 */
class CircuitView : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructs the circuit view widget
     * @param parent Parent widget (default: nullptr)
     */
    explicit CircuitView(QWidget *parent = nullptr);
    
    /**
     * @brief Adds a gate operation to the circuit
     * @param gate Gate name (e.g., "H", "X", "CNOT")
     * @param target Target qubit index
     * @param control Control qubit index (-1 if not used)
     * @throws std::invalid_argument if qubit indices are invalid
     */
    void addGate(const std::string &gate, int target, int control = -1);
    
    /**
     * @brief Executes the quantum circuit
     * @param initialState Binary string representing initial state (e.g., "00", "10")
     * @return QString containing quantum state amplitudes
     * @throws std::runtime_error if execution fails
     */
    QString executeCircuit(const QString &initialState = "");
    
    /// Clears all gates from the circuit
    void clearCircuit();
    
    /**
     * @brief Sets the number of qubits
     * @param count Number of qubits (1-5)
     * @throws std::invalid_argument if count out of valid range
     */
    void setQubitCount(int count);
    
    /**
     * @brief Gets current circuit state representation
     * @return QString with circuit state information
     */
    QString getCircuitState();
    
    /**
     * @brief Gets current qubit count
     * @return Number of qubits
     */
    int getQubitCount() const { return qubitCount; }

protected:
    /**
     * @brief Handles painting of the circuit visualization
     * @param event Paint event from Qt
     */
    void paintEvent(QPaintEvent *event) override;

private:
    /**
     * @brief Translates GUI gate names to backend gate names
     * @param guiName Gate name from UI (e.g., "Hadamard")
     * @return Backend gate name (e.g., "H")
     */
    std::string translateGateName(const std::string &guiName);
    
    CircuitManager circuitManager;     ///< Backend circuit execution engine
    int qubitCount;                    ///< Current number of qubits
    std::vector<QuantumGate> circuitGates;  ///< List of gates in circuit
};