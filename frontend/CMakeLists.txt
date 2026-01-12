cmake_minimum_required(VERSION 3.10)
project(QuantumSimulatorGUI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Ensure Qt is included
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Test Qml Quick)

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
    src/main_qml.cpp
    src/backend_bridge.cpp src/backend_bridge.h
    src/circuit_painter.cpp src/circuit_painter.h
    ${BACKEND_SRC}
    resources.qrc
)

# Link Qt
target_link_libraries(quantum_simulator_gui Qt6::Core Qt6::Gui Qt6::Qml Qt6::Quick)

# =============================
# GUI Unit Tests (Qt Test)
# =============================
enable_testing()

add_executable(gui_tests
    tests/test_gui.cpp
    src/backend_bridge.cpp src/backend_bridge.h
    src/circuit_painter.cpp src/circuit_painter.h
    ${BACKEND_SRC}
)

target_link_libraries(gui_tests Qt6::Core Qt6::Gui Qt6::Test Qt6::Qml Qt6::Quick)

add_test(NAME GuiTests COMMAND gui_tests)
