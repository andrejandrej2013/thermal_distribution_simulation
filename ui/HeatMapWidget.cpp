#include "HeatMapWidget.h"
#include <iostream>
#include <chrono>

HeatMapWidget::HeatMapWidget(QWidget *parent)
    : QWidget(parent){

    simulation = Simulation();

    int width = 1000, height = 1000;
    if (width > 30 || height > 30) {
        writeTemperature = false;
    }

    simulation.generateRandomTemperatureGrid(width, height);

    connect(&simulationTimer, &QTimer::timeout, this, &HeatMapWidget::updateSimulation);
    simulationTimer.start(0);

    connect(&uiTimer, &QTimer::timeout, this, &HeatMapWidget::updateUI);
    uiTimer.start(0);
}

void HeatMapWidget::updateSimulation() {
    auto start = std::chrono::high_resolution_clock::now();

    simulation.update();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double>(end - start);  // Convert to seconds

    std::cout << "Simulation update took: " << duration.count() << " seconds" << std::endl;
}

void HeatMapWidget::updateUI() {
    auto start = std::chrono::high_resolution_clock::now();

    heatmapCache = QPixmap();
    update(); // 🔥 Now UI updates separately!

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double>(end - start);

    std::cout << "UI update took: " << duration.count() << " seconds" << std::endl;
}

void HeatMapWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    // **If no need to redraw, use cached image**
    if (!heatmapCache.isNull()) {
        painter.drawPixmap(0, 0, heatmapCache);
        return;
    }

    auto temperatureGrid = simulation.getTemperatureGrid();
    if (temperatureGrid.empty()) return;

    int rows = temperatureGrid.size();
    int cols = temperatureGrid[0].size();
    double cellWidth = static_cast<double>(width()) / cols;
    double cellHeight = static_cast<double>(height()) / rows;

    auto start = std::chrono::high_resolution_clock::now();

    heatmapCache = QPixmap(size());
    heatmapCache.fill(Qt::black);
    QPainter pixmapPainter(&heatmapCache);

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            QColor color = getColorForTemperature(temperatureGrid[y][x].temperature);
            QRectF rect(x * cellWidth, y * cellHeight, cellWidth, cellHeight);
            pixmapPainter.fillRect(rect, color);
        }
    }

    painter.drawPixmap(0, 0, heatmapCache);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double>(end - start);
    std::cout << "UI render took: " << duration.count() << " seconds" << std::endl;
}

QColor HeatMapWidget::getColorForTemperature(double temp) const {
    QColor extremeCold = QColor(0, 0, 139);      // Dark blue (<=-20°C)
    QColor cold = QColor(0, 255, 255);           // Cyan (~10°C)
    QColor cool = QColor(173, 255, 47);          // Yellow-green (~15°C)
    QColor normal = QColor(0, 255, 0);           // **Bright green (22°C)**
    QColor warm = QColor(255, 165, 0);           // Orange (~30°C)
    QColor hot = QColor(255, 0, 0);              // Red (>=~40°C)

    if (temp <= -20) return extremeCold;   // **Extreme cold → Dark Blue**
    if (temp >= 40) return hot;            // **Extreme heat → Red**

    if (temp < 10) {
        // **Transition from Dark Blue (-20°C) → Cyan (10°C)**
        double ratio = (temp + 20) / 30.0;
        return QColor(
            (1 - ratio) * extremeCold.red() + ratio * cold.red(),
            (1 - ratio) * extremeCold.green() + ratio * cold.green(),
            (1 - ratio) * extremeCold.blue() + ratio * cold.blue()
        );
    } else if (temp < 15) {
        // **Transition from Cyan (10°C) → Yellow-Green (15°C)**
        double ratio = (temp - 10) / 5.0;
        return QColor(
            (1 - ratio) * cold.red() + ratio * cool.red(),
            (1 - ratio) * cold.green() + ratio * cool.green(),
            (1 - ratio) * cold.blue() + ratio * cool.blue()
        );
    } else if (temp < 22) {
        // **Transition from Yellow-Green (15°C) → Bright Green (22°C)**
        double ratio = (temp - 15) / 7.0;
        return QColor(
            (1 - ratio) * cool.red() + ratio * normal.red(),
            (1 - ratio) * cool.green() + ratio * normal.green(),
            (1 - ratio) * cool.blue() + ratio * normal.blue()
        );
    } else if (temp < 30) {
        // **Transition from Bright Green (22°C) → Orange (30°C)**
        double ratio = (temp - 22) / 8.0;
        return QColor(
            (1 - ratio) * normal.red() + ratio * warm.red(),
            (1 - ratio) * normal.green() + ratio * warm.green(),
            (1 - ratio) * normal.blue() + ratio * warm.blue()
        );
    } else {
        // **Transition from Orange (30°C) → Red (40°C)**
        double ratio = (temp - 30) / 10.0;
        return QColor(
            (1 - ratio) * warm.red() + ratio * hot.red(),
            (1 - ratio) * warm.green() + ratio * hot.green(),
            (1 - ratio) * warm.blue() + ratio * hot.blue()
        );
    }
}

