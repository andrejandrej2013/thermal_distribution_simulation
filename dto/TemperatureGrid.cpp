#include "TemperatureGrid.h"

TemperatureGrid::TemperatureGrid(int width, int height) {
    temperatureData.resize(height, std::vector<double>(width, 0.0));
}

void TemperatureGrid::updateFromSimulation(const std::vector<std::vector<double>>& newData) {
    temperatureData = newData;
}

const std::vector<std::vector<double>>& TemperatureGrid::getData() const {
    return temperatureData;
}
