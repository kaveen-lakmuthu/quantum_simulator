#ifndef CIRCUIT_VIEW_H
#define CIRCUIT_VIEW_H

#include <QWidget>
#include <vector>
#include <string>
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
    QString executeCircuit(const QString &initialState = "");
    void clearCircuit();
    void setQubitCount(int count);
    QString getCircuitState();
    int getQubitCount() const { return qubitCount; }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    std::string translateGateName(const std::string &guiName);
    CircuitManager circuitManager;
    int qubitCount;
    std::vector<QuantumGate> circuitGates;
};

#endif // CIRCUIT_VIEW_H