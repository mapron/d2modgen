import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

WidgetCommonControl {
    id: root
    property alias value_context: internal.value_context
    property alias value_key: internal.value_key
    property alias checked: internal.checked

    isCompact: true
    controlAfterCaption: false
    hideCaption: true

    control: DynamicCheckbox {
        id: internal
        text: root.caption
    }
}
