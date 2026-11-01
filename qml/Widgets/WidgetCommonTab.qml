import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

WidgetScrollable {
    property QtObject value_context: null
    property QtObject enabled_key: null

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

            visible: !!enabled_key
            checked: enabled_key ? enabled_key.value : false
            text: qsTr("Enable this tab")

            onClicked: enabled_key.value = checked
        }

        Button {
            text: qsTr("Clear values")
            onClicked: value_context.resetToZero()
        }
        Button {
            text: qsTr("Set recommended")
            onClicked: value_context.resetToRecommended()
        }
    }

    ColumnLayout {
        id: contentContainer
        enabled: enabled_key ? enabled_key.value : true
    }
}
