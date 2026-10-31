import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

WidgetScrollable {
    property QtObject value_context: null
    property string enabled_key: ""
    property string caption: ""
    property string tooltip: ""

    default property alias contentSlot: contentContainer.data

    RowLayout {
        id: pageHeaderRow
        spacing: 6

        Label {
            text: root.caption
            font.pointSize: 11
        }

        WidgetHelpIcon {
            tooltipText: root.tooltip
        }

        Item {
            Layout.fillWidth: true
        }

        CheckBox {
            id: headerCheckbox

            indicator.implicitWidth: 18
            indicator.implicitHeight: 18

            checked: config.enabledModules[enabled_key]
            text: qsTr("Enable this tab")

            onClicked: config.enabledModules[enabled_key] = checked
        }
        /*
        Button {
            text: qsTr("Reset to default")
            onClicked: value_context.resetToDefault()
        }*/
    }

    ColumnLayout {
        id: contentContainer
        enabled: config.enabledModules[enabled_key]
    }
}
