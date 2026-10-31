import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

ColumnLayout {
    id: leftNavigationColumn

    property var navigationModel: [
        {
            name: qsTr("Main"),
            tabIndex: 0
        },
        {
            name: qsTr("Extra"),
            tabIndex: 1
        },
        {
            name: qsTr("Randomizers"),
            tabIndex: -1
        },
        {
            name: qsTr("Item Randomizer"),
            value_key: config.moduleItemRandomizer.enabled,
            tabIndex: 3
        },
        {
            name: qsTr("Monster Randomizer"),
            value_key: config.moduleMonRandomizer.enabled,
            tabIndex: 4
        },
        {
            name: qsTr("Skill Randomizer"),
            value_key: config.moduleSkillRandomizer.enabled,
            tabIndex: 5
        },
        {
            name: qsTr("Make harder"),
            tabIndex: -1
        },
        {
            name: qsTr("Monster stats"),
            tabIndex: 7
        },
        {
            name: qsTr("Monster density"),
            tabIndex: 8
        },
        {
            name: qsTr("Challenge"),
            value_key: config.moduleChallenge.enabled,
            tabIndex: 9
        },
        {
            name: qsTr("Make easier"),
            tabIndex: -1
        },
        {
            name: qsTr("Horadric Cube"),
            tabIndex: 11
        },
        {
            name: qsTr("Gambling"),
            tabIndex: 12
        },
        {
            name: qsTr("Character"),
            value_key: config.moduleCharacter.enabled,
            tabIndex: 13
        },
        {
            name: qsTr("Increase Item Drops"),
            value_key: config.treasureClasses.items.enabled,
            tabIndex: 14
        },
        {
            name: qsTr("Increase Rune Drops"),
            value_key: config.treasureClasses.runes.enabled,
            tabIndex: 15
        },
        {
            name: qsTr("Perfect Rolls"),
            tabIndex: 16
        },
        {
            name: qsTr("Requirements"),
            tabIndex: 17
        },
        {
            name: qsTr("Misc"),
            tabIndex: -1
        },
        {
            name: qsTr("Quality of life"),
            tabIndex: 19
        },
        {
            name: qsTr("Drops filtering"),
            value_key: config.moduleDropFiltering.enabled,
            tabIndex: 20
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
            height: modelData.tabIndex === -1 ? 22 : 28

            Label {
                text: modelData.name
                visible: modelData.tabIndex === -1
                anchors.left: parent.left
                anchors.leftMargin: 45
                anchors.verticalCenter: parent.verticalCenter
                font.bold: true
                color: window.palette.text
            }

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 0
                anchors.rightMargin: 0
                spacing: 6

                visible: modelData.tabIndex >= 0

                CheckBox {
                    id: tabCheckbox
                    visible: !!modelData.value_key
                    indicator.implicitWidth: 18
                    indicator.implicitHeight: 18

                    checked: modelData.value_key ? modelData.value_key.value : false
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
