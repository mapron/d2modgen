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
    }

    minimumWidth: 800
    minimumHeight: 700

    RowLayout {
        id: mainLayout
        anchors.fill: parent
        anchors.margins: 10
        spacing: 15

        Navigation {
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
