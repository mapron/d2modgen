import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

MenuBar {
    id: mainMenuBar

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
                    font.pixelSize: 11
                    horizontalAlignment: Text.AlignRight
                }
            }
        }

        title: qsTr("File") // The '&' enables standard Alt+F keyboard navigation mnemonics

        Action {
            text: qsTr("Save config...")
            shortcut: "Ctrl+S"
            onTriggered: console.log("sabve")
        }
        Action {
            text: qsTr("Load config...")
            shortcut: "Ctrl+O"
            onTriggered: console.log("Open e")
        }
        Action {
            text: qsTr("Clear config")
            shortcut: "Ctrl+N"
            onTriggered: console.log("clear")
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
            onTriggered: console.log("Browse")
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
        delegate: mainMenuFile.delegate

        title: qsTr("Actions")

        Action {
            text: qsTr("Generate mod")
            shortcut: "F9"
            onTriggered: appui.generate()
        }
        Action {
            text: qsTr("New seed")
            shortcut: "F5"
            onTriggered: console.log("New seed")
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
