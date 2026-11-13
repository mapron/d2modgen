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
            name: qsTr("Main")
        },
        {
            name: qsTr("Extra")
        },
        {
            name: qsTr("Randomizers"),
            isHeader: true
        },
        {
            name: qsTr("Items"),
            value_key: config.itemRandomizer.enabled
        },
        {
            name: qsTr("Monsters"),
            value_key: config.monsterRandomizer.enabled
        },
        {
            name: qsTr("Skills"),
            value_key: config.skillRandomizer.enabled
        },
        {
            name: qsTr("Balance and stats"),
            isHeader: true
        },
        {
            name: qsTr("Items")
        },
        {
            name: qsTr("Monsters")
        },
        {
            name: qsTr("Character")
        },
        {
            name: qsTr("Skills")
        },
        {
            name: qsTr("Areas / density")
        },
        {
            name: qsTr("Gambling")
        },
        {
            name: qsTr("Drop rates"),
            isHeader: true
        },
        {
            name: qsTr("Items"),
            value_key: config.droprate.items.enabled
        },
        {
            name: qsTr("Runes"),
            value_key: config.droprate.runes.enabled
        },
        {
            name: qsTr("Misc"),
            value_key: config.droprate.misc.enabled
        },
        {
            name: qsTr("Other"),
            isHeader: true
        },
        {
            name: qsTr("Horadric Cube")
        },
        {
            name: qsTr("Quality of life")
        },
        {
            name: qsTr("Large inventory")
        }
    ]
    // Block horizontal growth, keeping this sidebar strictly predictable
    Layout.fillWidth: false
    Layout.preferredWidth: 200
    Layout.fillHeight: true

    spacing: 10

    property alias activeTabIndex: tabSelection.activeTabIndex

    ListView {
        id: tabSelection
        Layout.fillWidth: true
        Layout.fillHeight: true
        property int activeTabIndex: 0

        model: navigationModel
        clip: true

        delegate: Item {
            id: delegateRoot
            width: tabSelection.width

            readonly property bool isSectionHeader: !!modelData.isHeader
            readonly property int rowIndex: index

            height: isSectionHeader ? 22 : 28

            Label {
                text: modelData.name
                visible: delegateRoot.isSectionHeader
                anchors.left: parent.left
                anchors.leftMargin: 65
                anchors.verticalCenter: parent.verticalCenter
                font.bold: true
                color: window.palette.text
            }

            RowLayout {
                anchors.fill: parent
                spacing: 6
                visible: !delegateRoot.isSectionHeader

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
                    onClicked: tabSelection.activeTabIndex = delegateRoot.rowIndex
                    palette.button: (tabSelection.activeTabIndex === delegateRoot.rowIndex) ? window.palette.highlight : window.palette.button
                }
            }
        }
    }
}
