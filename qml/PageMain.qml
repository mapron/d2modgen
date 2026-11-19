/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion
import QtQuick.Dialogs
import CustomValidators 1.0
import "./Widgets"

WidgetScrollable {
    id: root
    property var value_context: appConfig

    RowLayout {
        spacing: 6

        LabelLarge {
            text: qsTr("Main")
        }

        Button {
            text: qsTr("Clear global app config")
            onClicked: appConfig.resetToZero()
        }
        Item {
            Layout.fillWidth: true
        }
    }

    WidgetFolder {
        caption: qsTr("Input folder (mandatory):")
        tooltip: Qt.platform.os === "linux" ? qsTr("Generally, just choose folder with Game.exe.<br>Note: for ProjectD2, select subfolder named 'ProjectD2' in Diablo II.<br>Press Detect! to search Wine/Proton prefixes for the game; it fills both the input folder and the Wine user root.") : qsTr("Generally, just choose folder with Game.exe.<br>Note: for ProjectD2, select subfolder named 'ProjectD2' in Diablo II.<br>Press Detect! to find information in Windows Registry.")

        validateNonEmpty: true
        showDetect: true

        value_key: root.value_context.inputPath
    }

    WidgetFolder {
        caption: qsTr("Output folder (optional, leave empty to auto-detect):")
        value_key: root.value_context.outputPath
    }

    RowLayout {
        spacing: 10
        Layout.fillWidth: true
        Label {
            text: qsTr("Target D2 Game version:")
        }
        RadioGroup {
            model: [qsTr("D2 Legacy"), qsTr("D2 Resurrected")]
            value_key: appConfig.versionMajor
        }
        Rectangle {
            visible: appConfig.versionMajor.value === 1
            Layout.fillHeight: true
            Layout.preferredWidth: 1
            color: palette.accent
        }
        RadioGroup {
            visible: appConfig.versionMajor.value === 1
            model: [qsTr("D2R - LoD"), qsTr("D2R - RotW")]
            value_key: appConfig.versionD2R
        }
        Item {
            Layout.fillWidth: true
        }
    }

    WidgetComboBox {
        caption: qsTr("Input mode, where to read game data:")
        tooltip: qsTr("This determines source data that is used by randomizer<br><b>Game data archive</b> - For D2R Resurrected, this is CASC storage;<br><b>Game data archive</b> - For D2 Legacy this is .mpq file;<br><b>Folder with extracted data</b> - If you want to use D2R select folder containing modinfo.json, otherwise select game root<br><b>Only .txt</b> - Folder with .txt files")
        model: [qsTr("Game data archive (CASC or Storm=mpq)"), qsTr("Folder with extracted data"), qsTr("Only .txt")]
        value_key: root.value_context.inputMode
        stretchCaption: false
    }

    WidgetComboBox {
        caption: qsTr("Output format, how files and folders will be created:")
        tooltip: qsTr("This determines output type:<br><b>D2 Resurrected mod</b> - create D2R mod with all needed subfolders, path is determined by config name (see Extra tab)<br><b>Full folders</b> - create folder structure (data/global/...) with txt/json<br><b>Only .txt</b> - Folder with .txt files, note that for D2R that will mean json files will not be written<br><b>Dry run</b> - read input, process randomizer, but don't write anything")
        model: [qsTr("D2 Resurrected mod"), qsTr("Full folders"), qsTr("Only .txt"), qsTr("Dry run")]
        value_key: root.value_context.outputMode
        stretchCaption: false
    }
    WidgetCheckbox {
        caption: qsTr("Create new seed on every Generate call")
        tooltip: qsTr("Default behavior: when you press generate, then if nothing changes, your mod will be exactly the same.<br>With this option Generate will produce different result, updating seeds on all 3 randomizer tabs")
        value_key: root.value_context.refreshSeed
    }
}
