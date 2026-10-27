import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

ColumnLayout {
    id: root
    // Base properties for data and strings
    property string caption: ""
    property string tooltip: ""
    property bool isCompact: false // do not place caption on separate baseline
    property bool controlAfterCaption: true // used only for isCompact==true
    property bool hideCaption: false // used only for isCompact==true
    property bool stretchCaption: false // used only for isCompact==true

    property string suffix: "" // Useful for adding "%" or units

    spacing: 4
    Layout.fillWidth: true

    required property Item control

    RowLayout {
        spacing: 6
        visible: !root.isCompact
        Layout.fillWidth: true

        Label {
            text: root.caption
        }

        WidgetHelpIcon {
            tooltipText: root.tooltip
        }

        Item {
            Layout.fillWidth: true
        }
    }

    RowLayout {
        spacing: 6
        RowLayout {
            spacing: 6
            visible: root.isCompact && root.controlAfterCaption
            Layout.fillWidth: root.stretchCaption
            Layout.horizontalStretchFactor: root.stretchCaption ? 1 : -1

            Label {
                visible: !hideCaption
                text: root.caption
            }

            WidgetHelpIcon {
                tooltipText: root.tooltip
            }
            Item {
                visible: root.stretchCaption
                Layout.fillWidth: root.stretchCaption
            }
        }
        RowLayout {
            id: contentContainer
            Layout.horizontalStretchFactor: 1
            Layout.fillWidth: true
        }
        RowLayout {
            spacing: 6
            visible: root.isCompact && !root.controlAfterCaption

            Label {
                visible: !hideCaption
                text: root.caption
            }

            WidgetHelpIcon {
                tooltipText: root.tooltip
            }
        }

        Component.onCompleted: {
            root.control.parent = contentContainer;
        }
    }
}
