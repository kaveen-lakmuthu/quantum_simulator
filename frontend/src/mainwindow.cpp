#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>

// MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), resultsWindow(nullptr) {
//     setWindowTitle("Quantum Circuit Simulator");
//     resize(800, 600);
    
//     circuitView = new CircuitView(this);
//     setupUI();
// }

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    qDebug() << "Initializing MainWindow...";
    
    try {
        // Initialize with null checks
        centralWidget = new QWidget(this);
        if (!centralWidget) throw std::runtime_error("Failed to create central widget");
        
        circuitView = new CircuitView(centralWidget);
        if (!circuitView) throw std::runtime_error("Failed to create circuit view");
        
        setupUI();
        qDebug() << "MainWindow initialized successfully";
    } catch (const std::exception& e) {
        qCritical() << "Initialization error:" << e.what();
        throw;
    }
}

void MainWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // Initial State Input
    QHBoxLayout *stateLayout = new QHBoxLayout();
    stateLayout->addWidget(new QLabel("Initial State:"));
    initialStateInput = new QLineEdit();
    initialStateInput->setPlaceholderText("e.g., 0101 for 4 qubits");
    stateLayout->addWidget(initialStateInput);
    mainLayout->addLayout(stateLayout);

    // Qubit Selection
    QHBoxLayout *qubitLayout = new QHBoxLayout();
    qubitLayout->addWidget(new QLabel("Qubit Count:"));
    qubitSelector = new QComboBox();
    for (int i = 1; i <= 5; ++i) {
        qubitSelector->addItem(QString::number(i));
    }
    connect(qubitSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::setQubitCount);
    qubitLayout->addWidget(qubitSelector);
    mainLayout->addLayout(qubitLayout);

    // Target/Control Qubit Selection
    QHBoxLayout *selectionLayout = new QHBoxLayout();
    selectionLayout->addWidget(new QLabel("Target:"));
    targetQubitSelector = new QComboBox();
    selectionLayout->addWidget(targetQubitSelector);
    
    selectionLayout->addWidget(new QLabel("Control:"));
    controlQubitSelector = new QComboBox();
    selectionLayout->addWidget(controlQubitSelector);
    mainLayout->addLayout(selectionLayout);

    // Circuit View
    mainLayout->addWidget(circuitView);

    // Gate Buttons
    setupGateButtons();
    
    // Execute/Clear Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *executeButton = new QPushButton("Execute");
    connect(executeButton, &QPushButton::clicked,
            this, &MainWindow::executeCircuitAndShowResults);
    buttonLayout->addWidget(executeButton);
    
    QPushButton *clearButton = new QPushButton("Clear");
    connect(clearButton, &QPushButton::clicked,
            [this]() { circuitView->clearCircuit(); });
    buttonLayout->addWidget(clearButton);
    mainLayout->addLayout(buttonLayout);

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
    
    // Initialize qubit selectors
    updateQubitSelectors();
}

void MainWindow::setupGateButtons() {
    QHBoxLayout *gateLayout = new QHBoxLayout();
    
    auto addGateButton = [&](const QString &name, void(MainWindow::*slot)()) {
        QPushButton *btn = new QPushButton(name);
        connect(btn, &QPushButton::clicked, this, slot);
        gateLayout->addWidget(btn);
    };
    
    addGateButton("Hadamard", &MainWindow::addHadamardGate);
    addGateButton("Pauli-X", &MainWindow::addPauliXGate);
    addGateButton("Pauli-Y", &MainWindow::addPauliYGate);
    addGateButton("Pauli-Z", &MainWindow::addPauliZGate);
    addGateButton("CNOT", &MainWindow::addCNOTGate);
    addGateButton("SWAP", &MainWindow::addSWAPGate);
    
    qobject_cast<QVBoxLayout*>(centralWidget()->layout())->addLayout(gateLayout);
}

void MainWindow::updateQubitSelectors() {
    int count = circuitView->getQubitCount();
    targetQubitSelector->clear();
    controlQubitSelector->clear();
    
    for (int i = 0; i < count; ++i) {
        targetQubitSelector->addItem(QString::number(i));
        controlQubitSelector->addItem(QString::number(i));
    }
    controlQubitSelector->setCurrentIndex(count > 1 ? 1 : 0);
}

void MainWindow::setQubitCount() {
    int count = qubitSelector->currentText().toInt();
    circuitView->setQubitCount(count);
    updateQubitSelectors();
}

void MainWindow::addHadamardGate() {
    circuitView->addGate("Hadamard", targetQubitSelector->currentIndex());
}

void MainWindow::addPauliXGate() {
    circuitView->addGate("Pauli-X", targetQubitSelector->currentIndex());
}

void MainWindow::addPauliYGate() {
    circuitView->addGate("Pauli-Y", targetQubitSelector->currentIndex());
}

void MainWindow::addPauliZGate() {
    circuitView->addGate("Pauli-Z", targetQubitSelector->currentIndex());
}

void MainWindow::addCNOTGate() {
    circuitView->addGate("CNOT", controlQubitSelector->currentIndex(), 
                        targetQubitSelector->currentIndex());
}

void MainWindow::addSWAPGate() {
    circuitView->addGate("SWAP", controlQubitSelector->currentIndex(),
                       targetQubitSelector->currentIndex());
}

void MainWindow::executeCircuitAndShowResults() {
    try {
        if (!resultsWindow) {
            resultsWindow = new ResultsWindow();
            resultsWindow->setAttribute(Qt::WA_DeleteOnClose);
        }
        
        QString results = circuitView->executeCircuit(initialStateInput->text());
        resultsWindow->setResults(results);
        resultsWindow->show();
        resultsWindow->raise();
        resultsWindow->activateWindow();
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Error", 
                             QString("Failed to execute circuit:\n%1").arg(e.what()));
    }
}