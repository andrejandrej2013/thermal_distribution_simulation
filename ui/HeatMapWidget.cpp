#include "HeatMapWidget.h"
#include <QPainter>
#include <iostream>
#include <chrono>

HeatMapWidget::HeatMapWidget(QWidget *parent)
    : QWidget(parent),
      simulation(1000, 1000),
      gridWidth(1000), gridHeight(1000)
{
    fpsTimer.start();
    connect(&simulationTimer, &QTimer::timeout, this, &HeatMapWidget::updateSimulation);
    simulationTimer.start(16); // ~60 FPS

    connect(&uiTimer, &QTimer::timeout, this, &HeatMapWidget::updateUI);
    uiTimer.start(16); // ~60 FPS
}

void HeatMapWidget::updateSimulation() {
    auto start = std::chrono::high_resolution_clock::now();
    simulation.step();
    auto end = std::chrono::high_resolution_clock::now();
    lastUpdateDuration = std::chrono::duration<double, std::milli>(end - start).count();
}

void HeatMapWidget::updateUI() {
    heatmapCache = QPixmap();
    update();
}

void HeatMapWidget::paintEvent(QPaintEvent *event) {
    auto fullStart = std::chrono::high_resolution_clock::now();
    QPainter painter(this);

    if (!heatmapCache.isNull()) {
        painter.drawPixmap(0, 0, heatmapCache);
        return;
    }

    auto dataFetchStart = std::chrono::high_resolution_clock::now();
    auto data = simulation.getTemperatureData();
    auto dataFetchEnd = std::chrono::high_resolution_clock::now();

    if (data.empty()) return;

    int rows = gridHeight;
    int cols = gridWidth;

    auto renderStart = std::chrono::high_resolution_clock::now();
    QImage image(cols, rows, QImage::Format_RGB32);
    uchar* pixels = image.bits();

    auto pixelLoopStart = std::chrono::high_resolution_clock::now();

    simulation.generateColorImage(pixels);

    auto pixelLoopEnd = std::chrono::high_resolution_clock::now();
    std::cout << "Pixel color loop: "
              << std::chrono::duration<double, std::milli>(pixelLoopEnd - pixelLoopStart).count()
              << " ms\n";

    heatmapCache = QPixmap::fromImage(image.scaled(size()));
    auto renderEnd = std::chrono::high_resolution_clock::now();

    painter.drawPixmap(0, 0, heatmapCache);

    // === FPS logic ===
    frameCount++;
    if (fpsTimer.elapsed() >= 1000) {
        currentFps = frameCount * 1000.0 / fpsTimer.elapsed();
        frameCount = 0;
        fpsTimer.restart();
    }

    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    QString info = QString("FPS: %1 Backend: %2 ms")
                       .arg(currentFps, 0, 'f', 1)
                       .arg(lastUpdateDuration, 0, 'f', 2);
    painter.drawText(width() - 150, 30, info);

    auto fullEnd = std::chrono::high_resolution_clock::now();
    std::cout << "Data fetch: " << std::chrono::duration<double, std::milli>(dataFetchEnd - dataFetchStart).count() << " ms\n";
    std::cout << "Render raw buffer: " << std::chrono::duration<double, std::milli>(renderEnd - renderStart).count() << " ms\n";
    std::cout << "Total paintEvent: " << std::chrono::duration<double, std::milli>(fullEnd - fullStart).count() << " ms\n";
}
