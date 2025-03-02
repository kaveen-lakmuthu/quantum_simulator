# Compiler and Flags
CXX = g++
NASM = nasm
CXXFLAGS = -I/usr/include/eigen3 -I/usr/include/gtest -Ibackend/src -Wall -Wextra -std=c++17
LDFLAGS = -lgtest -lgtest_main -pthread

# Target Executables
TARGET = quantum_simulator
TEST_TARGET = run_tests

# Source Files
SRC = backend/src/main.cpp backend/src/qubit_manager.cpp backend/src/gate_engine.cpp backend/src/circuit_manager.cpp backend/src/utils.cpp
TEST_SRC = backend/tests/test_runner.cpp backend/tests/test_qubit_manager.cpp backend/tests/test_gate_engine.cpp backend/tests/test_circuit_manager.cpp

# Assembly Files
ASM_SRC = backend/assembly/parallel_ops.asm
ASM_OBJ = backend/assembly/parallel_ops.o

# Build Rules
$(TARGET): $(SRC) $(ASM_OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(ASM_OBJ)

$(TEST_TARGET): $(TEST_SRC) backend/src/qubit_manager.cpp backend/src/gate_engine.cpp backend/src/circuit_manager.cpp backend/src/utils.cpp $(ASM_OBJ)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_SRC) backend/src/qubit_manager.cpp backend/src/gate_engine.cpp backend/src/circuit_manager.cpp backend/src/utils.cpp $(ASM_OBJ) $(LDFLAGS)

# Compile Assembly Files
%.o: %.asm
	$(NASM) -f elf64 -o $@ $<

# Clean Rule
clean:
	rm -f $(TARGET) $(TEST_TARGET) $(ASM_OBJ)
