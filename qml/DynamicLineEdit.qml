import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

TextField {
    id: textInput

    property alias value_context: item.value_context
    property alias value_key: item.value_key

    DynamicItemStr {
        id: item
    }

    color: textInput.palette.text

    text: item.dynamicValue

    selectByMouse: true

    onEditingFinished: item.dynamicValue = text

    background: Rectangle {
        color: textInput.palette.base
        border.color: textInput.acceptableInput ? palette.accent : "red"
        border.width: textInput.acceptableInput ? 1 : 2
    }
}
