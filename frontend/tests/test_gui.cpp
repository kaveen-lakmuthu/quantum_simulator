#include <QtTest/QtTest>
#include <QApplication>
#include "src/circuit_view.h"
#include "src/mainwindow.h"
#include "src/results_window.h"

class CircuitViewTest : public QObject {
    Q_OBJECT
private slots:
    void initTestCase() {
        // Initial state sanity
        CircuitView view;
        QCOMPARE(view.getQubitCount(), 2);
    }

    void testSetQubitCountValid() {
        CircuitView view;
        view.setQubitCount(3);
        QCOMPARE(view.getQubitCount(), 3);
    }

    void testSetQubitCountInvalid() {
        CircuitView view;
        bool threw = false;
        try { view.setQubitCount(0); } catch (const std::invalid_argument&) { threw = true; }
        QVERIFY(threw);
        threw = false;
        try { view.setQubitCount(6); } catch (const std::invalid_argument&) { threw = true; }
        QVERIFY(threw);
    }

    void testAddGateAndExecuteBellState() {
        CircuitView view;
        view.setQubitCount(2);
        view.clearCircuit();
        view.addGate("Hadamard", 0);
        // CNOT with control=0, target=1 → pass target first then control
        view.addGate("CNOT", 1, 0);
        QString results = view.executeCircuit("");
        QVERIFY(results.contains("|00"));
        QVERIFY(results.contains("|11"));
    }

    void testClearCircuit() {
        CircuitView view;
        view.setQubitCount(2);
        view.addGate("Hadamard", 0);
        view.clearCircuit();
        QString results = view.executeCircuit("");
        QVERIFY(results.contains("|00")); // default state |00⟩
    }
};

class ResultsWindowTest : public QObject {
    Q_OBJECT
private slots:
    void testResultsWindowSetAndClear() {
        ResultsWindow win;
        win.setResults("|00⟩: 1.0000 + 0.0000i\n");
        // No direct getter; ensure it doesn't crash
        win.clearResults();
        QVERIFY(true);
    }
};

// Use QTEST_MAIN for each test class separately isn't possible; create a runner.
int main(int argc, char** argv) {
    QApplication app(argc, argv);
    int status = 0;
    {
        CircuitViewTest tc; status |= QTest::qExec(&tc, argc, argv);
    }
    {
        ResultsWindowTest tc; status |= QTest::qExec(&tc, argc, argv);
    }
    return status;
}

// Required for Qt's AutoMOC when Q_OBJECT is used in a .cpp file
#include "test_gui.moc"
