import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

RowLayout {
    spacing: 10
    Layout.fillWidth: true

    property alias value_context: item.value_context
    property alias value_key: item.value_key
    property alias from: internalSlider.from
    property alias to: internalSlider.to

    DynamicItemInt {
        id: item
    }

    Slider {
        id: internalSlider
        Layout.fillWidth: true
        from: 0
        to: 100
        value: item.dynamicValue
        handle.implicitWidth: 16
        handle.implicitHeight: 16

        onMoved: item.dynamicValue = value
        live: true
    }

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

        text: item.dynamicValue.toString()

        validator: IntValidator {
            bottom: internalSlider.from
            top: internalSlider.to
        }
        selectByMouse: true

        onEditingFinished: item.dynamicValue = parseInt(text)
    }
}
