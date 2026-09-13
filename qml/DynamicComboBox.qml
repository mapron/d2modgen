import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

ComboBox {
    id: internalCombo
    Layout.fillWidth: false
    implicitContentWidthPolicy: ComboBox.WidestTextWhenCompleted

    property alias value_context: item.value_context
    property alias value_key: item.value_key

    DynamicItemInt {
        id: item
    }
    currentIndex: item.dynamicValue

    topPadding: 2
    bottomPadding: 2
    leftPadding: 4

    onActivated: item.dynamicValue = index
}
