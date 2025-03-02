#include "results_window.h"

ResultsWindow::ResultsWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Quantum Circuit Results");
    setFixedSize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(this);
    resultsLabel = new QLabel("Results will be displayed here.", this);
    resultsLabel->setWordWrap(true);
    layout->addWidget(resultsLabel);
}

void ResultsWindow::setResults(const QString &resultsText) {
    resultsLabel->setText(resultsText);
}
