import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion
import QtQuick.Dialogs

RowLayout {
    Label {
        text: "Main"
    }

    // ----------------------------------------------------
    // FILE DIALOG (Replacement for QFileDialog)
    // ----------------------------------------------------
    FileDialog {
        id: fileDialog
        title: qsTr("Please choose a file")
        //currentFolder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        nameFilters: ["Text files (*.txt)", "All files (*)"]

        onAccepted: {
            console.log("User selected file: " + fileDialog.selectedFile);
            // Tip: Note the switch from 'fileUrl' (Qt5) to 'selectedFile' (Qt6)
        }
        onRejected: {
            console.log("File selection canceled");
        }
    }

    // ----------------------------------------------------
    // MESSAGE DIALOG (Replacement for QMessageBox)
    // ----------------------------------------------------
    MessageDialog {
        id: msgDialog
        title: qsTr("Critical Action Required")
        text: qsTr("Do you want to save changes before exiting?")
        buttons: MessageDialog.Save | MessageDialog.Discard | MessageDialog.Cancel

        onButtonClicked: (button, role) => {
            if (button === MessageDialog.Save) {
                console.log("Save clicked");
            } else if (button === MessageDialog.Discard) {
                console.log("Discard clicked");
            }
        }
    }

    // ----------------------------------------------------
    // UI LAYOUT TO TRIGGER THEM
    // ----------------------------------------------------
    Column {
        spacing: 15

        Button {
            text: qsTr("Open File...")
            onClicked: fileDialog.open() // 💡 Use .open() to trigger
        }

        Button {
            text: qsTr("Show Alert Message")
            onClicked: msgDialog.open()  // 💡 Use .open() to trigger
        }
    }
}
