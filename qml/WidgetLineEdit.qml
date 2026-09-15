import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

WidgetCommonControl {
    property alias value_context: internal.value_context
    property alias value_key: internal.value_key

    control: DynamicLineEdit {
        id: internal
        Layout.fillWidth: true
    }
}
