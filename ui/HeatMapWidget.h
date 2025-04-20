#ifndef HEATMAPWIDGET_H
#define HEATMAPWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPixmap>
#include <QtCore/qelapsedtimer.h>

#include "../logic/OpenCLSimulation.h"

class HeatMapWidget : public QWidget {
    Q_OBJECT

public:
    explicit HeatMapWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QTimer simulationTimer;
    QTimer uiTimer;
    QPixmap heatmapCache;

    OpenCLSimulation simulation;
    int gridWidth;
    int gridHeight;

    void updateSimulation();
    void updateUI();
    QColor getColorForTemperature(float temp) const;

    QElapsedTimer fpsTimer;
    int frameCount = 0;
    double currentFps = 0.0;
    double lastUpdateDuration = 0.0;

    bool showStatsWindow = false;
};

#endif
