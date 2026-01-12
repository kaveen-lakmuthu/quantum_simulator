#pragma once

#include <QMainWindow>
#include "circuit_view.h"
#include "results_window.h"

class QComboBox;
class QPushButton;
class QLineEdit;
class QWidget;

/**
 * @class MainWindow
 * @brief Main application window for the Quantum Circuit Simulator GUI
 *
 * Provides the primary user interface for designing and executing quantum circuits.
 * Manages qubit selection, gate operations, circuit visualization, and results display.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief Constructs the main application window
     * @param parent Parent widget (default: nullptr)
     */
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    /// Handles qubit count selection from dropdown
    void setQubitCount();
    /// Adds Hadamard gate to circuit
    void addHadamardGate();
    /// Adds Pauli-X gate to circuit
    void addPauliXGate();
    /// Adds Pauli-Y gate to circuit
    void addPauliYGate();
    /// Adds Pauli-Z gate to circuit
    void addPauliZGate();
    /// Adds CNOT gate to circuit
    void addCNOTGate();
    /// Adds SWAP gate to circuit
    void addSWAPGate();
    /// Executes the circuit and displays results
    void executeCircuitAndShowResults();
    /// Updates qubit selector dropdowns when count changes
    void updateQubitSelectors();

private:
    /// Initializes main UI layout and connections
    void setupUI();
    /// Creates and connects gate operation buttons
    void setupGateButtons();
    /// Creates qubit selector dropdowns
    void setupQubitSelectors();

    CircuitView *circuitView;          ///< Circuit visualization widget
    ResultsWindow *resultsWindow;      ///< Quantum state results display
    QWidget *centralWidget;            ///< Central container widget
    
    QComboBox *qubitSelector;          ///< Qubit count selector
    QComboBox *controlQubitSelector;   ///< Control qubit selector for multi-qubit gates
    QComboBox *targetQubitSelector;    ///< Target qubit selector
    QLineEdit *initialStateInput;      ///< Initial quantum state input
};