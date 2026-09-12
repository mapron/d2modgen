import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

ApplicationWindow {
    id: window
    visible: true
    title: qsTr("D2 mod generator")

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

        window: "#353535"
        windowText: "#ffffff"
        base: "#191919"
        text: "#ffffff"
        button: "#353535"
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

    minimumWidth: 800
    minimumHeight: 700

    RowLayout {
        id: mainLayout
        anchors.fill: parent
        anchors.margins: 10
        spacing: 15

        MainNavigation {
            id: tabSelection
        }
        StackLayout {
            id: mainStack
            Layout.fillWidth: true
            Layout.minimumWidth: 420 // Prevents the text strings from clipping out horizontally
            Layout.fillHeight: true
            currentIndex: tabSelection.activeTabIndex

            PageMain {}
            PageTool {}
            TabChallenge {}
            TabCharacter {}
            TabCube {}
            TabDropFiltering {}
            TabGambling {}
            TabItemDrops {}
            TabItemRandomizer {}
            TabMonDensity {}
            TabMonRandomizer {}
            TabMonStats {}
            TabPerfectRoll {}
            TabQol {}
            TabRequirements {}
            TabRuneDrops {}
            TabSkillRandomizer {}
        }
    }
}
