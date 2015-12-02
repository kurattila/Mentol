import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Button {
    id: root
    property alias keypadText: buttonText.text
    property alias pixelSize: buttonText.font.pixelSize
    property bool isSpecialButton

    style: ButtonStyle {
        background: Rectangle {
            implicitWidth: 100
            implicitHeight: 25
            border.width: control.activeFocus ? 2 : 1
            border.color: "#888"
            radius: 4
            color: {
                if (isSpecialButton)
                    return control.pressed ? "#afa" : "#bfb";
                else
                    return control.pressed ? "#ccc" : "#eee";
            }
//            gradient: Gradient {
//                GradientStop { position: 0 ; color: control.pressed ? "#ccc" : "#eee" }
//                GradientStop { position: 1 ; color: control.pressed ? "#aaa" : "#ccc" }
//            }
        }
    }

    Text {
        id: buttonText
        anchors.centerIn: parent
        font.pixelSize: root.height / 2
    }
}

