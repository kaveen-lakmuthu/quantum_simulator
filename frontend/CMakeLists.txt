cmake_minimum_required(VERSION 3.10)
project(QuantumSimulatorGUI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Ensure Qt is included
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

# Explicitly include Eigen
include_directories(/usr/include/eigen3)
include_directories(../backend/src)

set(BACKEND_SRC
    ../backend/src/qubit_manager.cpp
    ../backend/src/circuit_manager.cpp
    ../backend/src/gate_engine.cpp
    ../backend/src/utils.cpp
)

add_executable(quantum_simulator_gui 
    src/main.cpp 
    src/mainwindow.cpp src/mainwindow.h
    src/circuit_view.cpp src/circuit_view.h
    src/results_window.cpp src/results_window.h  # Add result window
    ${BACKEND_SRC}
)

# Link Qt
target_link_libraries(quantum_simulator_gui Qt6::Core Qt6::Widgets Qt6::Gui)
