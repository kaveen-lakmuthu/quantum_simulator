#include "backend_bridge.h"
#include <QDebug>
#include <sstream>
#include <iomanip>
#include <bitset>

/// Constructs backend bridge with default 5-qubit system
BackendBridge::BackendBridge(QObject *parent)
    : QObject(parent), numQubits(5), circuit_executed(false) {
    qubits = new QubitManager(numQubits);
    initial_state = getQuantumState();
}

/// Destructor: cleans up quantum state manager
BackendBridge::~BackendBridge() {
    delete qubits;
}

/// Updates qubit count and resets quantum state
/// @param count New number of qubits (1-5)
void BackendBridge::setQubitCount(int count) {
    if (count < 1 || count > 5) {
        emit executionError("Qubit count must be between 1 and 5");
        return;
    }
    if (count == numQubits) return;

    numQubits = count;
    delete qubits;
    qubits = new QubitManager(numQubits);
    circuit = CircuitManager();  // Reset circuit
    circuit_executed = false;
    formatQuantumState();
    initial_state = getQuantumState();
    emit qubitCountChanged(count);
    emit quantumStateChanged(getQuantumState());
    emit initialStateChanged();
    emit circuitChanged(getCircuitDescription());
    emit circuitExecutedChanged();
}

/// Sets initial quantum state from binary string
/// @param stateString Binary representation (e.g., "01010")
void BackendBridge::setInitialState(const QString &stateString) {
    try {
        qubits->setInitialState(stateString.toStdString());
        formatQuantumState();
        emit quantumStateChanged(getQuantumState());
    } catch (const std::exception& e) {
        emit executionError(QString("Invalid initial state: %1").arg(e.what()));
    }
}

/// Adds quantum gate to circuit with optional control qubits
/// @param gateName Gate type (H, X, Y, Z, CNOT, SWAP, TOFFOLI)
/// @param target Target qubit index
/// @param control1 First control qubit (default: -1 = unused)
/// @param control2 Second control qubit (default: -1 = unused)
void BackendBridge::addGate(const QString &gateName, int target, int control1, int control2) {
    try {
        circuit.addGate(gateName.toStdString(), target, control1, control2);
        
        // Build human-readable gate description
        QString gateDesc;
        if (control1 >= 0 && control2 >= 0) {
            gateDesc = QString("%1(ctrl1=%2, ctrl2=%3, target=%4)")
                .arg(gateName).arg(control1).arg(control2).arg(target);
        } else if (control1 >= 0) {
            gateDesc = QString("%1(ctrl=%2, target=%3)")
                .arg(gateName).arg(control1).arg(target);
        } else {
            gateDesc = QString("%1(q%2)").arg(gateName).arg(target);
        }
        
        circuitGateList.append(gateDesc);
        emit circuitChanged(getCircuitDescription());
    } catch (const std::exception& e) {
        emit executionError(QString("Failed to add gate: %1").arg(e.what()));
    }
}

/// Adds measurement operation to circuit
/// @param target Qubit index to measure
void BackendBridge::addMeasurement(int target) {
    try {
        circuit.addGate("MEASURE", target);
        
        QString gateDesc = QString("MEASURE(q%1)").arg(target);
        circuitGateList.append(gateDesc);
        emit circuitChanged(getCircuitDescription());
    } catch (const std::exception& e) {
        emit executionError(QString("Failed to add measurement: %1").arg(e.what()));
    }
}

/// Executes circuit and updates quantum state
/// Resets to initial state before execution
void BackendBridge::executeCircuit() {
    try {
        // Reset to initial state before execution
        delete qubits;
        qubits = new QubitManager(numQubits);
        
        // Execute circuit
        delete qubits;
        qubits = new QubitManager(numQubits);
        initial_state = getQuantumState();
        emit initialStateChanged();
        
        circuit.executeCircuit(*qubits);
        circuit_executed = true;
        formatQuantumState();
        emit quantumStateChanged(getQuantumState());
        emit circuitExecutedChanged();
        emit executionSuccess();
    } catch (const std::exception& e) {
        emit executionError(QString("Execution failed: %1").arg(e.what()));
    }
}

/// Clears circuit and resets quantum state to |00...0⟩
void BackendBridge::clearCircuit() {
    circuit = CircuitManager();
    circuitGateList.clear();
    delete qubits;
    qubits = new QubitManager(numQubits);
    circuit_executed = false;
    formatQuantumState();
    initial_state = getQuantumState();
    emit circuitChanged(getCircuitDescription());
    emit quantumStateChanged(getQuantumState());
    emit initialStateChanged();
    emit circuitExecutedChanged();
}

