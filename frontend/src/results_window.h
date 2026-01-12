#pragma once

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

/**
 * @class ResultsWindow
 * @brief Displays quantum circuit execution results
 *
 * Shows quantum state amplitudes and measurement probabilities
 * in a user-friendly format.
 */
class ResultsWindow : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructs the results display window
     * @param parent Parent widget (default: nullptr)
     */
    explicit ResultsWindow(QWidget *parent = nullptr);
    
    /**
     * @brief Displays quantum state results
     * @param resultsText Formatted quantum state information
     */
    void setResults(const QString &resultsText);
    
    /// Clears all displayed results
    void clearResults();

private:
    QLabel *resultsLabel;      ///< Label widget for displaying results
};