import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

ComboBox {
    id: internalCombo
    Layout.fillWidth: false
    implicitContentWidthPolicy: ComboBox.WidestTextWhenCompleted

    property QtObject value_key: null

    currentIndex: value_key.value

    topPadding: 2
    bottomPadding: 2
    leftPadding: 4

    onActivated: value_key.value = currentIndex
}
