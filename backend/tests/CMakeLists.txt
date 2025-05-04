cmake_minimum_required(VERSION 3.10)
project(QuantumSimulatorTests)

# Find required packages
find_package(GTest REQUIRED)
find_package(Eigen3 REQUIRED)

# Include directories
include_directories(
    ${EIGEN3_INCLUDE_DIR}
    ../src
)

# Add test executables
add_executable(
    quantum_tests
    test_circuit_manager.cpp
    test_gate_engine.cpp
    test_qubit_manager.cpp
    test_runner.cpp
    ../src/circuit_manager.cpp
    ../src/gate_engine.cpp
    ../src/qubit_manager.cpp
    ../src/utils.cpp
)

# Link libraries
target_link_libraries(
    quantum_tests
    GTest::GTest
    GTest::Main
    pthread
)

# Add test
enable_testing()
add_test(NAME quantum_tests COMMAND quantum_tests)