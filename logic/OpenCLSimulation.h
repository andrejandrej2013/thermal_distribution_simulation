#ifndef OPENCLSIMULATION_H
#define OPENCLSIMULATION_H

#include <vector>
#include <OpenCL/opencl.h>
#include <QtCore/qtypes.h>

class OpenCLSimulation {
public:
    OpenCLSimulation(int width, int height);
    ~OpenCLSimulation();

    void step();
    const std::vector<float>& getTemperatureData() const;
    void generateColorImage(uchar* pixelBuffer);

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
    cl_kernel colorKernel;
    cl_mem outputColorBuffer;
    cl_mem tempBuffer;
    cl_mem diffBuffer;

    void initOpenCL();
    void loadKernel();
    void allocateBuffers();
};

#endif // OPENCLSIMULATION_H
