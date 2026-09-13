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
            name: qsTr("Tool"),
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
            value_context: appModuleItemRandomizer,
            tabIndex: 8
        },
        {
            name: qsTr("Monster Randomizer"),
            isHeader: false,
            hasCheckbox: true,
            value_context: appModuleMonRandomizer,
            tabIndex: 10
        },
        {
            name: qsTr("Skill Randomizer"),
            isHeader: false,
            hasCheckbox: true,
            value_context: appModuleSkillRandomizer,
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
            value_context: appModuleMonStats,
            tabIndex: 11
        },
        {
            name: qsTr("Monster density"),
            isHeader: false,
            hasCheckbox: true,
            value_context: appModuleMonDensity,
            tabIndex: 9
        },
        {
            name: qsTr("Challenge"),
            isHeader: false,
            hasCheckbox: true,
            value_context: appModuleChallenge,
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
            value_context: appModuleCube,
            tabIndex: 4
        },
        {
            name: qsTr("Gambling"),
            isHeader: false,
            hasCheckbox: true,
            value_context: appModuleGambling,
            tabIndex: 6
        },
        {
            name: qsTr("Character"),
            isHeader: false,
            hasCheckbox: true,
            value_context: appModuleCharacter,
            tabIndex: 3
        },
        {
            name: qsTr("Increase Item Drops"),
            isHeader: false,
            hasCheckbox: true,
            value_context: appModuleItemDrops,
            tabIndex: 7
        },
        {
            name: qsTr("Increase Rune Drops"),
            isHeader: false,
            hasCheckbox: true,
            value_context: appModuleRuneDrops,
            tabIndex: 15
        },
        {
            name: qsTr("Perfect Rolls"),
            isHeader: false,
            hasCheckbox: true,
            value_context: appModulePerfectRoll,
            tabIndex: 12
        },
        {
            name: qsTr("Requirements"),
            isHeader: false,
            hasCheckbox: true,
            value_context: appModuleRequirements,
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
            value_context: appModuleQol,
            tabIndex: 13
        },
        {
            name: qsTr("Drops filtering"),
            isHeader: false,
            hasCheckbox: true,
            value_context: appModuleDropFiltering,
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

                    checked: modelData.hasCheckbox ? modelData.value_context.enabled : false
                    onClicked: modelData.value_context.enabled = checked
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
