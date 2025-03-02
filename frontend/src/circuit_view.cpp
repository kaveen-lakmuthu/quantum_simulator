#include "circuit_view.h"
#include <QPainter>

CircuitView::CircuitView(QWidget *parent) : QWidget(parent), qubitCount(2) {}

void CircuitView::setQubitCount(int count) {
    qubitCount = count;
    update();  // Refresh the UI
}

void CircuitView::addGate(const std::string &gate, int target, int control) {
    circuitGates.push_back({gate, target, control});
    circuitManager.addGate(gate, target, control);
    update();  // Redraw circuit when a gate is added
}

void CircuitView::executeCircuit() {
    QubitManager qubitManager(qubitCount);
    circuitManager.executeCircuit(qubitManager);
}

void CircuitView::clearCircuit() {
    circuitGates.clear();
    circuitManager = CircuitManager();
    update();  // Clear the UI
}

// Draw Qubit Lines & Gates
void CircuitView::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int width = this->width();
    int height = this->height();
    int qubitSpacing = height / (qubitCount + 1);  // Spacing between qubits

    // 🎵 Draw Qubit Lines
    painter.setPen(QPen(Qt::black, 2));
    for (int i = 0; i < qubitCount; ++i) {
        int y = (i + 1) * qubitSpacing;
        painter.drawLine(20, y, width - 20, y);  // Horizontal qubit lines
    }

    // Draw Gates
    painter.setBrush(Qt::white);
    painter.setPen(QPen(Qt::blue, 2));

    int gateSpacing = 60;  // Distance between gates
    int xOffset = 40;  // Start drawing gates from this offset

    for (size_t i = 0; i < circuitGates.size(); ++i) {
        int y = (circuitGates[i].target + 1) * qubitSpacing;
        QRect gateRect(xOffset + i * gateSpacing, y - 15, 40, 30);
        painter.drawRect(gateRect);
        painter.drawText(gateRect, Qt::AlignCenter, QString::fromStdString(circuitGates[i].name));

        // Draw control dot for controlled gates (e.g., CNOT)
        if (circuitGates[i].control != -1) {
            int controlY = (circuitGates[i].control + 1) * qubitSpacing;
            painter.setBrush(Qt::black);
            painter.drawEllipse(QPoint(gateRect.center().x(), controlY), 5, 5);
        }
    }
}
