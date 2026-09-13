import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion
import QtQuick.Dialogs

WidgetScrollable {
    id: root
    property var value_context: appModuleMain

    RowLayout {
        spacing: 6

        Label {
            text: qsTr("Main")
            font.pointSize: 11
        }

        WidgetHelpIcon {
            tooltipText: qsTr("Main")
        }

        Button {
            text: qsTr("Reset to default")
            onClicked: value_context.resetToDefault()
        }
        Item {
            Layout.fillWidth: true
        }
    }

    WidgetLineEdit {
        caption: qsTr("Mod id:")
        tooltip: ""
        value_context: root.value_context
        value_key: "modname"
        isCompact: true
        implicitWidth: 100
    }

    WidgetCheckbox {
        id: isLegacy
        caption: qsTr("Use Diablo II legacy installation")
        tooltip: ""
        value_context: root.value_context
        value_key: "isLegacy"
    }

    WidgetCheckbox {
        caption: qsTr("Target for D2R Reign of the Warlock")
        tooltip: ""
        value_context: root.value_context
        value_key: "d2rUseROTW"
        visible: !isLegacy.checked
    }

    WidgetLineEdit {
        caption: qsTr("D2 Resurrected game root:")
        tooltip: ""
        value_context: root.value_context
        value_key: "d2rPath"
        implicitWidth: 300
        visible: !isLegacy.checked
    }
    WidgetLineEdit {
        caption: qsTr("D2 Legacy game root:")
        tooltip: ""
        value_context: root.value_context
        value_key: "d2legacyPath"
        implicitWidth: 300
        visible: isLegacy.checked
    }

    RowLayout {
        spacing: 10
        Layout.fillWidth: true

        Label {
            text: qsTr("Random seed:")
        }
        DynamicLineEdit {
            value_context: root.value_context
            value_key: "seed"
        }
        Button {
            text: qsTr("Generate new")
            onClicked: appui.newSeed()
        }
        WidgetCheckbox {
            caption: qsTr("Create new seed on every Generate call")
            tooltip: ""
            value_context: root.value_context
            value_key: "refreshSeed"
        }
    }

    WidgetLineEdit {
        caption: qsTr("Output directory (leave empty to output to D2R path):")
        tooltip: ""
        value_context: root.value_context
        value_key: "outPath"
    }

    WidgetCheckbox {
        caption: qsTr("Export all *.txt (for further manual edit)")
        tooltip: ""
        value_context: root.value_context
        value_key: "exportTxtCheck"
    }
}
