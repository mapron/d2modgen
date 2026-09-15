import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

WidgetCommonControl {
    property alias value_context: internal.value_context
    property alias value_key: internal.value_key
    property alias suffix: internal.suffix

    property alias denom: internal.denom
    property alias mult: internal.mult

    control: DynamicExponentialSlider {
        id: internal
        suffix: "%"
    }
}
