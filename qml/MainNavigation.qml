/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
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
            name: qsTr("Item balance"),
            tabIndex: 2
        },
        {
            name: qsTr("Randomizers"),
            tabIndex: -1
        },
        {
            name: qsTr("Item Randomizer"),
            value_key: config.itemRandomizer.enabled,
            tabIndex: 4
        },
        {
            name: qsTr("Monster Randomizer"),
            value_key: config.monsterRandomizer.enabled,
            tabIndex: 5
        },
        {
            name: qsTr("Skill Randomizer"),
            value_key: config.skillRandomizer.enabled,
            tabIndex: 6
        },
        {
            name: qsTr("Increase drops"),
            tabIndex: -1
        },
        {
            name: qsTr("Items"),
            value_key: config.treasureClasses.items.enabled,
            tabIndex: 8
        },
        {
            name: qsTr("Runes"),
            value_key: config.treasureClasses.runes.enabled,
            tabIndex: 9
        },
        {
            name: qsTr("Make harder"),
            tabIndex: -1
        },
        {
            name: qsTr("Monster stats"),
            tabIndex: 11
        },
        {
            name: qsTr("Challenge"),
            tabIndex: 12
        },
        {
            name: qsTr("Misc"),
            tabIndex: -1
        },
        {
            name: qsTr("Horadric Cube"),
            tabIndex: 14
        },
        {
            name: qsTr("Gambling"),
            tabIndex: 15
        },
        {
            name: qsTr("Character"),
            tabIndex: 16
        },
        {
            name: qsTr("Quality of life"),
            tabIndex: 17
        },
    /*{
            name: qsTr("Large inventory"),
            tabIndex: 18
        },*/
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
                anchors.leftMargin: 65
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

                Item {
                    id: checkboxContainer

                    Layout.preferredWidth: 24
                    Layout.fillHeight: true

                    CheckBox {
                        id: tabCheckbox

                        visible: !!modelData.value_key

                        anchors.centerIn: parent

                        indicator.implicitWidth: 18
                        indicator.implicitHeight: 18

                        checked: modelData.value_key ? modelData.value_key.value : false
                        onClicked: modelData.value_key.value = checked
                    }
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
