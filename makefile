# Compiler
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Target executable
TARGET = quantum_simulator

# Source files
SRC = src/main.cpp src/qubit_manager.cpp src/gate_engine.cpp

# Build the target
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Clean build artifacts
clean:
	rm -f $(TARGET)
