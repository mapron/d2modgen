import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

AppCommonControl {
    id: root

    property alias from: internalSlider.from
    property alias to: internalSlider.to

    onAppDataChanged: {
        value = getValue();
        internalSlider.value = value;
        textInput.text = value.toString();
    }

    // Bottom Row: Slider control + Numeric input layout
    RowLayout {
        spacing: 10
        Layout.fillWidth: true

        Slider {
            id: internalSlider
            Layout.fillWidth: true
            from: 0
            to: 100
            value: root.value
            handle.implicitWidth: 18
            handle.implicitHeight: 18

            onMoved: {
                root.value = internalSlider.value;
                root.setValue();
            }
            live: true
        }

        // Right-aligned direct integer readout box matching your UI
        RowLayout {
            spacing: 4

            Rectangle {

                width: 45
                height: 22
                radius: 2
                border.width: textInput.acceptableInput ? 1 : 2
                border.color: textInput.acceptableInput ? palette.mid : "red"

                TextInput {
                    id: textInput
                    anchors.centerIn: parent
                    text: root.value.toString()
                    validator: IntValidator {
                        bottom: root.from
                        top: root.to
                    }
                    font.pointSize: 10
                    selectByMouse: true

                    // Allow input box typing to update the slider position
                    onAccepted: {
                        root.value = parseInt(text);
                        root.setValue();
                    }
                }
            }

            Label {
                text: root.suffix
                visible: root.suffix !== ""
            }
        }
    }
}
