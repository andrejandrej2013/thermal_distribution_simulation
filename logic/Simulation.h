#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <random>
#include "../dto/TemperatureCell.h"

class Simulation {
public:
    void update();
    const std::vector<std::vector<TemperatureCell>>& getTemperatureGrid() const;
    void generateRandomTemperatureGrid(int width = 40, int height = 40);

private:
    int width = 0, height = 0;
    std::vector<std::vector<TemperatureCell>> temperatureGrid;
};

#endif
