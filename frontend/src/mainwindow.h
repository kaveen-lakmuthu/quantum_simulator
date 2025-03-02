#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include "circuit_view.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void setQubitCount(int count);
    void addHadamardGate();
    void addPauliXGate();
    void addPauliYGate();
    void addPauliZGate();
    void addCNOTGate();
    void addSWAPGate();
    void executeCircuit();

private:
    CircuitView *circuitView;
    QComboBox *qubitSelector;
    QComboBox *controlQubitSelector;
    
    QPushButton *executeButton;
    QPushButton *clearButton;

    QPushButton *hadamardButton;
    QPushButton *pauliXButton;
    QPushButton *pauliYButton;
    QPushButton *pauliZButton;
    QPushButton *cnotButton;
    QPushButton *swapButton;
};

#endif // MAINWINDOW_H
