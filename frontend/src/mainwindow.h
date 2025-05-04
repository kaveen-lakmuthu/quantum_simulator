#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "circuit_view.h"
#include "results_window.h"

class QComboBox;
class QPushButton;
class QLineEdit;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void setQubitCount();
    void addHadamardGate();
    void addPauliXGate();
    void addPauliYGate();
    void addPauliZGate();
    void addCNOTGate();
    void addSWAPGate();
    void executeCircuitAndShowResults();
    void updateQubitSelectors();

private:
    void setupUI();
    void setupGateButtons();
    void setupQubitSelectors();

    CircuitView *circuitView;
    ResultsWindow *resultsWindow;
    
    QComboBox *qubitSelector;
    QComboBox *controlQubitSelector;
    QComboBox *targetQubitSelector;
    QLineEdit *initialStateInput;
};

#endif // MAINWINDOW_H