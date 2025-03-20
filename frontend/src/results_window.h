#ifndef RESULTS_WINDOW_H
#define RESULTS_WINDOW_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class ResultsWindow : public QWidget {
    Q_OBJECT

public:
    explicit ResultsWindow(QWidget *parent = nullptr);
    void setResults(const QString &resultsText);  // Set results in window

private:
    QLabel *resultsLabel;  // Label to display results
};

#endif // RESULTS_WINDOW_H
