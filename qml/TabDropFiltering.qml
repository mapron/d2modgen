import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick

AppCommonTab {
    id: root
    value_context: "drop_filter"
    caption: qsTr("Drops filtering")
    tooltip: qsTr("This tab consists of two sections:<br>1. Make some items have compact names, like &quot;!MP5&quot; for Super Mana Potion. <br>2. Hide item labels on the ground (you still be able to pick them). ")

    Label {
        text: qsTr("<b>Make item names compact</b>: this will make item names take less space, e.g. '!HP2' for health potion.")
    }

    AppCheckbox {
        caption: qsTr("Compact potion names")
        tooltip: ""
        value_context: root.value_context
        value_key: "compact_pots"
    }

    AppCheckbox {
        caption: qsTr("Compact TP/ID scrolls")
        tooltip: ""
        value_context: root.value_context
        value_key: "compact_scrolls"
    }

    Label {
        text: qsTr("<b>Hide items on the ground</b>: this will make item names transparent; <br>you still can pickup them, but their labels will be invisible on Alt press.")
    }

    AppCheckbox {
        caption: qsTr("ID scroll")
        tooltip: ""
        value_context: root.value_context
        value_key: "hide_isc"
    }

    AppCheckbox {
        caption: qsTr("TP scroll")
        tooltip: ""
        value_context: root.value_context
        value_key: "hide_tsc"
    }

    AppCheckbox {
        caption: qsTr("Health pots 1-3")
        tooltip: ""
        value_context: root.value_context
        value_key: "hide_hps"
    }

    AppCheckbox {
        caption: qsTr("All Health pots")
        tooltip: ""
        value_context: root.value_context
        value_key: "hide_hpsa"
    }

    AppCheckbox {
        caption: qsTr("Mana pots 1-3")
        tooltip: ""
        value_context: root.value_context
        value_key: "hide_mps"
    }

    AppCheckbox {
        caption: qsTr("All Mana pots")
        tooltip: ""
        value_context: root.value_context
        value_key: "hide_mpsa"
    }

    AppCheckbox {
        caption: qsTr("Rejuv pots")
        tooltip: ""
        value_context: root.value_context
        value_key: "hide_rvs"
    }

    AppCheckbox {
        caption: qsTr("Full Rejuv pots")
        tooltip: ""
        value_context: root.value_context
        value_key: "hide_rvl"
    }

    AppCheckbox {
        caption: qsTr("Bolts/Arrows")
        tooltip: ""
        value_context: root.value_context
        value_key: "hide_ammo"
    }

    AppCheckbox {
        caption: qsTr("Stamina/Antidote/Thawing")
        tooltip: ""
        value_context: root.value_context
        value_key: "hide_stam"
    }

    AppCheckbox {
        caption: qsTr("Keys,Fire/Poison pots")
        tooltip: ""
        value_context: root.value_context
        value_key: "hide_junks"
    }

    AppCheckbox {
        caption: qsTr("Hide low quality/damaged/cracked items")
        tooltip: ""
        value_context: root.value_context
        value_key: "hide_lowq"
    }
}
