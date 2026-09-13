import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

ApplicationWindow {
    id: window
    visible: true
    title: qsTr("D2 mod generator by mapron - 0.7.0")

    menuBar: MainMenuBar {}

    property bool isDarkMode: appui.getApp("themeId", "dark") === "dark"

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
                    ModuleChallenge {}
                    ModuleCharacter {}
                    ModuleCube {}
                    ModuleDropFiltering {}
                    ModuleGambling {}
                    ModuleItemDrops {}
                    ModuleItemRandomizer {}
                    ModuleMonDensity {}
                    ModuleMonRandomizer {}
                    ModuleMonStats {}
                    ModulePerfectRoll {}
                    ModuleQol {}
                    ModuleRequirements {}
                    ModuleRuneDrops {}
                    ModuleSkillRandomizer {}
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 40 // Fixed height for the bottom bar
            spacing: 15

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
                }
            }

            Button {
                id: generateButton
                text: qsTr("Generate")
                font.pointSize: 13

                onClicked: appui.generate()
            }
        }
    }
}
