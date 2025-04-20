cmake_minimum_required(VERSION 3.30)
project(thermal_distribution_simulation)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt
find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED
)

# Set OpenCL as a system framework manually for macOS
if(APPLE)
    set(OPENCL_LIBRARIES "-framework OpenCL")
else()
    find_package(OpenCL REQUIRED)
endif()

# Main executable
add_executable(thermal_distribution_simulation
        main.cpp
        ui/HeatMapWidget.h
        ui/HeatMapWidget.cpp
        logic/OpenCLSimulation.h
        logic/OpenCLSimulation.cpp
)

target_link_libraries(thermal_distribution_simulation
        Qt::Core
        Qt::Gui
        Qt::Widgets
        ${OPENCL_LIBRARIES}
)

configure_file(kernel.cl kernel.cl COPYONLY)
