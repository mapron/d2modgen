import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

GridLayout {
    id: root
    // Base properties for data and strings
    property string caption: ""
    property string tooltip: ""
    property bool isCompact: false // do not place caption on separate baseline
    property bool controlAfterCaption: true // used only for isCompact==true
    property bool hideCaption: false // used only for isCompact==true

    property string suffix: "" // Useful for adding "%" or units

    // Grid properties adjust based on compactness
    columns: isCompact ? 2 : 1
    rowSpacing: 1
    columnSpacing: 10
    Layout.fillWidth: true
    uniformCellWidths: true

    default property alias contentSlot: contentContainer.data

    RowLayout {
        id: headerRow
        spacing: 6

        // Dynamically compute grid cell based on switches
        Layout.row: 0
        Layout.column: (!root.isCompact) ? 0 : (root.controlAfterCaption ? 0 : 1)
        Layout.fillWidth: true

        Label {
            visible: !root.hideCaption
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
        id: contentContainer
        spacing: 10

        // Dynamically shift cells down or sideways
        Layout.row: root.isCompact ? 0 : 1
        Layout.column: root.isCompact ? (root.controlAfterCaption ? 1 : 0) : 0

        // Let sliders expand, but allow compact widgets to remain tightly bound
        Layout.fillWidth: !root.isCompact || root.controlAfterCaption
    }
}
