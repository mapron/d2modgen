import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion
import QtQuick.Dialogs

MenuBar {
    id: mainMenuBar
    FileDialog {
        id: fileDialogSave
        fileMode: FileDialog.SaveFile
        nameFilters: ["Config (*.json)"]

        onAccepted: {
            appui.saveConfig(fileDialogSave.selectedFile);
        }
    }

    FileDialog {
        id: fileDialogOpen
        fileMode: FileDialog.OpenFile
        nameFilters: ["Config (*.json)"]

        onAccepted: {
            appui.loadConfig(fileDialogOpen.selectedFile);
        }
    }

    Menu {
        id: mainMenuFile
        delegate: MenuItem {
            id: menuItem
            implicitHeight: 26

            contentItem: RowLayout {
                spacing: 20
                width: menuItem.width - menuItem.leftPadding - menuItem.rightPadding

                // Left Text Component (Action name)
                Label {
                    text: menuItem.text
                    color: menuItem.highlighted ? window.palette.highlightedText : window.palette.text
                    Layout.fillWidth: true
                }

                // Right Text Component (The Shortcut description indicator)
                Label {
                    text: menuItem.action && menuItem.action.shortcut ? menuItem.action.shortcut : ""
                    color: menuItem.highlighted ? window.palette.highlightedText : window.palette.text
                    horizontalAlignment: Text.AlignRight
                }
            }
        }

        title: qsTr("File") // The '&' enables standard Alt+F keyboard navigation mnemonics

        Action {
            text: qsTr("Save config...")
            shortcut: "Ctrl+S"
            onTriggered: fileDialogSave.open()
        }
        Action {
            text: qsTr("Load config...")
            shortcut: "Ctrl+O"
            onTriggered: fileDialogOpen.open()
        }
        Action {
            text: qsTr("Clear config")
            shortcut: "Ctrl+N"
            onTriggered: appui.clearConfig()
        }

        Menu {
            title: qsTr("Config from preset")

            Action {
                text: "basic"
            }
            Action {
                text: "friendly_chaos"
            }
            Action {
                text: "magic_rando"
            }
            Action {
                text: "oh_no"
            }
        }
        Action {
            text: qsTr("Browse to settings folder")
            onTriggered: appui.browseToAppSettings()
        }

        MenuSeparator {} // Clean divider rule line separating file options from app close

        Action {

            text: qsTr("Quit without saving")
            onTriggered: {
                appui.disableAutoSave();
                window.close();
            }
        }
        Action {

            text: qsTr("Save and quit")
            onTriggered: window.close()
        }
    }

    Menu {
        delegate: MenuItem {
            id: menuItem2
            implicitHeight: 26

            contentItem: RowLayout {
                spacing: 20
                width: menuItem2.width - menuItem2.leftPadding - menuItem2.rightPadding

                // Left Text Component (Action name)
                Label {
                    text: menuItem2.text
                    color: menuItem2.highlighted ? window.palette.highlightedText : window.palette.text
                    Layout.fillWidth: true
                }

                // Right Text Component (The Shortcut description indicator)
                Label {
                    text: menuItem2.action && menuItem2.action.shortcut ? menuItem2.action.shortcut : ""
                    color: menuItem2.highlighted ? window.palette.highlightedText : window.palette.text
                    horizontalAlignment: Text.AlignRight
                }
            }
        }

        title: qsTr("Actions")

        Action {
            text: qsTr("Generate mod")
            shortcut: "F9"
            onTriggered: appui.generate()
        }
        Action {
            text: qsTr("New seed")
            shortcut: "F5"
            onTriggered: appui.newSeed()
        }
        Action {
            text: qsTr("Undo")
            shortcut: "Ctrl+Z"
            onTriggered: appui.makeUndo()
        }
        Menu {
            title: qsTr("Theme")

            Action {
                checkable: true
                checked: window.isDarkMode
                text: qsTr("Dark")
                onTriggered: {
                    window.isDarkMode = true;
                    appui.setApp("themeId", "dark");
                }
            }
            Action {
                checkable: true
                checked: !window.isDarkMode
                text: qsTr("Light")
                onTriggered: {
                    window.isDarkMode = false;
                    appui.setApp("themeId", "light");
                }
            }
        }

        Menu {
            title: qsTr("Language") + " (Language)"

            Action {
                checkable: true
                text: qsTr("English")
                onTriggered: appui.setApp("langId", "en_US")
            }
            Action {
                checkable: true
                text: qsTr("Russian")
                onTriggered: appui.setApp("langId", "ru_RU")
            }
            Action {
                checkable: true
                text: qsTr("Korean")
                onTriggered: appui.setApp("langId", "ko_KR")
            }
        }
    }

    Menu {
        title: qsTr("Help")
        Action {
            text: qsTr("About")
            onTriggered: Qt.openUrlExternally("https://github.com/mapron/d2modgen") // Opens native system browser
        }
    }
}
