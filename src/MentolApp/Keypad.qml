import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

Item {
    id: root

    property int buttonSpacing: 7
    property int buttonWidth: (width - 3*buttonSpacing) / 4
    property int buttonHeight: (height - 4*buttonSpacing) / 5
    property color background
    signal buttonPressed(string buttonText)

    Rectangle {
        anchors.fill: parent
        color: background

        GridLayout {
            columns: 4
            columnSpacing: buttonSpacing
            rowSpacing: buttonSpacing

            KeypadButton {
                keypadText: '7'
                Layout.preferredWidth: buttonWidth
                Layout.preferredHeight: buttonHeight
                onClicked: buttonPressed(keypadText)
            }

            KeypadButton {
                keypadText: '8'
                Layout.preferredWidth: buttonWidth
                Layout.preferredHeight: buttonHeight
                onClicked: buttonPressed(keypadText)
            }

            KeypadButton {
                keypadText: '9'
                Layout.preferredWidth: buttonWidth
                Layout.preferredHeight: buttonHeight
                onClicked: buttonPressed(keypadText)
            }

            KeypadButton {
                keypadText: '<--'
                Layout.preferredWidth: buttonWidth
                Layout.preferredHeight: buttonHeight
                onClicked: buttonPressed('delChar')
            }

            KeypadButton {
                keypadText: '4'
                Layout.preferredWidth: buttonWidth
                Layout.preferredHeight: buttonHeight
                onClicked: buttonPressed(keypadText)
            }

            KeypadButton {
                keypadText: '5'
                Layout.preferredWidth: buttonWidth
                Layout.preferredHeight: buttonHeight
                onClicked: buttonPressed(keypadText)
            }

            KeypadButton {
                keypadText: '6'
                Layout.preferredWidth: buttonWidth
                Layout.preferredHeight: buttonHeight
                onClicked: buttonPressed(keypadText)
            }

            KeypadButton {
                keypadText: 'C'
                Layout.preferredWidth: buttonWidth
                Layout.preferredHeight: buttonHeight
                onClicked: buttonPressed(keypadText)
            }

            KeypadButton {
                keypadText: '1'
                Layout.preferredWidth: buttonWidth
                Layout.preferredHeight: buttonHeight
                onClicked: buttonPressed(keypadText)
            }

            KeypadButton {
                keypadText: '2'
                Layout.preferredWidth: buttonWidth
                Layout.preferredHeight: buttonHeight
                onClicked: buttonPressed(keypadText)
            }

            KeypadButton {
                keypadText: '3'
                Layout.preferredWidth: buttonWidth
                Layout.preferredHeight: buttonHeight
                onClicked: buttonPressed(keypadText)
            }

            KeypadButton {
                keypadText: '1/x'
                Layout.preferredWidth: buttonWidth
                Layout.preferredHeight: buttonHeight
                onClicked: buttonPressed(keypadText)
            }

            KeypadButton {
                keypadText: '0'
                Layout.columnSpan: 2
                Layout.preferredWidth: 2 * buttonWidth + buttonSpacing
                Layout.preferredHeight: buttonHeight
                onClicked: buttonPressed(keypadText)
            }

            KeypadButton {
                keypadText: '.'
                Layout.preferredWidth: buttonWidth
                Layout.preferredHeight: buttonHeight
                onClicked: buttonPressed(keypadText)
            }

            Item {}

            KeypadButton {
                keypadText: 'Cancel'
                pixelSize: height / 3
                Layout.columnSpan: 2
                Layout.preferredWidth: 2 * buttonWidth + buttonSpacing
                Layout.preferredHeight: buttonHeight
                onClicked: buttonPressed(keypadText)
            }

            KeypadButton {
                keypadText: 'OK'
                pixelSize: height / 3
                Layout.columnSpan: 2
                Layout.preferredWidth: 2 * buttonWidth + buttonSpacing
                Layout.preferredHeight: buttonHeight
                onClicked: buttonPressed(keypadText)
            }
        }
    }
}

