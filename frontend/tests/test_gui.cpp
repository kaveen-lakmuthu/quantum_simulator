#include <QtTest/QtTest>
#include <QCoreApplication>
#include "src/backend_bridge.h"

/// @file test_gui.cpp
/// @brief Unit tests for BackendBridge QML interface
///
/// Tests the C++/QML bridge layer that exposes quantum backend functionality
/// to the QML frontend. Validates property bindings, signal emissions, and
/// quantum operation execution through the bridge.

class BackendBridgeTest : public QObject {
    Q_OBJECT
private slots:
    void initTestCase() {
        // Backend initializes with 5 qubits in |00000⟩ state
    }

    void testQubitCountProperty() {
        BackendBridge bridge;
        QCOMPARE(bridge.getQubitCount(), 5);
    }

    void testSetQubitCountValid() {
        BackendBridge bridge;
        bridge.setQubitCount(3);
        QCOMPARE(bridge.getQubitCount(), 3);
    }

    void testSetQubitCountInvalid() {
        BackendBridge bridge;
        bridge.setQubitCount(0);  // Out of range
        QCOMPARE(bridge.getQubitCount(), 5);  // Should remain unchanged
        bridge.setQubitCount(6);  // Out of range
        QCOMPARE(bridge.getQubitCount(), 5);  // Should remain unchanged
    }

    void testInitialStateTracking() {
        BackendBridge bridge;
        bridge.setQubitCount(2);
        QString initial = bridge.getInitialState();
        QVERIFY(initial.contains("| 00 ⟩"));
        QCOMPARE(bridge.isCircuitExecuted(), false);
    }

    void testAddGateAndExecuteBellState() {
        BackendBridge bridge;
        bridge.setQubitCount(2);
        
        // Build Bell state circuit
        bridge.addGate("H", 0);
        bridge.addGate("CNOT", 1, 0);
        
        QStringList gates = bridge.getCircuitGates();
        QCOMPARE(gates.size(), 2);
        
        // Execute circuit
        bridge.executeCircuit();
        QCOMPARE(bridge.isCircuitExecuted(), true);
        
        // Check final state contains both |00⟩ and |11⟩
        QString finalState = bridge.getQuantumState();
        QVERIFY(finalState.contains("| 00 ⟩"));
        QVERIFY(finalState.contains("| 11 ⟩"));
    }

    void testClearCircuit() {
        BackendBridge bridge;
        bridge.setQubitCount(2);
        bridge.addGate("H", 0);
        
        QStringList before = bridge.getCircuitGates();
        QCOMPARE(before.size(), 1);
        
        bridge.clearCircuit();
        
        QStringList after = bridge.getCircuitGates();
        QCOMPARE(after.size(), 0);
        QCOMPARE(bridge.isCircuitExecuted(), false);
    }

    void testExecutionFlow() {
        BackendBridge bridge;
        bridge.setQubitCount(1);
        
        // Get initial state
        QString initial = bridge.getInitialState();
        QVERIFY(!initial.isEmpty());
        
        // Add gate
        bridge.addGate("H", 0);
        QCOMPARE(bridge.isCircuitExecuted(), false);
        
        // Execute
        bridge.executeCircuit();
        QCOMPARE(bridge.isCircuitExecuted(), true);
        
        // Final state should be superposition (H gate on |0⟩)
        QString final = bridge.getQuantumState();
        QVERIFY(!final.isEmpty());
        QVERIFY(final.contains("⟩"));
    }

    void testAvailableQubits() {
        BackendBridge bridge;
        bridge.setQubitCount(3);
        
        QStringList qubits = bridge.getAvailableQubits();
        QCOMPARE(qubits.size(), 3);
        QCOMPARE(qubits[0], "0");
        QCOMPARE(qubits[1], "1");
        QCOMPARE(qubits[2], "2");
    }
};

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);
    BackendBridgeTest test;
    return QTest::qExec(&test, argc, argv);
}

#include "test_gui.moc"
