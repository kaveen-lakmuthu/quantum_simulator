#include "mainwindow.h"
#include <QHBoxLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget); // Initialize mainLayout
    circuitView = new CircuitView(this);
    mainLayout->addWidget(circuitView);

    resultsWindow = new ResultsWindow(this);

    // Initialize Initial State Input
    QHBoxLayout *initialStateLayout = new QHBoxLayout();
    QLabel *stateLabel = new QLabel("Initial State:");
    QLineEdit *initialStateInput = new QLineEdit();
    initialStateInput->setPlaceholderText("e.g., 00000 for 5 qubits");

    initialStateLayout->addWidget(stateLabel);
    initialStateLayout->addWidget(initialStateInput);
    mainLayout->addLayout(initialStateLayout);

    QHBoxLayout *topLayout = new QHBoxLayout();
    QLabel *label = new QLabel("Select Qubit Count:");
    qubitSelector = new QComboBox();
    for (int i = 0; i <= 5; ++i) {
        qubitSelector->addItem(QString::number(i));
    }
    connect(qubitSelector, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::setQubitCount);

    topLayout->addWidget(label);
    topLayout->addWidget(qubitSelector);
    mainLayout->addLayout(topLayout);

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
    
    mainLayout->addLayout(qubitSelectionLayout);

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

    mainLayout->addLayout(gateLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    executeButton = new QPushButton("Execute Circuit");
    connect(executeButton, &QPushButton::clicked, this, &MainWindow::executeCircuitAndShowResults);  
    buttonLayout->addWidget(executeButton);

    clearButton = new QPushButton("Clear");
    connect(clearButton, &QPushButton::clicked, this, [this]() { circuitView->clearCircuit(); });
    buttonLayout->addWidget(clearButton);

    mainLayout->addLayout(buttonLayout);
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

// void MainWindow::executeCircuitAndShowResults() {
//     qDebug() << "Executing circuit...";  // Debugging output

//     circuitView->executeCircuit();  // Runs the quantum simulation
//     QString results = circuitView->getCircuitState();  // Get results

//     if (!resultsWindow) {  // Make sure the results window is initialized
//         resultsWindow = new ResultsWindow(this);
//     }

//     resultsWindow->setResults(results);  // Pass results to the window
//     resultsWindow->show();               // Open the results window
//     resultsWindow->raise();              // Bring it to the front
//     resultsWindow->activateWindow();     // Ensure focus is on the result window

//     qDebug() << "Results window opened";  // Debugging output
// }

void MainWindow::executeCircuitAndShowResults() {
    circuitView->executeCircuit(initialStateInput->text());  
    resultsWindow->setResults(circuitView->getCircuitState());  
    resultsWindow->show();  
}