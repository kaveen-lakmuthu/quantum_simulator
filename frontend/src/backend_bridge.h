#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include "qubit_manager.h"
#include "gate_engine.h"
#include "circuit_manager.h"

/**
 * @class BackendBridge
 * @brief Exposes quantum simulator backend to QML frontend
 *
 * Provides a clean interface for QML code to interact with C++ quantum
 * simulation logic. Uses Qt's property/signal/slot mechanism for data binding.
 */
class BackendBridge : public QObject {
    Q_OBJECT
    Q_PROPERTY(int qubitCount READ getQubitCount WRITE setQubitCount NOTIFY qubitCountChanged)
    Q_PROPERTY(QString quantumState READ getQuantumState NOTIFY quantumStateChanged)
    Q_PROPERTY(QString initialState READ getInitialState NOTIFY initialStateChanged)
    Q_PROPERTY(QString circuitDescription READ getCircuitDescription NOTIFY circuitChanged)
    Q_PROPERTY(QStringList circuitGates READ getCircuitGates NOTIFY circuitChanged)
    Q_PROPERTY(bool circuitExecuted READ isCircuitExecuted NOTIFY circuitExecutedChanged)

public:
    explicit BackendBridge(QObject *parent = nullptr);
    ~BackendBridge();

    // Property accessors
    int getQubitCount() const { return numQubits; }
    QString getQuantumState() const;
    QString getInitialState() const;
    QString getCircuitDescription() const;
    QStringList getCircuitGates() const;
    bool isCircuitExecuted() const;
    Q_INVOKABLE int getMaxQubits() const { return 5; }
    Q_INVOKABLE int getMinQubits() const { return 1; }

    // QML-exposed methods
    Q_INVOKABLE void setQubitCount(int count);
    Q_INVOKABLE void setInitialState(const QString &stateString);
    Q_INVOKABLE void addGate(const QString &gateName, int target, int control1 = -1, int control2 = -1);
    Q_INVOKABLE void executeCircuit();
    Q_INVOKABLE void clearCircuit();
    Q_INVOKABLE QStringList getAvailableQubits() const;
    Q_INVOKABLE QString getResults() const;

signals:
    void qubitCountChanged(int newCount);
    void quantumStateChanged(const QString &state);
    void initialStateChanged();
    void circuitChanged(const QString &description);
    void circuitExecutedChanged();
    void executionError(const QString &errorMessage);
    void executionSuccess();

private:
    int numQubits;
    QubitManager *qubits;
    CircuitManager circuit;
    GateEngine gateEngine;
    QString lastError;
    QStringList circuitGateList;
    QString initial_state;
    bool circuit_executed;

    void formatQuantumState();
    QString formatAmplitude(std::complex<double> amp) const;
};
