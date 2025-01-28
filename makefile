# Compiler and Flags
CXX = g++
CXXFLAGS = -I/usr/include/eigen3 -Wall -Wextra -std=c++17

# Target
TARGET = quantum_simulator

# Source Files
SRC = backend/src/main.cpp backend/src/qubit_manager.cpp backend/src/gate_engine.cpp backend/src/utils.cpp

# Build Rule
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Clean Rule
clean:
	rm -f $(TARGET)
