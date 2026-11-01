import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick
import "../Widgets"

WidgetCommonTab {
    id: root
    value_context: config.qualityOfLife
    enabled_key: null
    caption: qsTr("Quality of life")
    tooltip: qsTr("Tiny quality things to make you life easier: <br>1. Insreasing stacks for tomes/quivers/keys;<br>2. Remove Unique charm limit (useful with randomizer);<br>3. Allow usage of some skills in the town;<br>4. Reduce item costs. ")

    RowLayout {
        ColumnLayout {
            WidgetCheckbox {
                caption: qsTr("Show ilevel in items' titles")
                tooltip: ""
                value_key: root.value_context.showItemLevel
            }

            WidgetCheckbox {
                caption: qsTr("Disable experience penalty after level 70")
                tooltip: qsTr("That means you won't have around 1% EXP at higher levels, so high level grind will be much faster.")
                value_key: root.value_context.disableExpPenalty
            }
            WidgetCheckbox {
                caption: qsTr("Remove limit on Unique charms")
                tooltip: ""
                value_key: root.value_context.uniqueCharmLimit
            }
        }
        ColumnLayout {
            WidgetCheckbox {
                caption: qsTr("Allow using Teleport, BC, BO in town")
                tooltip: ""
                value_key: root.value_context.weakenTownSkills
            }

            WidgetCheckbox {
                caption: qsTr("Allow all skills in town")
                tooltip: ""
                value_key: root.value_context.weakenTownSkillsExt
            }
        }
    }

    WidgetSlider {
        caption: qsTr("Tomes (ID/TP) size")
        tooltip: ""
        value_key: root.value_context.tomeSize
        isCompact: true
    }

    WidgetSlider {
        caption: qsTr("Key chain size")
        tooltip: ""
        value_key: root.value_context.keySize
        isCompact: true
    }

    WidgetSlider {
        caption: qsTr("Quivers size")
        tooltip: qsTr("D2R already have 500 quiver size, and PD2 have unlimited, so use this for LoD")
        value_key: root.value_context.quiverSize
        isCompact: true
    }

    WidgetSlider {
        caption: qsTr("Reduce costs of skills and stats on items (affects repair cost mostly)")
        tooltip: ""
        value_key: root.value_context.reduceCost
        isCompact: false
    }

    Label {
        text: qsTr("<b>Make item names compact</b>: this will make item names take less space, e.g. '!HP2' for health potion.<br>Have only effect for D2R")
    }

    WidgetCheckbox {
        caption: qsTr("Compact potion names")
        tooltip: ""
        value_key: root.value_context.compactPots
    }

    WidgetCheckbox {
        caption: qsTr("Compact TP/ID scrolls")
        tooltip: ""
        value_key: root.value_context.compactScrolls
    }
}
