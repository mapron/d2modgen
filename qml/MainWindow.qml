/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion
import "Modules"
import "Widgets"

ApplicationWindow {
    id: window
    visible: true
    title: qsTr("D2 mod generator by mapron")

    menuBar: MainMenuBar {}

    property bool isDarkMode: appConfig.themeId.value === "dark"

    palette: isDarkMode ? darkPalette : lightPalette

    Palette {
        id: lightPalette

        window: "#f0f0f0"
        windowText: "#000000"
        base: "#ffffff"
        text: "#000000"
        button: "#e1e1e1"
        buttonText: "#000000"
        highlight: "#308cc6"

        link: "#308cc6"

        // Colors applied only when an item/window is disabled
        disabled {
            windowText: "#a0a0a0"
            text: "#a0a0a0"
            button: "#e1e1e1"
            buttonText: "#a0a0a0"
            highlight: "#a0a0a0" // Grays out the slider track/handle progress
        }
    }

    Palette {
        id: darkPalette

        window: "#31363B"
        windowText: "#ffffff"
        base: "#191919"
        text: "#ffffff"
        button: "#33383D"
        buttonText: "#ffffff"
        highlight: "#2a82da"

        link: "#4a92ea"

        // Colors applied only when an item/window is disabled
        disabled {
            windowText: "#666666"
            text: "#666666"
            button: "#353535"
            buttonText: "#666666"
            highlight: "#555555"
        }
    }

    minimumWidth: 900
    minimumHeight: 700

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true // Tells it to take up all remaining vertical space
            spacing: 5

            MainNavigation {
                id: tabSelection
            }
            Frame {
                id: stackContainer
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumWidth: 420

                // Customizing the frame to have your accent-colored border
                background: Rectangle {
                    color: "transparent" // Keeps the internal background transparent
                    border.color: window.palette.highlight
                    border.width: 1
                }
                StackLayout {
                    id: mainStack

                    anchors.fill: parent // Fills the inside of the frame
                    currentIndex: tabSelection.activeTabIndex

                    PageMain {}
                    PageTool {}
                    ModuleItemBalance {}

                    Item {}
                    ModuleItemRandomizer {}
                    ModuleMonRandomizer {}
                    ModuleSkillRandomizer {}

                    Item {}
                    ModuleItemDrops {}
                    ModuleRuneDrops {}

                    Item {}
                    ModuleMonStats {}
                    ModuleChallenge {}
                    ModuleMaps {}

                    Item {}
                    ModuleCube {}
                    ModuleGambling {}
                    ModuleCharacter {}
                    ModuleQol {}
                    ModuleExpandedInventory {}
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 40 // Fixed height for the bottom bar
            spacing: 15

            Button {
                text: qsTr("Check app updates")

                onClicked: versionChecker.checkForUpdates()
            }

            Label {
                text: versionChecker.versionText
                verticalAlignment: Text.AlignVCenter
                color: window.palette.windowText

                onLinkActivated: link => Qt.openUrlExternally(link)
            }

            Item {
                Layout.fillWidth: true
            }

            Label {
                id: statusLabel
                text: qsTr("Status label: Ready")
                verticalAlignment: Text.AlignVCenter
                color: window.palette.windowText

                Connections {
                    target: appui
                    function onStatusUpdate(status) {
                        statusLabel.text = Qt.formatDateTime(new Date(), "[HH:mm:ss] ") + status;
                    }
                    function onShowErrorDialog(mainLine, extraInfo) {
                        errorDialog.mainText = mainLine;
                        errorDialog.detailsText = extraInfo;
                        errorDialog.open();
                    }
                }
            }

            Button {
                text: qsTr("Generate")
                Component.onCompleted: font.pointSize = font.pointSize * 1.2

                onClicked: appui.generate()
            }
        }
    }

    Dialog {
        id: errorDialog
        title: qsTr("Error on Generate")
        modal: true
        anchors.centerIn: parent
        standardButtons: Dialog.Ok
        header: LabelImportant {
            text: errorDialog.title
        }

        width: Math.min(parent.width - 40, 450)

        property string mainText: ""
        property string detailsText: ""

        contentItem: ColumnLayout {
            spacing: 10

            LabelLarge {
                text: errorDialog.mainText
                Layout.fillWidth: true
                wrapMode: Text.Wrap
            }

            ScrollView {
                id: scrollView
                Layout.fillWidth: true
                // Automatically sizes to text up to 200px (~8-10 lines).
                // Below 200px, no scrollbar appears. Above 200px, it scrolls.
                Layout.preferredHeight: Math.min(errorLabel.implicitHeight, 200)
                clip: true

                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

                TextArea {
                    id: errorLabel
                    text: errorDialog.detailsText
                    readOnly: true
                    wrapMode: TextEdit.Wrap
                    selectByMouse: true

                    color: palette.text

                    background: Rectangle {
                        color: palette.base
                        radius: 4

                        border.color: palette.mid
                        border.width: 1
                    }
                }
            }
        }
    }
}
