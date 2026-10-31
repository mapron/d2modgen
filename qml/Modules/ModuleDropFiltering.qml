import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick
import "../Widgets"

WidgetCommonTab {
    id: root
    value_context: config.moduleDropFiltering
    enabled_key: config.enabledModules.moduleDropFiltering
    caption: qsTr("Drops filtering")
    tooltip: qsTr("This tab consists of two sections:<br>1. Make some items have compact names, like &quot;!MP5&quot; for Super Mana Potion. <br>2. Hide item labels on the ground (you still be able to pick them). ")

    Label {
        text: qsTr("<b>Make item names compact</b>: this will make item names take less space, e.g. '!HP2' for health potion.")
    }

    WidgetCheckbox {
        caption: qsTr("Compact potion names")
        tooltip: ""
        value_key: root.value_context.compact_pots
    }

    WidgetCheckbox {
        caption: qsTr("Compact TP/ID scrolls")
        tooltip: ""
        value_key: root.value_context.compact_scrolls
    }

    Label {
        text: qsTr("<b>Hide items on the ground</b>: this will make item names transparent; <br>you still can pickup them, but their labels will be invisible on Alt press.") + qsTr("<br><b>With D2R: RotW - just use game loot filter! You don't need those anymore!<b>")
    }

    RowLayout {
        ColumnLayout {

            WidgetCheckbox {
                caption: qsTr("ID scroll")
                tooltip: ""
                        value_key: root.value_context.hide_isc
            }

            WidgetCheckbox {
                caption: qsTr("TP scroll")
                tooltip: ""
                        value_key: root.value_context.hide_tsc
            }

            WidgetCheckbox {
                caption: qsTr("Health pots 1-3")
                tooltip: ""
                        value_key: root.value_context.hide_hps
            }

            WidgetCheckbox {
                caption: qsTr("All Health pots")
                tooltip: ""
                        value_key: root.value_context.hide_hpsa
            }

            WidgetCheckbox {
                caption: qsTr("Mana pots 1-3")
                tooltip: ""
                        value_key: root.value_context.hide_mps
            }

            WidgetCheckbox {
                caption: qsTr("All Mana pots")
                tooltip: ""
                        value_key: root.value_context.hide_mpsa
            }
        }
        ColumnLayout {
            WidgetCheckbox {
                caption: qsTr("Rejuv pots")
                tooltip: ""
                        value_key: root.value_context.hide_rvs
            }

            WidgetCheckbox {
                caption: qsTr("Full Rejuv pots")
                tooltip: ""
                        value_key: root.value_context.hide_rvl
            }
            WidgetCheckbox {
                caption: qsTr("Bolts/Arrows")
                tooltip: ""
                        value_key: root.value_context.hide_ammo
            }

            WidgetCheckbox {
                caption: qsTr("Stamina/Antidote/Thawing")
                tooltip: ""
                        value_key: root.value_context.hide_stam
            }

            WidgetCheckbox {
                caption: qsTr("Keys,Fire/Poison pots")
                tooltip: ""
                        value_key: root.value_context.hide_junks
            }

            WidgetCheckbox {
                caption: qsTr("Hide low quality/damaged/cracked items")
                tooltip: ""
                        value_key: root.value_context.hide_lowq
            }
        }
    }
}
