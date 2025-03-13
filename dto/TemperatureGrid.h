#ifndef TEMPERATUREGRID_H
#define TEMPERATUREGRID_H

#include <vector>

class TemperatureGrid {
public:
    TemperatureGrid(int width, int height);
    void updateFromSimulation(const std::vector<std::vector<double>>& newData);
    const std::vector<std::vector<double>>& getData() const;

private:
    std::vector<std::vector<double>> temperatureData;
};

#endif
