import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuantumSim 1.0

/**
 * @file MainWindow.qml
 * @brief Main application window for quantum circuit simulator
 * 
 * Provides modern UI with three-panel layout:
 * - Left: Gate selection and circuit controls
 * - Center: Visual circuit diagram (top) and gate list (bottom)
 * - Right: Real-time quantum state display
 * 
 * Uses Catppuccin-inspired dark theme for professional appearance.
 */
ApplicationWindow {
    id: root
    visible: true
    width: 1200
    height: 900
    title: "Quantum Circuit Simulator"

    color: "#1e1e2e"

    // Style configuration
    palette {
        buttonText: "#ffffff"
        button: "#313244"
        base: "#1e1e2e"
        text: "#cdd6f4"
        highlight: "#89b4fa"
    }

    // Header
    header: Rectangle {
        color: "#313244"
        height: 60

        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 15

            Text {
                text: "Quantum Circuit Simulator"
                font.pixelSize: 24
                font.bold: true
                color: "#89b4fa"
            }

            Item { Layout.fillWidth: true }

            RowLayout {
                spacing: 10

                Text {
                    text: "Qubits:"
                    color: "#cdd6f4"
                    font.pixelSize: 14
                }

                ComboBox {
                    id: qubitCombo
                    model: ["1", "2", "3", "4", "5"]
                    currentIndex: 4  // Default to 5 qubits
                    onCurrentValueChanged: {
                        backend.setQubitCount(parseInt(currentValue))
                        // Clear initial state when qubit count changes
                        initialStateInput.text = ""
                    }
                }
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 15
        spacing: 15

        // Control Panel
        Rectangle {
            Layout.fillWidth: true
            color: "#313244"
            radius: 8
            height: 120

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 10

                Text {
                    text: "Initial State (binary, e.g., 010):"
                    color: "#cdd6f4"
                    font.pixelSize: 12
                }

                RowLayout {
                    spacing: 10

                    TextField {
                        id: initialStateInput
                        placeholderText: "e.g., 01010"
                        Layout.preferredWidth: 200
                        background: Rectangle {
                            color: "#1e1e2e"
                            border.color: "#45475a"
                            border.width: 1
                            radius: 4
                        }
                    }

                    Button {
                        text: "Set Initial State"
                        onClicked: {
                            if (initialStateInput.text.length > 0) {
                                backend.setInitialState(initialStateInput.text)
                            }
                        }
                    }

                    Item { Layout.fillWidth: true }
                }
            }
        }

        // Main Content Area
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 15

            // Left Panel - Gates
            Rectangle {
                Layout.preferredWidth: 250
                Layout.fillHeight: true
                color: "#313244"
                radius: 8

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15
                    spacing: 10

                    Text {
                        text: "Single-Qubit Gates"
                        color: "#89b4fa"
                        font.bold: true
                        font.pixelSize: 14
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 5
                        Text {
                            text: "Target:"
                            color: "#cdd6f4"
                            font.pixelSize: 12
                        }
                        ComboBox {
                            id: singleQubitTarget
                            model: backend.getAvailableQubits()
                            Layout.fillWidth: true
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 8

                        Button {
                            Layout.fillWidth: true
                            text: "Hadamard (H)"
                            onClicked: backend.addGate("H", parseInt(singleQubitTarget.currentValue))
                        }

                        Button {
                            Layout.fillWidth: true
                            text: "Pauli-X"
                            onClicked: backend.addGate("X", parseInt(singleQubitTarget.currentValue))
                        }

                        Button {
                            Layout.fillWidth: true
                            text: "Pauli-Y"
                            onClicked: backend.addGate("Y", parseInt(singleQubitTarget.currentValue))
                        }

                        Button {
                            Layout.fillWidth: true
                            text: "Pauli-Z"
                            onClicked: backend.addGate("Z", parseInt(singleQubitTarget.currentValue))
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        height: 1
                        color: "#45475a"
                    }

                    Text {
                        text: "Multi-Qubit Gates"
                        color: "#89b4fa"
                        font.bold: true
                        font.pixelSize: 14
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 8

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 5
                            Text {
                                text: "Target:"
                                color: "#cdd6f4"
                            }
                            ComboBox {
                                id: targetQubit
                                model: backend.getAvailableQubits()
                                Layout.fillWidth: true
                            }
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 5
                            Text {
                                text: "Control:"
                                color: "#cdd6f4"
                            }
                            ComboBox {
                                id: controlQubit
                                model: backend.getAvailableQubits()
                                Layout.fillWidth: true
                            }
                        }

                        Button {
                            Layout.fillWidth: true
                            text: "CNOT"
                            onClicked: {
                                backend.addGate("CNOT", parseInt(targetQubit.currentValue), parseInt(controlQubit.currentValue))
                            }
                        }

                        Button {
                            Layout.fillWidth: true
                            text: "SWAP"
                            onClicked: {
                                backend.addGate("SWAP", parseInt(targetQubit.currentValue), parseInt(controlQubit.currentValue))
                            }
                        }
                    }

                    Item { Layout.fillHeight: true }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 8

                        Button {
                            Layout.fillWidth: true
                            text: "Execute"
                            highlighted: true
                            onClicked: {
                                backend.executeCircuit()
                                successMessage.visible = true
                                successTimer.start()
                            }
                        }

                        Button {
                            Layout.fillWidth: true
                            text: "Clear"
                            onClicked: backend.clearCircuit()
                        }
                    }
                }
            }

            // Middle - Circuit Visualization (Split View)
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#313244"
                radius: 8

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15
                    spacing: 10

                    Text {
                        text: "Quantum Circuit"
                        color: "#89b4fa"
                        font.bold: true
                        font.pixelSize: 14
                    }

                    // Top half: Visual circuit diagram
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "#1e1e2e"
                        radius: 4
                        border.color: "#45475a"
                        border.width: 1

                        CircuitPainter {
                            id: circuitCanvas
                            anchors.fill: parent
                            qubitCount: backend.qubitCount
                            gates: backend.circuitGates
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        height: 1
                        color: "#45475a"
                    }

                    Text {
                        text: "Gate Sequence"
                        color: "#89b4fa"
                        font.bold: true
                        font.pixelSize: 12
                    }

                    // Bottom half: Gate list
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 150
                        color: "#1e1e2e"
                        radius: 4
                        border.color: "#45475a"
                        border.width: 1

                        ListView {
                            id: circuitListView
                            anchors.fill: parent
                            anchors.margins: 10
                            spacing: 5
                            clip: true
                            
                            model: backend.circuitGates
                            
                            delegate: Rectangle {
                                width: circuitListView.width
                                height: 30
                                color: "#313244"
                                radius: 4
                                
                                Text {
                                    anchors.centerIn: parent
                                    text: modelData
                                    color: "#cdd6f4"
                                    font.family: "Courier"
                                    font.pixelSize: 12
                                }
                            }
                            
                            Text {
                                anchors.centerIn: parent
                                text: "No gates added yet.\nAdd gates using the controls on the left."
                                color: "#6c7086"
                                visible: circuitListView.count === 0
                                horizontalAlignment: Text.AlignHCenter
                            }
                        }
                    }
                }
            }

            // Right Panel - Results
            Rectangle {
                Layout.preferredWidth: 300
                Layout.fillHeight: true
                color: "#313244"
                radius: 8

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15
                    spacing: 10

                    Text {
                        text: "Quantum State"
                        color: "#89b4fa"
                        font.bold: true
                        font.pixelSize: 14
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "#1e1e2e"
                        radius: 4
                        border.color: "#45475a"
                        border.width: 1

                        ScrollView {
                            anchors.fill: parent
                            anchors.margins: 10
                            clip: true

                            Text {
                                width: parent.width
                                text: {
                                    var output = "Initial Quantum State:\n" + backend.initialState;
                                    
                                    if (backend.circuitGates.length > 0) {
                                        output += "\nDefined Quantum Circuit:\n";
                                        for (var i = 0; i < backend.circuitGates.length; i++) {
                                            output += backend.circuitGates[i] + "\n";
                                        }
                                    }
                                    
                                    if (backend.circuitExecuted) {
                                        output += "\nExecuting Quantum Circuit...\n";
                                        output += "\nFinal Quantum State:\n" + backend.quantumState;
                                    }
                                    
                                    return output;
                                }
                                color: "#a6adc8"
                                font.family: "Courier"
                                font.pixelSize: 11
                                wrapMode: Text.WordWrap
                            }
                        }
                    }
                }
            }
        }

        // Messages
        Rectangle {
            id: successMessage
            Layout.fillWidth: true
            height: 40
            color: "#40b4fa"
            radius: 4
            visible: false

            Text {
                anchors.centerIn: parent
                text: "Circuit executed successfully!"
                color: "#1e1e2e"
                font.bold: true
            }

            Timer {
                id: successTimer
                interval: 3000
                onTriggered: successMessage.visible = false
            }
        }
    }

    Connections {
        target: backend
        function onExecutionError(error) {
            errorDialog.text = error
            errorDialog.visible = true
        }
    }

    Dialog {
        id: errorDialog
        property string text
        title: "Error"
        modal: true
        anchors.centerIn: parent

        ColumnLayout {
            anchors.fill: parent
            Text {
                text: errorDialog.text
                color: "#cdd6f4"
            }
            Button {
                Layout.alignment: Qt.AlignRight
                text: "OK"
                onClicked: errorDialog.close()
            }
        }
    }
}
