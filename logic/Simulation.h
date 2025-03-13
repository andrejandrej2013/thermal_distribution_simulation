#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <random>
#include "../dto/TemperatureCell.h"
#include <CL/cl.h>

class Simulation {
public:
    Simulation(int width = 40, int height = 40);
    void update();
    const std::vector<std::vector<TemperatureCell>>& getTemperatureGrid() const;

private:
    int width, height;
    std::vector<std::vector<TemperatureCell>> temperatureGrid;
};

#endif
