import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

CheckBox {
    id: checkBox

    property alias value_context: item.value_context
    property alias value_key: item.value_key

    DynamicItemInt {
        id: item
    }
    checked: item.dynamicValue === 1

    indicator.implicitWidth: 18
    indicator.implicitHeight: 18

    onClicked: item.dynamicValue = checked ? 1 : 0
}
