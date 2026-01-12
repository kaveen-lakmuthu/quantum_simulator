#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QStandardPaths>
#include "backend_bridge.h"
#include "circuit_painter.h"

/**
 * @brief Main entry point for QML-based quantum simulator GUI
 * 
 * Initializes Qt application, registers custom QML types, exposes
 * backend bridge to QML context, and loads main QML interface.
 * 
 * @param argc Command line argument count
 * @param argv Command line argument values
 * @return Application exit code (0 = success)
 */
int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    // Register custom QML types for circuit visualization
    qmlRegisterType<CircuitPainter>("QuantumSim", 1, 0, "CircuitPainter");

    QQmlApplicationEngine engine;

    // Create backend bridge and expose to QML
    BackendBridge backend;
    engine.rootContext()->setContextProperty("backend", &backend);

    // Load QML from filesystem (during development)
    // In production, use embedded resources: QUrl(QStringLiteral("qrc:/MainWindow.qml"))
    const QUrl url = QUrl::fromLocalFile(
        QCoreApplication::applicationDirPath() + "/../src/MainWindow.qml"
    );
    
    engine.load(url);

    if (engine.rootObjects().isEmpty()) {
        qWarning() << "Failed to load QML. Tried:" << url.toLocalFile();
        return -1;
    }

    return app.exec();
}
