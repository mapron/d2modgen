import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion
import QtQuick.Dialogs

WidgetScrollable {

    property var value_context: appModuleMain

    RowLayout {
        id: pageHeaderRow
        spacing: 6

        Label {
            text: qsTr("Tool")
            font.pointSize: 11
        }

        WidgetHelpIcon {
            tooltipText: qsTr("Main")
        }

        Item {
            Layout.fillWidth: true
        }
    }

    ColumnLayout {
        Layout.fillWidth: true
        spacing: 4

        Label {
            text: qsTr("D2R save and user settings root:")
        }
        TextField {
            id: d2rSaveRootInput
            text: "C:/Users/mapron/Saved Games/Diablo II Resurrected/"
            Layout.fillWidth: true
        }
    }

    ColumnLayout {
        Layout.fillWidth: true
        spacing: 4

        Button {
            id: copySettingsCheck
            text: qsTr("Copy Settings.json")
        }
        Label {
            text: qsTr("(this will copy default settings to mod folder)")
        }
    }

    // -------------------------------------------------------------
    // GROUP 3: Commands & Desktop Launch Actions
    // -------------------------------------------------------------
    ColumnLayout {
        Layout.fillWidth: true
        spacing: 4

        Label {
            text: qsTr("D2R command arguments (read-only):")
        }
        TextField {
            id: commandArgsInput
            text: "-mod rando -txt"
            readOnly: true
            Layout.fillWidth: true
            // Visual indicator that it is read-only using palette fallback
            //palette.base: mainWindow.palette.disabled.base
        }
    }

    Button {
        text: qsTr("Make shortcut on Desktop")
        Layout.preferredWidth: 200
    }

    RowLayout {
        spacing: 10
        Layout.fillWidth: true

        Button {
            text: qsTr("Reset launch to unmodded")
            Layout.fillWidth: true
        }
        Button {
            text: qsTr("Set launch to mod")
            Layout.fillWidth: true
        }
        Label {
            text: qsTr("(Close Battlenet launcher before actions!)")
            font.bold: true
            Layout.fillWidth: true
        }
    }
}
