import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

ColumnLayout {
    id: leftNavigationColumn

    ListModel {
        id: navigationModel

        ListElement {
            name: qsTr("Main")
            isHeader: false
            hasCheckbox: false
            tabIndex: 0
        }
        ListElement {
            name: qsTr("Tool")
            isHeader: false
            hasCheckbox: false
            tabIndex: 1
        }

        ListElement {
            name: qsTr("Randomizers")
            isHeader: true
            hasCheckbox: false
            tabIndex: -1
        }
        ListElement {
            name: qsTr("Item Randomizer")
            isHeader: false
            hasCheckbox: true
            checkedKey: "randomizer"
            tabIndex: 8
        }
        ListElement {
            name: qsTr("Monster Randomizer")
            isHeader: false
            hasCheckbox: true
            checkedKey: "monRandomizer"
            tabIndex: 10
        }
        ListElement {
            name: qsTr("Skill Randomizer")
            isHeader: false
            hasCheckbox: true
            checkedKey: "skillRandomizer"
            tabIndex: 16
        }

        ListElement {
            name: qsTr("Make harder")
            isHeader: true
            hasCheckbox: false
            tabIndex: -1
        }
        ListElement {
            name: qsTr("Monster stats")
            isHeader: false
            hasCheckbox: true
            checkedKey: "monster_stats"
            tabIndex: 11
        }
        ListElement {
            name: qsTr("Monster density")
            isHeader: false
            hasCheckbox: true
            checkedKey: "density"
            tabIndex: 9
        }
        ListElement {
            name: qsTr("Challenge")
            isHeader: false
            hasCheckbox: true
            checkedKey: "challenge"
            tabIndex: 2
        }

        ListElement {
            name: qsTr("Make easier")
            isHeader: true
            hasCheckbox: false
            tabIndex: -1
        }
        ListElement {
            name: qsTr("Horadric Cube")
            isHeader: false
            hasCheckbox: true
            checkedKey: "cube"
            tabIndex: 4
        }
        ListElement {
            name: qsTr("Gambling")
            isHeader: false
            hasCheckbox: true
            checkedKey: "gambling"
            tabIndex: 6
        }
        ListElement {
            name: qsTr("Character")
            isHeader: false
            hasCheckbox: true
            checkedKey: "character"
            tabIndex: 3
        }
        ListElement {
            name: qsTr("Increase Item Drops")
            isHeader: false
            hasCheckbox: true
            checkedKey: "item_drops"
            tabIndex: 7
        }
        ListElement {
            name: qsTr("Increase Rune Drops")
            isHeader: false
            hasCheckbox: true
            checkedKey: "rune_drops"
            tabIndex: 15
        }
        ListElement {
            name: qsTr("Perfect Rolls")
            isHeader: false
            hasCheckbox: true
            checkedKey: "perfect_rolls"
            tabIndex: 12
        }
        ListElement {
            name: qsTr("Requirements")
            isHeader: false
            hasCheckbox: true
            checkedKey: "requirements"
            tabIndex: 14
        }

        ListElement {
            name: qsTr("Misc")
            isHeader: true
            hasCheckbox: false
            tabIndex: -1
        }
        ListElement {
            name: qsTr("Quality of life")
            isHeader: false
            hasCheckbox: true
            checkedKey: "qol"
            tabIndex: 13
        }
        ListElement {
            name: qsTr("Drops filtering")
            isHeader: false
            hasCheckbox: true
            checkedKey: "drop_filter"
            tabIndex: 5
        }
    }
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
        model: navigationModel
        clip: true
        spacing: 1 // Tiny gap between items for clean alignment

        // Track the active StackLayout index separately from the ListView's raw row index
        property int activeTabIndex: 0

        delegate: Item {
            width: tabSelection.width
            height: isHeader ? 22 : 28

            // --- 1. GROUP HEADER BANNER ---
            Label {
                text: name
                visible: isHeader
                anchors.left: parent.left
                anchors.leftMargin: 6
                anchors.verticalCenter: parent.verticalCenter
                font.bold: true
                color: window.palette.text
            }

            // --- 2. THE SIMPLE SINGLE-CLICK BUTTON FRAME ---
            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 6
                anchors.rightMargin: 6
                spacing: 8

                visible: !isHeader

                CheckBox {
                    id: tabCheckbox
                    visible: hasCheckbox
                    indicator.implicitWidth: 18
                    indicator.implicitHeight: 18

                    checked: hasCheckbox ? appui.getEnabled(checkedKey) : false

                    Connections {
                        target: appui
                        function onDataChanged() {
                            if (hasCheckbox)
                                tabCheckbox.checked = appui.getEnabled(checkedKey);
                        }
                    }

                    onClicked: {
                        appui.setEnabled(checkedKey, checked);
                    }
                }

                Button {
                    text: name
                    Layout.fillWidth: true
                    onClicked: {
                        tabSelection.activeTabIndex = tabIndex;
                    }

                    palette.button: (tabSelection.activeTabIndex === tabIndex) ? window.palette.highlight : window.palette.button
                }
            }
        }
    }
}
