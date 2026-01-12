#pragma once

#include <QQuickPaintedItem>
#include <QPainter>
#include <QStringList>

/**
 * @class CircuitPainter
 * @brief Custom QML component for rendering quantum circuit diagrams
 *
 * Provides visual representation of quantum circuits with qubit lines
 * and gate symbols. Integrates with QML scene graph for efficient rendering.
 *
 * @note Inherits QQuickPaintedItem for custom painting in QML
 */
class CircuitPainter : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(int qubitCount READ getQubitCount WRITE setQubitCount NOTIFY qubitCountChanged)
    Q_PROPERTY(QStringList gates READ getGates WRITE setGates NOTIFY gatesChanged)

public:
    explicit CircuitPainter(QQuickItem *parent = nullptr);

    /// Returns current number of qubits displayed
    int getQubitCount() const { return numQubits; }
    
    /// Sets number of qubit lines to draw
    void setQubitCount(int count);
    
    /// Returns list of gates in circuit
    QStringList getGates() const { return gateList; }
    
    /// Updates gate list and triggers repaint
    void setGates(const QStringList &gates);

    /// QQuickPaintedItem override: renders circuit to painter
    void paint(QPainter *painter) override;

signals:
    void qubitCountChanged(int count);
    void gatesChanged(const QStringList &gates);

private:
    int numQubits;              ///< Number of qubit lines
    QStringList gateList;       ///< Ordered list of gate descriptions
    
    /// Constant: horizontal spacing between gates (pixels)
    static constexpr int GATE_SPACING = 80;
    
    /// Constant: vertical spacing between qubit lines (pixels)
    static constexpr int QUBIT_SPACING = 60;
    
    /// Constant: left margin before first gate (pixels)
    static constexpr int LEFT_MARGIN = 40;
    
    /// Constant: top margin before first qubit (pixels)
    static constexpr int TOP_MARGIN = 30;

    /// Draws horizontal qubit lines
    void drawQubitLines(QPainter *painter);
    
    /// Draws individual gate at specified position
    void drawGate(QPainter *painter, const QString &gateDesc, int gateIndex);
    
    /// Parses gate description string to extract type and qubit indices
    void parseGateDescription(const QString &desc, QString &gateName, 
                            int &target, int &control1, int &control2);
};
