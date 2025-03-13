#include <QApplication>
#include <QMainWindow>
#include "ui/HeatMapWidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QMainWindow mainWindow;
    HeatMapWidget heatMap;
    mainWindow.setCentralWidget(&heatMap);
    mainWindow.resize(800, 800);
    mainWindow.show();

    return QApplication::exec();
}
