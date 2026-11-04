/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick
import "../Widgets"

Frame {
    id: root

    property QtObject value_context: null
    property string caption: ""
    property bool isDefault: false
    property bool isEquipment: false

    Layout.fillWidth: true
    Layout.fillHeight: true

    background: Rectangle {
        color: "transparent"
        border.color: window.palette.highlight
        border.width: 1
    }

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            id: pageHeaderRow
            spacing: 6

            Label {
                text: root.caption
                font.pointSize: 11
            }

            Item {
                Layout.fillWidth: true
            }

            CheckBox {
                id: headerCheckbox
                visible: !root.isDefault

                indicator.implicitWidth: 18
                indicator.implicitHeight: 18

                checked: root.value_context.doOverride.value
                text: qsTr("Override")

                onClicked: root.value_context.doOverride.value = checked
            }

            Button {
                text: qsTr("Clear")
                onClicked: value_context.resetToZero()
            }
            Button {
                text: qsTr("Recommended")
                onClicked: {
                    let doOverride = root.value_context.doOverride.value;
                    value_context.resetToRecommended();
                    root.value_context.doOverride.value = doOverride;
                }
            }
            Button {
                visible: !root.isDefault
                text: qsTr("Sync from common")
                onClicked: {
                    let doOverride = root.value_context.doOverride.value;
                    value_context.assign(config.itemBalance.defaults);
                    root.value_context.doOverride.value = doOverride;
                }
            }
        }

        ColumnLayout {
            id: main
            visible: root.isDefault || headerCheckbox.checked

            Separator {}

            RowLayout {
                spacing: 10
                Layout.fillWidth: true
                Label {
                    text: qsTr("Min/max roll mode:")
                }
                RadioGroup {
                    model: [qsTr("Keep random"), qsTr("Anti-perfect"), qsTr("Average"), qsTr("Perfect max")]
                    value_key: root.value_context.rollMode
                }
                Item {
                    Layout.fillWidth: true
                }
            }

            WidgetSlider {
                caption: qsTr("Level 1 power multiply")
                tooltip: ""
                value_key: root.value_context.powerScaleLevel1
            }

            WidgetSlider {
                caption: qsTr("Level 50 (or common) power multiply")
                tooltip: ""
                value_key: root.value_context.powerScaleLevel50
            }

            WidgetSlider {
                caption: qsTr("Original properties to keep [rando]")
                tooltip: qsTr("You can select how many properties of original item you want to keep.<br>For 0%, no original property is kept<br>For 50% - random half of original properties is retained;<br>For 100%, all original properties remain and you can add more with next two sliders to generate on top.")
                value_key: root.value_context.randoKeepOriginalPercent
                visible: !isEquipment
            }

            WidgetRangeSlider {
                caption: qsTr("Generated properties [rando]")
                tooltip: qsTr("This determine extra properties added aside those kept.<br>0% means no properties generated (you still can re-roll existing ones)<br>For min=50% and max= 200%, item will generate from half to twice original count as extra properties.<br>Note: each item type has its own hard limit that you can not exceed, e.g. magic affix can have up to 3 properties.")
                value_key1: root.value_context.randoGenerateMin
                value_key2: root.value_context.randoGenerateMax
                visible: !isEquipment
            }

            WidgetSlider {
                caption: qsTr("Change level requirement (lvl_req)")
                tooltip: ""
                value_key: root.value_context.reqLevel
            }

            WidgetSlider {
                caption: qsTr("Change item quality level (ilvl)")
                tooltip: qsTr("That controls how valueable item is from drop calculation process.<br> Lower value means you get item dropped much earlier in the game.")
                value_key: root.value_context.itemLevel
            }
        }
    }
}
