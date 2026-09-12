import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

ScrollView {
    clip: true
    // Hides scrollbar until the layout height drops below contents height requirements
    ScrollBar.vertical.policy: ScrollBar.AsNeeded
    ScrollBar.horizontal.policy: ScrollBar.AsNeeded

    property string value_context: ""
    property string caption: ""
    property string tooltip: ""

    default property alias contentSlot: contentContainer.data

    ColumnLayout {
        //anchors.centerIn: parent

        id: contentContainer
        //width: 500
        spacing: 16

        RowLayout {
            id: pageHeaderRow
            spacing: 6

            // Dynamically compute grid cell based on switches
            Layout.fillWidth: true

            Label {
                text: root.caption
                font.pointSize: 12
            }

            HelpIcon {
                tooltipText: root.tooltip
            }
        }
    }
}
