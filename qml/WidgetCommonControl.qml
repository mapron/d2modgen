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

    property int value: getValue()
    property string suffix: "" // Useful for adding "%" or units

    property string value_context: "global"
    property string value_key: "key"

    function getValue() {
        return appui.getInt(value_context, value_key);
    }

    function setValue() {
        //console.log("SetValue");
        //console.log(value_context);
        //console.log(value_key);
        //console.log(value);
        appui.set(value_context, value_key, value);
    }

    signal appDataChanged

    // Grid properties adjust based on compactness
    columns: isCompact ? 2 : 1
    rowSpacing: 1
    columnSpacing: 10
    Layout.fillWidth: true
    uniformCellWidths: true

    // THE MAGIC: Child files drop inputs directly into the data array alias
    default property alias contentSlot: contentContainer.data

    Connections {
        target: appui
        function onDataChanged() {
            root.appDataChanged();
        }
    }

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
            //Layout.fillWidth: true
        }

        WidgetHelpIcon {
            tooltipText: root.tooltip
        }

        // This spacer absorbs extra row width, keeping text left-aligned
        // when a CheckBox sits to the left of it.
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
