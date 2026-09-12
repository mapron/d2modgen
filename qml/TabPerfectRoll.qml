import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick

WidgetCommonTab {
    id: root
    value_context: "perfect_rolls"
    caption: qsTr("Perfect rolls")
    tooltip: qsTr("Remove random effect when rolling between min and max value for something.")

    WidgetCheckbox {
        caption: qsTr("Perfect rolls for crafting in the Cube")
        tooltip: ""
        value_context: root.value_context
        value_key: "craft"
    }

    WidgetCheckbox {
        caption: qsTr("Perfect Uniques")
        tooltip: ""
        value_context: root.value_context
        value_key: "uniques"
    }

    WidgetCheckbox {
        caption: qsTr("Perfect Rune Word rolls")
        tooltip: ""
        value_context: root.value_context
        value_key: "runeWords"
    }

    WidgetCheckbox {
        caption: qsTr("Perfect Set items")
        tooltip: ""
        value_context: root.value_context
        value_key: "setItems"
    }

    WidgetCheckbox {
        caption: qsTr("Perfect Magic/Rare affixes")
        tooltip: ""
        value_context: root.value_context
        value_key: "affixes"
    }

    WidgetCheckbox {
        caption: qsTr("Perfect predetermined crafted affixes")
        tooltip: ""
        value_context: root.value_context
        value_key: "crafted"
    }

    WidgetCheckbox {
        caption: qsTr("Always max defense Armor")
        tooltip: ""
        value_context: root.value_context
        value_key: "armor"
    }

    WidgetExponentialSlider {
        caption: qsTr("Unique items level 1 power multiply")
        tooltip: ""
        denom: 2
        mult: 5
        value_context: root.value_context
        value_key: "uniques_power_l1"
        isCompact: false
        suffix: "%"
    }

    WidgetExponentialSlider {
        caption: qsTr("Unique items level 50 power multiply")
        tooltip: ""
        denom: 2
        mult: 5
        value_context: root.value_context
        value_key: "uniques_power_l50"
        isCompact: false
        suffix: "%"
    }

    WidgetExponentialSlider {
        caption: qsTr("Set items level 1 power multiply")
        tooltip: ""
        denom: 2
        mult: 5
        value_context: root.value_context
        value_key: "set_power_l1"
        isCompact: false
        suffix: "%"
    }

    WidgetExponentialSlider {
        caption: qsTr("Set items level 50 power multiply")
        tooltip: ""
        denom: 2
        mult: 5
        value_context: root.value_context
        value_key: "set_power_l50"
        isCompact: false
        suffix: "%"
    }

    WidgetExponentialSlider {
        caption: qsTr("Magic affix level 1 power multiply")
        tooltip: ""
        denom: 2
        mult: 5
        value_context: root.value_context
        value_key: "affix_power_l1"
        isCompact: false
        suffix: "%"
    }

    WidgetExponentialSlider {
        caption: qsTr("Magic affix level 50 power multiply")
        tooltip: ""
        denom: 2
        mult: 5
        value_context: root.value_context
        value_key: "affix_power_l50"
        isCompact: false
        suffix: "%"
    }

    WidgetExponentialSlider {
        caption: qsTr("Rune Words power multiply")
        tooltip: ""
        denom: 2
        mult: 5
        value_context: root.value_context
        value_key: "rw_power"
        isCompact: false
        suffix: "%"
    }

    WidgetExponentialSlider {
        caption: qsTr("Crafted items power multiply")
        tooltip: ""
        denom: 2
        mult: 5
        value_context: root.value_context
        value_key: "crafted_power"
        isCompact: false
        suffix: "%"
    }

    WidgetExponentialSlider {
        caption: qsTr("Rune/Gems power multiply")
        tooltip: ""
        denom: 2
        mult: 5
        value_context: root.value_context
        value_key: "gem_power"
        isCompact: false
        suffix: "%"
    }
}
