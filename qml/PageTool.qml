import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion
import QtQuick.Dialogs

WidgetScrollable {
    id: root
    property var value_context: appModuleMain

    DynamicItemInt {
        id: versionMajor
        value_context: root.value_context
        value_key: "versionMajor"
    }
    DynamicItemInt {
        id: versionD2R
        value_context: root.value_context
        value_key: "versionD2R"
    }

    RowLayout {
        id: pageHeaderRow
        spacing: 6

        Label {
            text: qsTr("Extra helpful utilities")
            font.pointSize: 11
        }

        Item {
            Layout.fillWidth: true
        }
    }

    WidgetCheckbox {
        caption: qsTr("Force output for all txt/json (for further manual edit)")
        tooltip: qsTr("This will force to output all game files,<br>not only used by randomizer in current configuration.")
        value_context: root.value_context
        value_key: "exportAll"
    }

    Rectangle {
        Layout.fillWidth: true
        Layout.preferredHeight: 1
        color: palette.accent
    }

    WidgetCommonControl {
        caption: qsTr("Mod id:")
        tooltip: qsTr("That will affect game's saves folder subdirectory and mod subfolder for D2R")
        isCompact: true
        visible: versionMajor.dynamicValue == 1

        control: DynamicLineEdit {
            id: modname_edit
            Layout.minimumWidth: 100
            value_context: root.value_context
            value_key: "modname"
        }
    }

    ColumnLayout {
        Layout.fillWidth: true
        spacing: 4
        visible: versionMajor.dynamicValue == 1

        Label {
            text: qsTr("D2R folder for mod save and settings:")
        }
        TextField {
            id: user_settings
            readOnly: true
            text: appui.saveFolder + modname_edit.text
            Layout.fillWidth: true
        }
    }

    RowLayout {
        Layout.fillWidth: true
        spacing: 4
        visible: versionMajor.dynamicValue == 1

        Button {
            text: qsTr("Copy Settings.json")
            onClicked: appui.copyModSettings()
        }
        WidgetHelpIcon {
            tooltipText: qsTr("This will copy Game Settings to mod folder<br>That will allow to skip configuring mod on launch.")
        }
        Item {
            Layout.minimumWidth: 30
        }
        Button {
            text: qsTr("Copy all save files (.d2s)")
            onClicked: appui.copySaves()
        }
        WidgetHelpIcon {
            tooltipText: qsTr("this will copy all your character save files to modded folder.")
        }
    }

    Rectangle {
        visible: versionMajor.dynamicValue == 1
        Layout.fillWidth: true
        Layout.preferredHeight: 1
        color: palette.accent
    }

    // -------------------------------------------------------------
    // GROUP 3: Commands & Desktop Launch Actions
    // -------------------------------------------------------------
    ColumnLayout {
        Layout.fillWidth: true
        spacing: 4

        Label {
            text: qsTr("D2 commandline arguments (read-only):")
        }
        TextField {
            id: command_line
            readOnly: true
            text: (versionMajor.dynamicValue == 0 ? "-direct" : "-mod " + modname_edit.text) + " -txt "
            Layout.fillWidth: true
        }
    }

    Label {
        text: qsTr("Choose convenient way to launch the mod:")
    }

    RowLayout {
        Label {
            text: qsTr("Option 1:")
        }

        Button {
            text: qsTr("Make shortcut on Desktop")
            Layout.preferredWidth: 200
            onClicked: appui.makeShortCut(command_line.text)
        }
    }

    RowLayout {
        spacing: 10
        Layout.fillWidth: true
        visible: versionMajor.dynamicValue == 1

        Label {
            text: qsTr("Option 2:")
        }
        Button {
            text: qsTr("Reset launch to unmodded")
            Layout.fillWidth: true
            onClicked: appui.setLaunch("")
        }
        Button {
            text: qsTr("Set launch to mod")
            Layout.fillWidth: true
            onClicked: appui.setLaunch(command_line.text)
        }
        Label {
            text: qsTr("(Close Battlenet launcher before actions!)")
            font.bold: true
            Layout.fillWidth: true
        }
    }
}
