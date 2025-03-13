cmake_minimum_required(VERSION 3.30)
project(thermal_distribution_simulation)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
  Core
  Gui
  Widgets
  REQUIRED)

add_executable(thermal_distribution_simulation main.cpp
        ui/HeatMapWidget.h
        ui/HeatMapWidget.cpp
        logic/Simulation.h
        logic/Simulation.cpp
        dto/TemperatureGrid.h
        dto/TemperatureGrid.cpp
        dto/TemperatureCell.h
        dto/TemperatureCell.cpp)
target_link_libraries(thermal_distribution_simulation
  Qt::Core
  Qt::Gui
  Qt::Widgets
)

