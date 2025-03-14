#include "Simulation.h"

void Simulation::generateRandomTemperatureGrid(int width, int height) {
    this->height = height;
    this->width = width;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> tempDist(-10.0, 50.0);
    std::uniform_real_distribution<double> diffDist(0.05, 0.15);

    temperatureGrid.resize(height, std::vector<TemperatureCell>(width));
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            temperatureGrid[y][x] = TemperatureCell(tempDist(gen), diffDist(gen));
        }
    }
}


void Simulation::update() {
    std::vector<std::vector<TemperatureCell>> newGrid = temperatureGrid;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double sum = temperatureGrid[y][x].temperature;
            int count = 1;

            if (x > 0) { sum += temperatureGrid[y][x - 1].temperature; count++; }
            if (x < width - 1) { sum += temperatureGrid[y][x + 1].temperature; count++; }
            if (y > 0) { sum += temperatureGrid[y - 1][x].temperature; count++; }
            if (y < height - 1) { sum += temperatureGrid[y + 1][x].temperature; count++; }

            double avgTemp = sum / count;
            newGrid[y][x].temperature += (avgTemp - temperatureGrid[y][x].temperature) * temperatureGrid[y][x].diffusionRate;
        }
    }

    temperatureGrid = newGrid;
}

const std::vector<std::vector<TemperatureCell>>& Simulation::getTemperatureGrid() const {
    return temperatureGrid;
}
