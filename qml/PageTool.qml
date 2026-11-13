/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion
import QtQuick.Dialogs
import "./Widgets"

WidgetScrollable {
    id: root
    property var value_context: appConfig
    property bool isD2R: appConfig.versionMajor.value === 1

    RowLayout {
        id: pageHeaderRow
        spacing: 6

        LabelLarge {
            text: qsTr("Extra helpful utilities")
        }

        Item {
            Layout.fillWidth: true
        }
    }

    Separator {}

    WidgetLineEdit {
        caption: qsTr("Mod id:")
        tooltip: qsTr("That will affect game's saves folder subdirectory and mod subfolder for D2R")
        stretchCaption: false
        visible: root.isD2R
        value_key: root.value_context.modname
    }

    WidgetFolder {
        caption: qsTr("Wine user root:")
        tooltip: qsTr("Path to user root folder (drive_c/Users/username) of the Wine/Proton prefix where D2R is installed")
        isCompact: false
        value_key: root.value_context.userHome
        visible: Qt.platform.os === "linux"
    }

    ColumnLayout {
        Layout.fillWidth: true
        spacing: 4
        visible: root.isD2R

        Label {
            text: qsTr("D2R folder for mod save and settings:")
        }
        TextField {
            id: user_settings
            readOnly: true
            text: appConfig.userHome.value + appConfig.saveSubfolder.value + appConfig.modname.value
            Layout.fillWidth: true
        }
    }

    RowLayout {
        Layout.fillWidth: true
        spacing: 4
        visible: root.isD2R

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

    Separator {
        visible: root.isD2R
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
            text: (root.isD2R ? "-mod " + appConfig.modname.value : "-direct -3dfx") + " -txt "
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
            text: Qt.platform.os === "linux" ? qsTr("Make shortcut") : qsTr("Make shortcut on Desktop")
            Layout.preferredWidth: 200
            onClicked: appui.makeShortCut(command_line.text)
        }
        WidgetHelpIcon {
            visible: Qt.platform.os === "linux"
            tooltipText: qsTr("Adds a launcher to the application menu and to the Desktop.")
        }
    }

    WidgetLineEdit {
        property string preset: appui.runnerPreset(appConfig.userHome.value, !root.isD2R)

        caption: qsTr("Runner template:")
        tooltip: qsTr("Command the shortcut runs: {prefix} is the Wine prefix, {exe} the game executable, {args} the command line above.<br>Leave it empty to use the preset.")
        isCompact: false
        value_key: root.value_context.runnerTemplate
        placeholderText: preset !== "" ? preset : qsTr("No preset for this Wine prefix: enter a command")
        visible: Qt.platform.os === "linux"
    }

    RowLayout {
        spacing: 10
        Layout.fillWidth: true
        visible: root.isD2R

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

    WidgetComboBox {
        isCompact: false
        caption: qsTr("Locale id for D2R:")
        tooltip: ""
        model: ["enUS", "deDE", "esES", "esMX", "frFR", "itIT", "jaJP", "koKR", "plPL", "ptBR", "ruRU", "zhCN", "zhTW"]
        value_key: root.value_context.d2rLocaleId
    }
    WidgetLineEdit {
        caption: qsTr("Extra trace ids, comma-separated:")
        tooltip: qsTr("That is mainly used by developer, or to collect extra traces for support.")
        stretchCaption: false
        value_key: root.value_context.extraTrace
    }
}
