#include "results_window.h"
#include <QDebug>
#include <stdexcept>

/// Initializes the results window with labels and layout
ResultsWindow::ResultsWindow(QWidget *parent) : QWidget(parent) {
    qDebug() << "Creating ResultsWindow...";
    
    try {
        QVBoxLayout *layout = new QVBoxLayout(this);
        resultsLabel = new QLabel("Results will appear here", this);
        resultsLabel->setWordWrap(true);
        
        // Verify pointer creation
        if (!layout || !resultsLabel) {
            throw std::runtime_error("Failed to create UI elements");
        }
        
        layout->addWidget(resultsLabel);
        setLayout(layout);
        setWindowTitle("Quantum Circuit Results");
        resize(600, 400);
        
        qDebug() << "ResultsWindow created successfully";
    } catch (const std::exception& e) {
        qCritical() << "ResultsWindow initialization error:" << e.what();
        throw;
    }
}

/// Displays quantum state amplitude results
void ResultsWindow::setResults(const QString &resultsText) {
    if (!resultsLabel) {
        qDebug() << "Error: resultsLabel is null!";
        return;
    }
    // Format output as monospaced text for better readability
    resultsLabel->setText("<pre style='font-family: monospace;'>" + resultsText + "</pre>");
}

/// Clears all displayed results
void ResultsWindow::clearResults() {
    if (resultsLabel) {
        resultsLabel->clear();
    }
}