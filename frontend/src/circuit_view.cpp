#include "circuit_view.h"
#include <QPainter>
#include <QDebug>
#include <bitset>
#include <stdexcept>
#include <unordered_map>
#include <cmath>

/// Initializes circuit view with default 2 qubits
CircuitView::CircuitView(QWidget *parent) : QWidget(parent), qubitCount(2) {}

/// Maps GUI gate names to backend gate identifiers
std::string CircuitView::translateGateName(const std::string &guiName) {
    // Gate name translation table (GUI name → Backend name)
    static const std::unordered_map<std::string, std::string> nameMap = {
        {"Hadamard", "H"}, 
        {"Pauli-X", "X"}, 
        {"Pauli-Y", "Y"}, 
        {"Pauli-Z", "Z"},
        {"CNOT", "CNOT"}, 
        {"SWAP", "SWAP"}, 
        {"TOFFOLI", "TOFFOLI"}
    };
    auto it = nameMap.find(guiName);
    return (it != nameMap.end()) ? it->second : guiName;
}

void CircuitView::setQubitCount(int count) {
    if (count < 1 || count > 5) {
        throw std::invalid_argument("Qubit count must be between 1 and 5");
    }
    qubitCount = count;
    update();  // Trigger repaint
}

void CircuitView::addGate(const std::string &gate, int target, int control) {
    // Validate target qubit index
    if (target < 0 || target >= qubitCount) {
        throw std::invalid_argument("Invalid target qubit index");
    }
    // Validate control qubit index (if used)
    if (control != -1 && (control < 0 || control >= qubitCount)) {
        throw std::invalid_argument("Invalid control qubit index");
    }
    
    // Translate GUI gate name to backend identifier
    std::string backendName = translateGateName(gate);
    circuitGates.push_back({gate, target, control});
    circuitManager.addGate(backendName, target, control);
    update();  // Trigger repaint
}

QString CircuitView::executeCircuit(const QString &initialState) {
    try {
        // Create quantum state manager
        QubitManager qubitManager(qubitCount);
        
        // Initialize with provided state if valid
        if (!initialState.isEmpty()) {
            if (initialState.length() != qubitCount) {
                throw std::invalid_argument(
                    "Initial state length must match qubit count");
            }
            qubitManager.setInitialState(initialState.toStdString());
        }
        
        // Execute all gates in circuit sequence
        circuitManager.executeCircuit(qubitManager);
        
        // Format output: |binary⟩: real + imag*i
        QString resultText;
        Eigen::VectorXcd state = qubitManager.getState();
        
        for (int i = 0; i < state.size(); ++i) {
            // Display amplitudes above numerical noise threshold
            if (std::abs(state[i]) > 1e-10) {
                // Extract binary representation for this amplitude index
                std::string binaryStr = std::bitset<32>(i).to_string();
                binaryStr = binaryStr.substr(32 - qubitCount);  // Get rightmost N bits
                
                resultText += QString("|%1⟩: %2 + %3i\n")
                    .arg(QString::fromStdString(binaryStr))
                    .arg(state[i].real(), 0, 'f', 4)
                    .arg(state[i].imag(), 0, 'f', 4);
            }
        }
        
        if (resultText.isEmpty()) {
            resultText = "All amplitudes are zero (state collapsed)";
        }
        
        return resultText;
    } catch (const std::exception& e) {
        return QString("Error: %1").arg(e.what());
    }
}

void CircuitView::clearCircuit() {
    circuitGates.clear();
    circuitManager = CircuitManager();
    update();
}

void CircuitView::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int width = this->width();
    int height = this->height();
    int qubitSpacing = height / (qubitCount + 1);

    // Draw Qubit Lines (horizontal quantum state lines)
    painter.setPen(QPen(Qt::black, 2));
    for (int i = 0; i < qubitCount; ++i) {
        int y = (i + 1) * qubitSpacing;
        painter.drawLine(20, y, width - 20, y);
    }

    // Draw Gate Boxes and Connections
    int gateSpacing = 60;
    int xOffset = 40;

    for (size_t i = 0; i < circuitGates.size(); ++i) {
        const auto& gate = circuitGates[i];
        int y = (gate.target + 1) * qubitSpacing;
        
        // Gate rectangle visualization
        painter.setPen(QPen(Qt::blue, 2));
        painter.setBrush(Qt::white);
        QRect gateRect(xOffset + i * gateSpacing, y - 15, 40, 30);
        painter.drawRect(gateRect);
        painter.drawText(gateRect, Qt::AlignCenter, QString::fromStdString(gate.name));

        // Draw control connection if multi-qubit gate
        if (gate.control != -1) {
            int controlY = (gate.control + 1) * qubitSpacing;
            painter.setPen(QPen(Qt::black, 1, Qt::DashLine));
            // Draw connection line from control to target
            painter.drawLine(gateRect.center().x(), controlY, 
                           gateRect.center().x(), y);
            
            // Draw control point (filled circle)
            painter.setBrush(Qt::black);
            painter.drawEllipse(QPoint(gateRect.center().x(), controlY), 5, 5);
        }
    }
}

QString CircuitView::getCircuitState() {
    return executeCircuit();
}