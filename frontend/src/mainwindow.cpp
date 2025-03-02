#include "mainwindow.h"
#include <QHBoxLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    circuitView = new CircuitView(this);
    layout->addWidget(circuitView);

    QHBoxLayout *topLayout = new QHBoxLayout();
    QLabel *label = new QLabel("Select Qubit Count:");
    qubitSelector = new QComboBox();
    for (int i = 1; i <= 5; ++i) {
        qubitSelector->addItem(QString::number(i));
    }
    connect(qubitSelector, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::setQubitCount);

    topLayout->addWidget(label);
    topLayout->addWidget(qubitSelector);
    layout->addLayout(topLayout);

    QHBoxLayout *qubitSelectionLayout = new QHBoxLayout();
    QLabel *targetLabel = new QLabel("Target Qubit:");
    QLabel *controlLabel = new QLabel("Control Qubit:");
    
    qubitSelector = new QComboBox();
    controlQubitSelector = new QComboBox();

    for (int i = 0; i < 5; ++i) {
        qubitSelector->addItem(QString::number(i));
        controlQubitSelector->addItem(QString::number(i));
    }

    qubitSelectionLayout->addWidget(targetLabel);
    qubitSelectionLayout->addWidget(qubitSelector);
    qubitSelectionLayout->addWidget(controlLabel);
    qubitSelectionLayout->addWidget(controlQubitSelector);
    
    layout->addLayout(qubitSelectionLayout);

    QHBoxLayout *gateLayout = new QHBoxLayout();

    hadamardButton = new QPushButton("Hadamard");
    connect(hadamardButton, &QPushButton::clicked, this, &MainWindow::addHadamardGate);
    gateLayout->addWidget(hadamardButton);

    pauliXButton = new QPushButton("Pauli-X");
    connect(pauliXButton, &QPushButton::clicked, this, &MainWindow::addPauliXGate);
    gateLayout->addWidget(pauliXButton);

    pauliYButton = new QPushButton("Pauli-Y");
    connect(pauliYButton, &QPushButton::clicked, this, &MainWindow::addPauliYGate);
    gateLayout->addWidget(pauliYButton);

    pauliZButton = new QPushButton("Pauli-Z");
    connect(pauliZButton, &QPushButton::clicked, this, &MainWindow::addPauliZGate);
    gateLayout->addWidget(pauliZButton);

    cnotButton = new QPushButton("CNOT");
    connect(cnotButton, &QPushButton::clicked, this, &MainWindow::addCNOTGate);
    gateLayout->addWidget(cnotButton);

    swapButton = new QPushButton("SWAP");
    connect(swapButton, &QPushButton::clicked, this, &MainWindow::addSWAPGate);
    gateLayout->addWidget(swapButton);

    layout->addLayout(gateLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    executeButton = new QPushButton("Execute Circuit");
    connect(executeButton, &QPushButton::clicked, this, &MainWindow::executeCircuit);
    buttonLayout->addWidget(executeButton);

    clearButton = new QPushButton("Clear");
    connect(clearButton, &QPushButton::clicked, this, [this]() { circuitView->clearCircuit(); });
    buttonLayout->addWidget(clearButton);

    layout->addLayout(buttonLayout);
}

void MainWindow::setQubitCount(int count) {
    circuitView->setQubitCount(count);
}

void MainWindow::addHadamardGate() {
    circuitView->addGate("Hadamard", qubitSelector->currentIndex());
}

void MainWindow::addPauliXGate() {
    circuitView->addGate("Pauli-X", qubitSelector->currentIndex());
}

void MainWindow::addPauliYGate() {
    circuitView->addGate("Pauli-Y", qubitSelector->currentIndex());
}

void MainWindow::addPauliZGate() {
    circuitView->addGate("Pauli-Z", qubitSelector->currentIndex());
}

void MainWindow::addCNOTGate() {
    circuitView->addGate("CNOT", controlQubitSelector->currentIndex(), qubitSelector->currentIndex());
}

void MainWindow::addSWAPGate() {
    circuitView->addGate("SWAP", controlQubitSelector->currentIndex(), qubitSelector->currentIndex());
}

void MainWindow::executeCircuit() {
    circuitView->executeCircuit();
}
