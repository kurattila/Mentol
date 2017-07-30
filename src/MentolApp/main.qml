import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0

ApplicationWindow {
    visible: true
    width: 850
    height: 600
    title: qsTr("Mentol")

    property color mentolLightGray:  'lightgray'
    property color mentolLightGreen: '#23a96e'
    property color mentolDarkGreen:  '#23473e'

    color: mentolLightGreen

    ColumnLayout {
        x: 5
        y: 5
        width: parent.width - 10
        height: parent.height - 10

        MouseArea {
            Layout.fillWidth: true
            Layout.preferredHeight: Math.max(50, parent.height / 10)
            onClicked: mainViewModel.keypadShown = !mainViewModel.keypadShown;

            Text {
                id: valueInput
                text: mainViewModel.keypadProcessorWholeInput + " v"
                anchors.fill: parent
                font.pixelSize: height * 0.9
                color: 'white'
                font.weight: Font.Black
                horizontalAlignment: TextInput.AlignRight
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                anchors.fill: parent

                Text {
                    id: textualVisualization
                    Layout.preferredHeight: parent.height / 2
                    text: mainViewModel.textualVisualization;
                    font.pixelSize: valueInput.font.pixelSize
                    color: mentolLightGray
                }

                Column {
                    id: canvasColumn
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Row {
                        id: legendCaptions
                        spacing: 20
                        Text {
                            width: canvasColumn.width / 2 - 10
                            text: 'Difficulty'
                            color: mentolDarkGreen
                            font.pixelSize: valueInput.height * 0.3
                            font.weight: Font.Bold
                            horizontalAlignment: Text.AlignRight
                        }
                        Text {
                            text: 'Precision'
                            color: mentolDarkGreen
                            font.pixelSize: valueInput.height * 0.3
                            font.weight: Font.Black
                        }
                    }

                    Repeater {
                        id: canvasRepeater
                        model: mainViewModel.complexityDistribution

                        MouseArea {
                            onPressed: mainViewModel.setUserPreferenceOfComplexity(Math.abs(100 - model.precision))
                            hoverEnabled: true
        //                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                            onEntered: if (pressed) mainViewModel.setUserPreferenceOfComplexity(Math.abs(100 - model.precision))

                            width: canvasColumn.width
                            height: (canvasColumn.height - legendCaptions.height) / canvasRepeater.count

                            Row {
                                anchors.fill: parent
                                layoutDirection: "RightToLeft"

                                Item {
                                    width: canvasColumn.width / 2
                                    height: parent.height

                                    Rectangle {
                                        id: precisionBar
                                        color: mentolDarkGreen
                                        border.width: 7
                                        radius: 14
                                        border.color: mentolLightGreen
                                        opacity: model.isUserPreferred ? 1 : 0.9
                                        height: parent.height
                                        width: 0.01 * canvasColumn.width / 2 * (100 - Math.abs(100 - model.precision))

                                        Text {
                                            width: parent.width
                                            height: parent.height
                                            text: model.precisionText
                                            color: model.isUserPreferred ? mentolLightGray : mentolLightGreen
                                            font.pixelSize: valueInput.height * 0.5
                                            font.weight: Font.Bold
                                            horizontalAlignment: Text.AlignRight
                                            verticalAlignment: Text.AlignVCenter
                                            x: -20
                                        }
                                    }
                                }

                                Rectangle {
                                    width: model.complexityNormalized * canvasColumn.width / 2
                                    height: parent.height
                                    color: mentolDarkGreen
                                    border.width: 7
                                    radius: 14
                                    border.color: mentolLightGreen
                                    opacity: model.isUserPreferred ? 1 : 0.9
                                }
                            }
                        }
                    }
                }
            }

            Keypad {
                visible: mainViewModel.keypadShown
                width: parent.width
                height: parent.height
                background: mentolLightGreen
                onButtonPressed: mainViewModel.keypadButtonPressed(buttonText);
            }

        }
    }
}
