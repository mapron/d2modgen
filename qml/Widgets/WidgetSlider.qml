import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

WidgetCommonControl {
    property alias value_key: internal.value_key

    control: DynamicExponentialSlider {
        id: internal
    }
}
