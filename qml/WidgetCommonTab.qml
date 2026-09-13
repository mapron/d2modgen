import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

WidgetScrollable {
    property var value_context: null
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

            checked: value_context.enabled
            text: qsTr("Enable this tab")

            onClicked: value_context.enabled = checked
        }
        Button {
            text: qsTr("Reset to default")
            onClicked: value_context.resetToDefault()
        }
    }

    ColumnLayout {
        id: contentContainer
        enabled: value_context.enabled
    }
}
