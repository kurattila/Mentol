import QtQuick 2.0
import QtQuick.Controls 1.2

Button {
    id: root
    property alias keypadText: buttonText.text
    property alias pixelSize: buttonText.font.pixelSize

    Text {
        id: buttonText
        anchors.centerIn: parent
        font.pixelSize: root.height / 2
    }
}

