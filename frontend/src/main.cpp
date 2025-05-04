#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[]) {
    qDebug() << "Starting application...";
    QApplication app(argc, argv);
    
    try {
        qDebug() << "Creating main window...";
        MainWindow mainWindow;
        qDebug() << "Showing main window...";
        mainWindow.show();
        qDebug() << "Entering event loop...";
        return app.exec();
    } catch (const std::exception& e) {
        qCritical() << "Fatal error:" << e.what();
        return 1;
    }
}