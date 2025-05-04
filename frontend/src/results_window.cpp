// #include "results_window.h"
// #include <QScrollArea>
// #include <QDebug>

// ResultsWindow::ResultsWindow(QWidget *parent) : QWidget(parent) {
//     QVBoxLayout *layout = new QVBoxLayout(this);
    
//     QScrollArea *scrollArea = new QScrollArea(this);
//     resultsLabel = new QLabel(scrollArea);
//     resultsLabel->setWordWrap(true);
//     resultsLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
//     resultsLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    
//     scrollArea->setWidget(resultsLabel);
//     scrollArea->setWidgetResizable(true);
//     layout->addWidget(scrollArea);
    
//     setLayout(layout);
//     resize(500, 400);
//     setWindowTitle("Quantum Circuit Results");
// }

// void ResultsWindow::setResults(const QString &resultsText) {
//     if (!resultsLabel) {
//         qDebug() << "Error: resultsLabel is null!";
//         return;
//     }
//     resultsLabel->setText("<pre>" + resultsText + "</pre>");
// }

// void ResultsWindow::clearResults() {
//     if (resultsLabel) {
//         resultsLabel->clear();
//     }
// }

#include "results_window.h"
#include <QDebug>

ResultsWindow::ResultsWindow(QWidget *parent) : QWidget(parent) {
    qDebug() << "Creating ResultsWindow...";
    
    QVBoxLayout *layout = new QVBoxLayout(this);
    resultsLabel = new QLabel("Results will appear here", this);
    resultsLabel->setWordWrap(true);
    
    // Verify pointer creation
    if (!layout || !resultsLabel) {
        qCritical() << "Failed to create UI elements";
        return;
    }
    
    layout->addWidget(resultsLabel);
    setLayout(layout);
    qDebug() << "ResultsWindow created successfully";
}