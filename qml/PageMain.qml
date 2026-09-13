import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion
import QtQuick.Dialogs

WidgetScrollable {

    property string value_context: "main"

    RowLayout {
        id: pageHeaderRow
        spacing: 6

        Label {
            text: qsTr("Main")
            font.pointSize: 11
        }

        WidgetHelpIcon {
            tooltipText: qsTr("Main")
        }

        Item {
            Layout.fillWidth: true
        }

        Button {
            text: qsTr("Reset to default")
            onClicked: {
                appui.resetToDefault(value_context);
            }
        }
    }

    RowLayout {
        spacing: 10
        Layout.fillWidth: true

        Label {
            text: qsTr("Mod id:")
            Layout.preferredWidth: 60
        }
        TextField {
            id: modIdInput
            text: "rando"
            Layout.fillWidth: true
        }
    }

    CheckBox {
        id: legacyInstallCheck
        text: qsTr("Use Diablo II legacy installation")
    }

    CheckBox {
        id: targetWarlockCheck
        text: qsTr("Target for D2R Reign of the Warlock")
        checked: true
    }

    ColumnLayout {
        Layout.fillWidth: true
        spacing: 4

        Label {
            text: qsTr("D2R path:")
        }
        TextField {
            id: d2rPathInput
            text: "E:\\Games\\Diablo II Resurrected\\"
            Layout.fillWidth: true
        }
    }

    RowLayout {
        spacing: 10
        Layout.fillWidth: true

        Label {
            text: qsTr("Random seed:")
        }
        TextField {
            id: randomSeedInput
            text: "2246554441"
            Layout.preferredWidth: 150
        }
    }

    ColumnLayout {
        Layout.fillWidth: true
        spacing: 4

        Label {
            text: qsTr("Output directory (leave empty to output to D2R path):")
        }
        TextField {
            id: outputDirInput
            text: ""
            Layout.fillWidth: true
        }
    }

    CheckBox {
        id: exportTxtCheck
        text: qsTr("Export all *.txt (for further manual edit)")
    }
}
