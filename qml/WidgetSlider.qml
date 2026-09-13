import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

WidgetCommonControl {
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
            handle.implicitWidth: 16
            handle.implicitHeight: 16

            onMoved: {
                root.value = internalSlider.value;
                root.setValue();
                textInput.text = root.value.toString();
            }
            live: true
        }

        // Right-aligned direct integer readout box matching your UI
        RowLayout {
            spacing: 4

            TextField {
                id: textInput

                color: textInput.palette.text

                background: Rectangle {
                    implicitWidth: 45
                    implicitHeight: 22
                    color: textInput.palette.base
                    border.color: textInput.acceptableInput ? palette.mid : "red"
                    border.width: textInput.acceptableInput ? 1 : 2
                }

                text: root.value.toString()
                validator: IntValidator {
                    bottom: root.from
                    top: root.to
                }
                selectByMouse: true

                // Allow input box typing to update the slider position
                onEditingFinished: {
                    root.value = parseInt(text);
                    root.setValue();
                }
            }

            Label {
                text: root.suffix
                Layout.minimumWidth: 20
            }
        }
    }
}
