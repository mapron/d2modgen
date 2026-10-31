import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

ColumnLayout {
    id: leftNavigationColumn

    property var navigationModel: [
        {
            name: qsTr("Main"),
            isHeader: false,
            hasCheckbox: false,
            tabIndex: 0
        },
        {
            name: qsTr("Extra"),
            isHeader: false,
            hasCheckbox: false,
            tabIndex: 1
        },
        {
            name: qsTr("Randomizers"),
            isHeader: true,
            hasCheckbox: false,
            tabIndex: -1
        },
        {
            name: qsTr("Item Randomizer"),
            isHeader: false,
            hasCheckbox: true,
            value_key: config.enabledModules.moduleItemRandomizer,
            tabIndex: 8
        },
        {
            name: qsTr("Monster Randomizer"),
            isHeader: false,
            hasCheckbox: true,
            value_key: config.enabledModules.moduleMonRandomizer,
            tabIndex: 10
        },
        {
            name: qsTr("Skill Randomizer"),
            isHeader: false,
            hasCheckbox: true,
            value_key: config.enabledModules.moduleSkillRandomizer,
            tabIndex: 16
        },
        {
            name: qsTr("Make harder"),
            isHeader: true,
            hasCheckbox: false,
            tabIndex: -1
        },
        {
            name: qsTr("Monster stats"),
            isHeader: false,
            hasCheckbox: true,
            value_key: config.enabledModules.moduleMonStats,
            tabIndex: 11
        },
        {
            name: qsTr("Monster density"),
            isHeader: false,
            hasCheckbox: true,
            value_key: config.enabledModules.moduleMonDensity,
            tabIndex: 9
        },
        {
            name: qsTr("Challenge"),
            isHeader: false,
            hasCheckbox: true,
            value_key: config.enabledModules.moduleChallenge,
            tabIndex: 2
        },
        {
            name: qsTr("Make easier"),
            isHeader: true,
            hasCheckbox: false,
            tabIndex: -1
        },
        {
            name: qsTr("Horadric Cube"),
            isHeader: false,
            hasCheckbox: true,
            value_key: config.enabledModules.moduleCube,
            tabIndex: 4
        },
        {
            name: qsTr("Gambling"),
            isHeader: false,
            hasCheckbox: true,
            value_key: config.enabledModules.moduleGambling,
            tabIndex: 6
        },
        {
            name: qsTr("Character"),
            isHeader: false,
            hasCheckbox: true,
            value_key: config.enabledModules.moduleCharacter,
            tabIndex: 3
        },
        {
            name: qsTr("Increase Item Drops"),
            isHeader: false,
            hasCheckbox: true,
            value_key: config.enabledModules.moduleItemDrops,
            tabIndex: 7
        },
        {
            name: qsTr("Increase Rune Drops"),
            isHeader: false,
            hasCheckbox: true,
            value_key: config.enabledModules.moduleRuneDrops,
            tabIndex: 15
        },
        {
            name: qsTr("Perfect Rolls"),
            isHeader: false,
            hasCheckbox: true,
            value_key: config.enabledModules.modulePerfectRoll,
            tabIndex: 12
        },
        {
            name: qsTr("Requirements"),
            isHeader: false,
            hasCheckbox: true,
            value_key: config.enabledModules.moduleRequirements,
            tabIndex: 14
        },
        {
            name: qsTr("Misc"),
            isHeader: true,
            hasCheckbox: false,
            tabIndex: -1
        },
        {
            name: qsTr("Quality of life"),
            isHeader: false,
            hasCheckbox: true,
            value_key: config.enabledModules.moduleQol,
            tabIndex: 13
        },
        {
            name: qsTr("Drops filtering"),
            isHeader: false,
            hasCheckbox: true,
            value_key: config.enabledModules.moduleDropFiltering,
            tabIndex: 5
        },
    ]
    // Block horizontal growth, keeping this sidebar strictly predictable
    Layout.fillWidth: false
    Layout.preferredWidth: 200
    Layout.fillHeight: true

    // Enforce minimum height so navigation controls never crash together
    Layout.minimumHeight: 280
    spacing: 10

    property alias activeTabIndex: tabSelection.activeTabIndex

    ListView {
        id: tabSelection
        Layout.fillWidth: true
        Layout.fillHeight: true
        // Track the active StackLayout index separately from the ListView's raw row index
        property int activeTabIndex: 0

        model: navigationModel
        clip: true

        delegate: Item {
            width: tabSelection.width
            height: modelData.isHeader ? 22 : 28

            // --- 1. GROUP HEADER BANNER ---
            Label {
                text: modelData.name
                visible: modelData.isHeader
                anchors.left: parent.left
                anchors.leftMargin: 45
                anchors.verticalCenter: parent.verticalCenter
                font.bold: true
                color: window.palette.text
            }

            // --- 2. THE SIMPLE SINGLE-CLICK BUTTON FRAME ---
            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 0
                anchors.rightMargin: 0
                spacing: 6

                visible: !modelData.isHeader

                CheckBox {
                    id: tabCheckbox
                    visible: modelData.hasCheckbox
                    indicator.implicitWidth: 18
                    indicator.implicitHeight: 18

                    checked: modelData.hasCheckbox ? modelData.value_key.value : false
                    onClicked: modelData.value_key.value = checked
                }

                Button {
                    text: modelData.name
                    Layout.fillWidth: true

                    onClicked: tabSelection.activeTabIndex = modelData.tabIndex

                    palette.button: (tabSelection.activeTabIndex === modelData.tabIndex) ? window.palette.highlight : window.palette.button
                }
            }
        }
    }
}
