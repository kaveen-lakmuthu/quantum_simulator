# Compiler and Flags
CXX = g++
CXXFLAGS = -I/usr/include/eigen3 -I/usr/include/gtest -Ibackend/src -Wall -Wextra -std=c++17 -O3
LDFLAGS = -lgtest -lgtest_main -pthread

# Target Executables
TARGET = quantum_simulator
TEST_TARGET = run_tests

# Source Files
SRC = backend/src/main.cpp backend/src/qubit_manager.cpp backend/src/gate_engine.cpp backend/src/circuit_manager.cpp backend/src/utils.cpp
TEST_SRC = backend/tests/test_runner.cpp backend/tests/test_qubit_manager.cpp backend/tests/test_gate_engine.cpp backend/tests/test_circuit_manager.cpp

# Build Rules
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

$(TEST_TARGET): $(TEST_SRC) backend/src/qubit_manager.cpp backend/src/gate_engine.cpp backend/src/circuit_manager.cpp backend/src/utils.cpp
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_SRC) backend/src/qubit_manager.cpp backend/src/gate_engine.cpp backend/src/circuit_manager.cpp backend/src/utils.cpp $(LDFLAGS)

# Clean Rule
clean:
	rm -f $(TARGET) $(TEST_TARGET)
