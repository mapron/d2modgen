import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

CheckBox {
    id: checkBox

    property QtObject value_key: null

    checked: value_key.value

    indicator.implicitWidth: 18
    indicator.implicitHeight: 18

    onClicked: value_key.value = checked
}
