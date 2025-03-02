#ifndef CIRCUIT_VIEW_H
#define CIRCUIT_VIEW_H

#include <QWidget>
#include <vector>
#include "../backend/src/circuit_manager.h"

struct QuantumGate {
    std::string name;
    int target;
    int control;
};

class CircuitView : public QWidget {
    Q_OBJECT

public:
    explicit CircuitView(QWidget *parent = nullptr);
    void addGate(const std::string &gate, int target, int control = -1);
    void executeCircuit();
    void clearCircuit();
    void setQubitCount(int count);

protected:
    void paintEvent(QPaintEvent *event) override;  // Handles drawing the circuit

private:
    CircuitManager circuitManager;
    int qubitCount;
    std::vector<QuantumGate> circuitGates;  // Store gates for visualization
};

#endif // CIRCUIT_VIEW_H
