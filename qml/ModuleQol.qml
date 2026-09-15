import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick

WidgetCommonTab {
    id: root
    value_context: appModuleQol
    caption: qsTr("Quality of life")
    tooltip: qsTr("Tiny quality things to make you life easier: <br>1. Insreasing stacks for tomes/quivers/keys;<br>2. Remove Unique charm limit (useful with randomizer);<br>3. Allow usage of some skills in the town;<br>4. Reduce item costs. ")

    RowLayout {
        ColumnLayout {
            WidgetCheckbox {
                caption: qsTr("Increase tome sizes 20 -> 60")
                tooltip: ""
                value_context: root.value_context
                value_key: "tomeSize"
            }

            WidgetCheckbox {
                caption: qsTr("Increase key chain 12 -> 50")
                tooltip: ""
                value_context: root.value_context
                value_key: "keySize"
            }

            WidgetCheckbox {
                caption: qsTr("Increase quivers size -> 511")
                tooltip: qsTr("D2R already have 500 quiver size, so use this for LoD")
                value_context: root.value_context
                value_key: "quiverSize"
            }

            WidgetCheckbox {
                caption: qsTr("Remove limit on Unique charms")
                tooltip: ""
                value_context: root.value_context
                value_key: "uniqueCharmLimit"
            }
        }
        ColumnLayout {
            WidgetCheckbox {
                caption: qsTr("Allow using Teleport, BC, BO in town")
                tooltip: ""
                value_context: root.value_context
                value_key: "weakenTownSkills"
            }

            WidgetCheckbox {
                caption: qsTr("Allow all skills in town")
                tooltip: ""
                value_context: root.value_context
                value_key: "weakenTownSkillsExt"
            }

            WidgetCheckbox {
                caption: qsTr("Show ilevel in items' titles")
                tooltip: ""
                value_context: root.value_context
                value_key: "showItemLevel"
            }

            WidgetCheckbox {
                caption: qsTr("Disable experience penalty after level 70")
                tooltip: qsTr("That means you won't have around 1% EXP at higher levels, so high level grind will be much faster.")
                value_context: root.value_context
                value_key: "disableExpPenalty"
            }
        }
    }

    WidgetRangeSlider {
        caption: qsTr("Reduce costs of skills and stats on items (affects repair cost mostly)")
        tooltip: ""
        from: 5
        to: 100
        value_context: root.value_context
        value_key: "reduceCost"
        isCompact: false
    }
}