/// Removes a gate from the circuit at specified index
/// @param index Gate index to remove (0-based)
void BackendBridge::removeGate(int index) {
    try {
        circuit.removeGate(index);
        
        // Update gate list
        circuitGateList.clear();
        for (int i = 0; i < circuit.getCircuitSize(); ++i) {
            const auto& gate = circuit.getGate(i);
            QString gateDesc;
            if (gate.control_qubit1 >= 0 && gate.control_qubit2 >= 0) {
                gateDesc = QString("%1(ctrl1=%2, ctrl2=%3, target=%4)")
                    .arg(QString::fromStdString(gate.gate_name))
                    .arg(gate.control_qubit1).arg(gate.control_qubit2).arg(gate.target_qubit);
            } else if (gate.control_qubit1 >= 0) {
                gateDesc = QString("%1(ctrl=%2, target=%3)")
                    .arg(QString::fromStdString(gate.gate_name))
                    .arg(gate.control_qubit1).arg(gate.target_qubit);
            } else {
                gateDesc = QString("%1(q%2)")
                    .arg(QString::fromStdString(gate.gate_name)).arg(gate.target_qubit);
            }
            circuitGateList.append(gateDesc);
        }
        
        circuit_executed = false;
        emit circuitChanged(getCircuitDescription());
        emit circuitExecutedChanged();
    } catch (const std::exception& e) {
        emit executionError(QString("Failed to remove gate: %1").arg(e.what()));
    }
}

/// Reorders a gate to a new position in the circuit
/// @param fromIndex Current index of gate (0-based)
/// @param toIndex Target index for gate (0-based)
void BackendBridge::reorderGates(int fromIndex, int toIndex) {
    try {
        circuit.reorderGates(fromIndex, toIndex);
        
        // Update gate list
        circuitGateList.clear();
        for (int i = 0; i < circuit.getCircuitSize(); ++i) {
            const auto& gate = circuit.getGate(i);
            QString gateDesc;
            if (gate.control_qubit1 >= 0 && gate.control_qubit2 >= 0) {
                gateDesc = QString("%1(ctrl1=%2, ctrl2=%3, target=%4)")
                    .arg(QString::fromStdString(gate.gate_name))
                    .arg(gate.control_qubit1).arg(gate.control_qubit2).arg(gate.target_qubit);
            } else if (gate.control_qubit1 >= 0) {
                gateDesc = QString("%1(ctrl=%2, target=%3)")
                    .arg(QString::fromStdString(gate.gate_name))
                    .arg(gate.control_qubit1).arg(gate.target_qubit);
            } else {
                gateDesc = QString("%1(q%2)")
                    .arg(QString::fromStdString(gate.gate_name)).arg(gate.target_qubit);
            }
            circuitGateList.append(gateDesc);
        }
        
        circuit_executed = false;
        emit circuitChanged(getCircuitDescription());
        emit circuitExecutedChanged();
    } catch (const std::exception& e) {
        emit executionError(QString("Failed to reorder gate: %1").arg(e.what()));
    }
}

/// Returns circuit size
/// @return Number of gates in circuit
int BackendBridge::getCircuitSize() const {
    return circuit.getCircuitSize();
}

/// Returns list of available qubit indices as strings
/// @return QStringList containing "0", "1", ..., "(n-1)"
QStringList BackendBridge::getAvailableQubits() const {
    QStringList qubits;
    for (int i = 0; i < numQubits; ++i) {
        qubits.append(QString::number(i));
    }
    return qubits;
}

/// Formats quantum state as human-readable string
/// Shows basis states with amplitudes above threshold
/// @return Formatted string with basis states and complex amplitudes
QString BackendBridge::getQuantumState() const {
    if (!qubits) return "No state";
    
    QString result;
    const auto& state = qubits->getState();
    
    for (int i = 0; i < state.size(); ++i) {
        if (std::abs(state(i)) > 1e-6) {
            std::bitset<5> bits(i);
            QString basisState = QString::fromStdString(bits.to_string()).right(numQubits);
            result += QString("| %1 ⟩ : (%2, %3)\n")
                .arg(basisState)
                .arg(state(i).real(), 0, 'f', 6)
                .arg(state(i).imag(), 0, 'f', 6);
        }
    }
    return result.isEmpty() ? "No states above threshold" : result;
}

/// Gets the initial quantum state before circuit execution
/// @return Formatted initial state string
QString BackendBridge::getInitialState() const {
    return initial_state;
}

/// Checks if circuit has been executed
/// @return True if circuit has been executed, false otherwise
bool BackendBridge::isCircuitExecuted() const {
    return circuit_executed;
}

/// Returns circuit description summary
/// @return String describing number of gates in circuit
QString BackendBridge::getCircuitDescription() const {
    if (circuitGateList.isEmpty()) {
        return "No gates added yet";
    }
    return QString("%1 gate(s) added").arg(circuitGateList.size());
}

/// Returns ordered list of gate descriptions
/// @return QStringList with human-readable gate descriptions
QStringList BackendBridge::getCircuitGates() const {
    return circuitGateList;
}

/// Returns current quantum state (alias for getQuantumState)
/// @return Formatted quantum state string
QString BackendBridge::getResults() const {
    return getQuantumState();
}

/// Formats complex amplitude as string with real and imaginary parts
/// @param amp Complex amplitude value
/// @return Formatted string "(real, imag i)"
QString BackendBridge::formatAmplitude(std::complex<double> amp) const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(4);
    oss << "(" << amp.real() << ", " << amp.imag() << "i)";
    return QString::fromStdString(oss.str());
}

/// Placeholder for future state formatting enhancements
void BackendBridge::formatQuantumState() {
    // Trigger state change notification
}
