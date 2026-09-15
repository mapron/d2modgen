import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion
import QtQuick.Dialogs
import CustomValidators 1.0

WidgetScrollable {
    id: root
    property var value_context: appModuleMain

    RowLayout {
        spacing: 6

        Label {
            text: qsTr("Main")
            font.pointSize: 11
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

        control: RowLayout {
            Layout.fillWidth: true

            DynamicLineEdit {
                validator: BasicPathValidator {}
                Layout.fillWidth: true
                value_context: root.value_context
                value_key: "inputPath"
            }
            Button {
                text: qsTr("Detect!")
                onClicked: appui.detectPath()
            }
        }
    }

    WidgetCommonControl {
        caption: qsTr("Output folder (optional, leave empty to auto-detect):")
        tooltip: ""

        control: DynamicLineEdit {
            Layout.fillWidth: true
            value_context: root.value_context
            value_key: "outputPath"
        }
    }

    RowLayout {
        spacing: 10
        Layout.fillWidth: true
        Label {
            text: qsTr("Target D2 Game version:")
        }

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

        ButtonGroup {
            id: radioGroupMajor
            onClicked: button => {
                versionMajor.dynamicValue = button.buttonIndex;
            }
        }
        ButtonGroup {
            id: radioGroupD2R
            onClicked: button => {
                versionD2R.dynamicValue = button.buttonIndex;
            }
        }

        Repeater {
            model: [qsTr("D2 Legacy"), qsTr("D2 Resurrected")]

            RadioButton {
                text: modelData
                ButtonGroup.group: radioGroupMajor
                property int buttonIndex: index
                checked: index === versionMajor.dynamicValue
            }
        }

        Rectangle {
            visible: versionMajor.dynamicValue == 1
            Layout.fillHeight: true
            Layout.preferredWidth: 1
            color: palette.accent
        }
        Repeater {
            model: [qsTr("D2R - LoD"), qsTr("D2R - RotW")]
            RadioButton {
                text: modelData
                visible: versionMajor.dynamicValue == 1
                ButtonGroup.group: radioGroupD2R
                property int buttonIndex: index
                checked: index === versionD2R.dynamicValue
            }
        }

        Item {
            Layout.fillWidth: true
        }
    }

    WidgetComboBox {
        caption: qsTr("Input mode, where to read game data:")
        tooltip: qsTr("This determines source data that is used by randomizer<br><b>Game data archive</b> - For D2R Resurrected, this is CASC storage;<br><b>Game data archive</b> - For D2 Legacy this is .mpq file;<br><b>Folder with extracted data</b> - If you want to use D2R select folder containing modinfo.json, otherwise select game root<br><b>Only .txt</b> - Folder with .txt files")
        model: [qsTr("Game data archive (CASC or Storm=mpq)"), qsTr("Folder with extracted data"), qsTr("Only .txt")]
        value_context: root.value_context
        value_key: "inputMode"
    }

    WidgetComboBox {
        caption: qsTr("Output format, how files and folders will be created:")
        tooltip: qsTr("This determines output type:<br><b>D2 Resurrected mod</b> - create D2R mod with all needed subfolders, path is determined by config name (see Extra tab)<br><b>Full folders</b> - create folder structure (data/global/...) with txt/json<br><b>Only .txt</b> - Folder with .txt files, note that for D2R that will mean json files will not be written<br><b>Dry run</b> - read input, process randomizer, but don't write anything")
        model: [qsTr("D2 Resurrected mod"), qsTr("Full folders"), qsTr("Only .txt"), qsTr("Dry run")]
        value_context: root.value_context
        value_key: "outputMode"
    }

    RowLayout {
        spacing: 10
        Layout.fillWidth: true

        Label {
            text: qsTr("Random seed:")
        }
        WidgetHelpIcon {
            tooltipText: qsTr("Seed determine how reproduceable generation results are.<br>If seed stays the same, output of generation is also always exactly the same.<br>If you want each generation be stable and reproduceable, remove <b>Create new seed</b>")
        }
        DynamicLineEdit {
            value_context: root.value_context
            value_key: "seed"

            Layout.minimumWidth: 100
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
}
