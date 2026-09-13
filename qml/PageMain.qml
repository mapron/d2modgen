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

    WidgetCommonControl {
        caption: qsTr("Input folder (mandatory):")
        tooltip: qsTr("Generally, just choose folder with Game.exe.<br>Note: for ProjectD2, select subfolder named 'ProjectD2' in Diablo II.<br>Press Detect! to find information in Windows Registry.")

        DynamicLineEdit {
            Layout.minimumWidth: 300
            value_context: root.value_context
            value_key: "inputPath"
        }
        Button {
            text: qsTr("Detect!")
            onClicked: appui.detectPath()
        }
    }

    WidgetCommonControl {
        caption: qsTr("Output folder (optional, leave empty to auto-detect):")
        tooltip: ""

        DynamicLineEdit {
            Layout.minimumWidth: 300
            value_context: root.value_context
            value_key: "outputPath"
        }
    }

    WidgetComboBox {
        caption: qsTr("Target D2 Game version:")
        model: [qsTr("D2 Legacy (Pre-D2R): 1.14, ProjectD2"), qsTr("D2 Resurrected - LoD"), qsTr("D2 Resurrected - RotW"),]
        value_context: root.value_context
        value_key: "version"
        isCompact: true
        implicitWidth: 100
    }

    WidgetComboBox {
        caption: qsTr("Input mode, where to read game data:")
        tooltip: qsTr("This determines source data that is used by randomizer<br>Game data archive - For D2R Resurrected, this is CASC storage;<br>Game data archive - For D2 Legacy this is .mpq files;<br>Folder with extracted data - If you want to use D2R select folder containing modinfo.json<br>Last option is for folder with .txt files")
        model: [qsTr("Game data archive"), qsTr("Folder with extracted data"), qsTr("Only .txt")]
        value_context: root.value_context
        value_key: "inputMode"
        isCompact: true
        implicitWidth: 100
    }

    WidgetComboBox {
        caption: qsTr("Output format, how files and folders will be created:")
        tooltip: qsTr("This determines output type:<br>First one will create D2R mod with all needed subfolders,<br>Second one will create folder structure with txt/json<br>Last one will only output .txt")
        model: [qsTr("D2 Resurrected mod"), qsTr("Full folders"), qsTr("Only .txt")]
        value_context: root.value_context
        value_key: "outputMode"
        isCompact: true
        implicitWidth: 100
    }

    WidgetCommonControl {
        caption: qsTr("Mod id:")
        tooltip: qsTr("That will affect game's saves folder subdirectory and mod subfolder for D2R")
        isCompact: true

        DynamicLineEdit {
            Layout.minimumWidth: 100
            value_context: root.value_context
            value_key: "modname"
        }
    }

    RowLayout {
        spacing: 10
        Layout.fillWidth: true

        Label {
            text: qsTr("Random seed:")
        }
        WidgetHelpIcon {
            tooltipText: qsTr("Seed determine how reproduceable generation results are.<br>If seed stays the same, output of generation is also always exactly the same.")
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

    WidgetCheckbox {
        caption: qsTr("Force output for all txt/json (for further manual edit)")
        tooltip: qsTr("This will force to output all game files,<br>not only used by randomizer in current configuration.")
        value_context: root.value_context
        value_key: "exportAll"
    }
}
