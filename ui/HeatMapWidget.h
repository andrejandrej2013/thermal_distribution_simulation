#ifndef HEATMAPWIDGET_H
#define HEATMAPWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <iostream>
#include <chrono>
#include "../dto/TemperatureCell.h"
#include "../logic/Simulation.h"

class HeatMapWidget : public QWidget {
    Q_OBJECT

public:
    explicit HeatMapWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Simulation simulation;
    QTimer simulationTimer;
    QTimer uiTimer;
    QPixmap heatmapCache;

    bool writeTemperature = true;

    void updateSimulation();
    void updateUI();
    QColor getColorForTemperature(double temp) const;
};

#endif
