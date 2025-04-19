#ifndef OPENCLSIMULATION_H
#define OPENCLSIMULATION_H

#include <vector>
#include <OpenCL/opencl.h>

class OpenCLSimulation {
public:
    OpenCLSimulation(int width, int height); // ← updated constructor
    ~OpenCLSimulation();

    void step();
    const std::vector<float>& getTemperatureData() const;

private:
    int width;
    int height;
    std::vector<float> temperature;
    std::vector<float> diffusion;

    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;
    cl_mem tempBuffer;
    cl_mem diffBuffer;

    void initOpenCL();
    void loadKernel();
    void allocateBuffers();
};

#endif // OPENCLSIMULATION_H
