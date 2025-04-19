#include "HeatMapWidget.h"
#include <QPainter>
#include <iostream>
#include <chrono>

HeatMapWidget::HeatMapWidget(QWidget *parent)
    : QWidget(parent),
      simulation(800, 800),
      gridWidth(800), gridHeight(800)
{
    connect(&simulationTimer, &QTimer::timeout, this, &HeatMapWidget::updateSimulation);
    simulationTimer.start(16); // ~60 FPS

    connect(&uiTimer, &QTimer::timeout, this, &HeatMapWidget::updateUI);
    uiTimer.start(16); // ~60 FPS
}

void HeatMapWidget::updateSimulation() {
    auto start = std::chrono::high_resolution_clock::now();
    simulation.step();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Simulation step took: " << duration.count() << " seconds\n";
}

void HeatMapWidget::updateUI() {
    heatmapCache = QPixmap(); // Clear cache
    update(); // Trigger paintEvent
}

void HeatMapWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    if (!heatmapCache.isNull()) {
        painter.drawPixmap(0, 0, heatmapCache);
        return;
    }

    auto data = simulation.getTemperatureData();
    if (data.empty()) return;

    int rows = gridHeight;
    int cols = gridWidth;
    double cellWidth = static_cast<double>(width()) / cols;
    double cellHeight = static_cast<double>(height()) / rows;

    heatmapCache = QPixmap(size());
    heatmapCache.fill(Qt::black);
    QPainter pixmapPainter(&heatmapCache);

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            float temp = data[y * gridWidth + x];
            QColor color = getColorForTemperature(temp);
            QRectF rect(x * cellWidth, y * cellHeight, cellWidth, cellHeight);
            pixmapPainter.fillRect(rect, color);
        }
    }

    painter.drawPixmap(0, 0, heatmapCache);
}

QColor HeatMapWidget::getColorForTemperature(float temp) const {
    QColor extremeCold(0, 0, 139);
    QColor cold(0, 255, 255);
    QColor cool(173, 255, 47);
    QColor normal(0, 255, 0);
    QColor warm(255, 165, 0);
    QColor hot(255, 0, 0);

    if (temp <= -20) return extremeCold;
    if (temp >= 40) return hot;

    if (temp < 10) {
        float r = (temp + 20) / 30.0f;
        return QColor::fromRgbF((1 - r) * extremeCold.redF() + r * cold.redF(),
                                (1 - r) * extremeCold.greenF() + r * cold.greenF(),
                                (1 - r) * extremeCold.blueF() + r * cold.blueF());
    } else if (temp < 15) {
        float r = (temp - 10) / 5.0f;
        return QColor::fromRgbF((1 - r) * cold.redF() + r * cool.redF(),
                                (1 - r) * cold.greenF() + r * cool.greenF(),
                                (1 - r) * cold.blueF() + r * cool.blueF());
    } else if (temp < 22) {
        float r = (temp - 15) / 7.0f;
        return QColor::fromRgbF((1 - r) * cool.redF() + r * normal.redF(),
                                (1 - r) * cool.greenF() + r * normal.greenF(),
                                (1 - r) * cool.blueF() + r * normal.blueF());
    } else if (temp < 30) {
        float r = (temp - 22) / 8.0f;
        return QColor::fromRgbF((1 - r) * normal.redF() + r * warm.redF(),
                                (1 - r) * normal.greenF() + r * warm.greenF(),
                                (1 - r) * normal.blueF() + r * warm.blueF());
    } else {
        float r = (temp - 30) / 10.0f;
        return QColor::fromRgbF((1 - r) * warm.redF() + r * hot.redF(),
                                (1 - r) * warm.greenF() + r * hot.greenF(),
                                (1 - r) * warm.blueF() + r * hot.blueF());
    }
}
