#include "circuit_painter.h"
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QRegularExpression>

/// Constructs circuit painter with default empty state
CircuitPainter::CircuitPainter(QQuickItem *parent)
    : QQuickPaintedItem(parent), numQubits(5) {
    setRenderTarget(QQuickPaintedItem::FramebufferObject);
}

/// Updates qubit count and triggers repaint
void CircuitPainter::setQubitCount(int count) {
    if (count != numQubits && count >= 1 && count <= 5) {
        numQubits = count;
        emit qubitCountChanged(count);
        update(); // Trigger repaint
    }
}

/// Updates gate list and triggers repaint
void CircuitPainter::setGates(const QStringList &gates) {
    gateList = gates;
    emit gatesChanged(gates);
    update(); // Trigger repaint
}

/// Main paint method: renders complete circuit diagram
void CircuitPainter::paint(QPainter *painter) {
    if (!painter) return;

    // Enable antialiasing for smooth lines and text
    painter->setRenderHint(QPainter::Antialiasing);
    
    // Fill background
    painter->fillRect(boundingRect(), QColor("#1e1e2e"));
    
    // Draw qubit lines
    drawQubitLines(painter);
    
    // Draw gates
    for (int i = 0; i < gateList.size(); ++i) {
        drawGate(painter, gateList[i], i);
    }
}

/// Draws horizontal lines representing qubits
void CircuitPainter::drawQubitLines(QPainter *painter) {
    QPen pen(QColor("#6c7086"), 2);
    painter->setPen(pen);
    
    int lineLength = LEFT_MARGIN + (gateList.size() + 1) * GATE_SPACING;
    if (lineLength < 400) lineLength = 400; // Minimum line length
    
    for (int i = 0; i < numQubits; ++i) {
        int y = TOP_MARGIN + i * QUBIT_SPACING;
        painter->drawLine(LEFT_MARGIN, y, lineLength, y);
        
        // Draw qubit label
        painter->setPen(QColor("#a6adc8"));
        QFont font = painter->font();
        font.setPointSize(10);
        painter->setFont(font);
        painter->drawText(10, y + 5, QString("|q%1⟩").arg(i));
        painter->setPen(pen);
    }
}

/// Renders individual gate symbol at specified position
void CircuitPainter::drawGate(QPainter *painter, const QString &gateDesc, int gateIndex) {
    QString gateName;
    int target = -1, control1 = -1, control2 = -1;
    
    parseGateDescription(gateDesc, gateName, target, control1, control2);
    
    if (target < 0 || target >= numQubits) return;
    
    int x = LEFT_MARGIN + (gateIndex + 1) * GATE_SPACING;
    int yTarget = TOP_MARGIN + target * QUBIT_SPACING;
    
    // Single-qubit gate
    if (control1 < 0) {
        // Determine box width based on gate name
        int boxWidth = 35;
        int boxHeight = 30;
        if (gateName == "MEASURE") {
            boxWidth = 55;
        }
        
        // Draw gate box
        painter->setPen(QPen(QColor("#89b4fa"), 2));
        painter->setBrush(QBrush(QColor("#313244")));
        painter->drawRect(x - boxWidth/2, yTarget - boxHeight/2, boxWidth, boxHeight);
        
        // Draw gate label
        painter->setPen(QColor("#cdd6f4"));
        QFont font = painter->font();
        font.setBold(true);
        if (gateName == "MEASURE") {
            font.setPointSize(7);
        } else {
            font.setPointSize(9);
        }
        painter->setFont(font);
        painter->drawText(QRect(x - boxWidth/2, yTarget - boxHeight/2, boxWidth, boxHeight), 
                         Qt::AlignCenter, gateName);
    }
    // Multi-qubit gate (CNOT, SWAP)
    else if (control1 >= 0 && control1 < numQubits) {
        int yControl = TOP_MARGIN + control1 * QUBIT_SPACING;
        
        // Draw control-target line
        painter->setPen(QPen(QColor("#f38ba8"), 2));
        painter->drawLine(x, qMin(yControl, yTarget), x, qMax(yControl, yTarget));
        
        // Draw control dot
        painter->setBrush(QBrush(QColor("#f38ba8")));
        painter->drawEllipse(QPoint(x, yControl), 5, 5);
        
        // Draw target symbol
        if (gateName == "CNOT") {
            // CNOT: circle with plus
            painter->setPen(QPen(QColor("#89b4fa"), 2));
            painter->setBrush(Qt::NoBrush);
            painter->drawEllipse(QPoint(x, yTarget), 12, 12);
            painter->drawLine(x - 8, yTarget, x + 8, yTarget);
            painter->drawLine(x, yTarget - 8, x, yTarget + 8);
        } else if (gateName == "SWAP") {
            // SWAP: X marks on both qubits
            painter->setPen(QPen(QColor("#89b4fa"), 2));
            painter->drawLine(x - 8, yTarget - 8, x + 8, yTarget + 8);
            painter->drawLine(x - 8, yTarget + 8, x + 8, yTarget - 8);
            painter->drawLine(x - 8, yControl - 8, x + 8, yControl + 8);
            painter->drawLine(x - 8, yControl + 8, x + 8, yControl - 8);
        }
    }
}

/// Parses gate description string (e.g., "H(q0)" or "CNOT(ctrl=0, target=1)")
void CircuitPainter::parseGateDescription(const QString &desc, QString &gateName,
                                         int &target, int &control1, int &control2) {
    // Reset outputs
    gateName.clear();
    target = control1 = control2 = -1;
    
    // Extract gate name (everything before opening parenthesis)
    int parenPos = desc.indexOf('(');
    if (parenPos > 0) {
        gateName = desc.left(parenPos);
    }
    
    // Parse qubit indices using regex
    // Pattern: q followed by digit, or "target=" followed by digit, etc.
    QRegularExpression qRe("q(\\d+)");
    QRegularExpression targetRe("target=(\\d+)");
    QRegularExpression ctrlRe("ctrl=(\\d+)");
    QRegularExpression ctrl1Re("ctrl1=(\\d+)");
    QRegularExpression ctrl2Re("ctrl2=(\\d+)");
    
    // Try simple format: "H(q0)"
    auto qMatch = qRe.match(desc);
    if (qMatch.hasMatch()) {
        target = qMatch.captured(1).toInt();
        return;
    }
    
    // Try complex format: "CNOT(ctrl=0, target=1)"
    auto targetMatch = targetRe.match(desc);
    if (targetMatch.hasMatch()) {
        target = targetMatch.captured(1).toInt();
    }
    
    auto ctrlMatch = ctrlRe.match(desc);
    if (ctrlMatch.hasMatch()) {
        control1 = ctrlMatch.captured(1).toInt();
    }
    
    auto ctrl1Match = ctrl1Re.match(desc);
    if (ctrl1Match.hasMatch()) {
        control1 = ctrl1Match.captured(1).toInt();
    }
    
    auto ctrl2Match = ctrl2Re.match(desc);
    if (ctrl2Match.hasMatch()) {
        control2 = ctrl2Match.captured(1).toInt();
    }
}
