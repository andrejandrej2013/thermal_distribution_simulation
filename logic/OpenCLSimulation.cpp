#include "OpenCLSimulation.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

OpenCLSimulation::OpenCLSimulation(int w, int h) : width(w), height(h) {
    temperature.resize(width * height);
    diffusion.resize(width * height);

    // Random init
    for (int i = 0; i < width * height; ++i) {
        temperature[i] = static_cast<float>(rand() % 60 - 10); // -10°C to 50°C
        diffusion[i] = 0.01f + static_cast<float>(rand()) / RAND_MAX * 0.2f;
    }

    initOpenCL();
    loadKernel();
    allocateBuffers();
}

OpenCLSimulation::~OpenCLSimulation() {
    clReleaseMemObject(tempBuffer);
    clReleaseMemObject(diffBuffer);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
}

void OpenCLSimulation::initOpenCL() {
    clGetPlatformIDs(1, &platform, nullptr);
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, nullptr);
    context = clCreateContext(nullptr, 1, &device, nullptr, nullptr, nullptr);
    queue = clCreateCommandQueue(context, device, 0, nullptr);
}

void OpenCLSimulation::loadKernel() {
    std::ifstream kernelFile("kernel.cl");
    std::string source((std::istreambuf_iterator<char>(kernelFile)),
                        std::istreambuf_iterator<char>());

    const char* src = source.c_str();
    size_t len = source.size();

    program = clCreateProgramWithSource(context, 1, &src, &len, nullptr);
    if (clBuildProgram(program, 0, nullptr, nullptr, nullptr, nullptr) != CL_SUCCESS) {
        char log[4096];
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(log), log, nullptr);
        std::cerr << "Build error:\n" << log << std::endl;
        throw std::runtime_error("Failed to build OpenCL program");
    }

    kernel = clCreateKernel(program, "updateTemp", nullptr);
}

void OpenCLSimulation::allocateBuffers() {
    tempBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
                                sizeof(float) * temperature.size(), temperature.data(), nullptr);

    diffBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                sizeof(float) * diffusion.size(), diffusion.data(), nullptr);

    clSetKernelArg(kernel, 0, sizeof(cl_mem), &tempBuffer);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &diffBuffer);
    clSetKernelArg(kernel, 2, sizeof(int), &width);
    clSetKernelArg(kernel, 3, sizeof(int), &height);
}

void OpenCLSimulation::step() {
    size_t globalSize[2] = { static_cast<size_t>(width), static_cast<size_t>(height) };

    clEnqueueNDRangeKernel(queue, kernel, 2, nullptr, globalSize, nullptr, 0, nullptr, nullptr);
    clFinish(queue);

    clEnqueueReadBuffer(queue, tempBuffer, CL_TRUE, 0, sizeof(float) * temperature.size(), temperature.data(), 0, nullptr, nullptr);
}

const std::vector<float>& OpenCLSimulation::getTemperatureData() const {
    return temperature;
}
