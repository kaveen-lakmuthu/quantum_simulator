# 🚀 Quantum Circuit Simulator

A **Quantum Circuit Simulator** with a **Qt-based GUI** and a **C++ backend** for quantum gate simulation.  
This project allows users to visually design and simulate quantum circuits.

---

## **📜 Features**
✅ **Graphical Quantum Circuit Editor** – Add, remove, and modify quantum gates.  
✅ **Backend Simulation Engine** – Executes circuits using **Eigen** for linear algebra.  
✅ **Gate Support** – Hadamard, Pauli-X/Y/Z, CNOT, SWAP.  
✅ **Multi-Qubit Control** – Supports multi-qubit operations and controlled gates.  
✅ **Assembly-Level Optimization** – Performance boost using inline assembly.  

---

## **🛠️ Installation & Setup**
### **📌 Prerequisites**
- **Arch Linux** (or any Linux distro)
- **C++17 Compiler** (`g++` or `clang++`)
- **CMake** (`pacman -S cmake`)
- **Qt6** (`pacman -S qt6-base`)
- **Eigen3** (`pacman -S eigen`)

### **📥 Clone the Repository**
```bash
git clone https://github.com/kaveen-lakmuthu/quantum_simulator.git
cd quantum_simulator
```

### **⚙️ Build and Run**
```bash
cd frontend
mkdir -p build && cd build
cmake ..
make -j$(nproc)
./quantum_simulator_gui
```

---

## **🖥️ Usage**
1. **Select the number of qubits** from the dropdown.
2. **Click on a gate** (Hadamard, Pauli, CNOT, etc.).
3. **Choose a target qubit** (and control qubit for multi-qubit gates).
4. **Click "Execute Circuit"** to simulate.
5. **View results** in the output window.

---

## **📂 Project Structure**
```
quantum_simulator/
│── backend/                # Quantum simulation logic
|   |
│   ├── src/                # Source code
|   |   |
│   │   ├── main.cpp        # Entry point (CLI mode)
│   │   ├── qubit_manager.h # Qubit state management
│   │   ├── gate_engine.h   # Gate operations
│   │   +── circuit_manager.h # Circuit execution logic
|   |
│   +── tests/              # Unit tests
|
│── frontend/               # GUI using Qt6
|   |
│   +── src/                
│       ├── mainwindow.h    # Main GUI window
│       ├── circuit_view.h  # Visual representation of circuit
│       +── results_window.h # Output display
|
│── CMakeLists.txt          # Build configuration
│── README.md               # Project documentation
+── LICENSE                 # GNU GPL License file
```

---

## **🛠️ Development & Future Improvements**
✅ **Planned Features:**
- Quantum measurement support  
- Support for phase gates (S, T)  
- Save & load circuits  


<!-- 🚀 **Want to Contribute?**  
Feel free to **fork, submit issues, or open a PR**. 🎉   -->

---

## **📜 License**
This project is licensed under the **GNU General Public License v3.0 (GPL-3.0)**.  
See the full license in the **`LICENSE`** file or visit:  
[GNU GPL v3.0](https://www.gnu.org/licenses/gpl-3.0.html)

---

## **📞 Contact**
📧 Email: [kaveensalakmuthu@gmail.com]  
🌐 GitHub: [github.com/kaveen-lakmuthu](https://github.com/kaveen-lakmuthu)  
