#ifndef TEMPERATURECELL_H
#define TEMPERATURECELL_H

class TemperatureCell {
public:
    double temperature;
    double diffusionRate;

    TemperatureCell(double temp = 0.0, double diffRate = 0.1);
};

#endif
