import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick

AppCommonTab {
    id: root
    value_context: "perfect_rolls"
    caption: qsTr("Perfect rolls")
    tooltip: qsTr("Remove random effect when rolling between min and max value for something.")

    AppCheckbox {
        caption: qsTr("Perfect rolls for crafting in the Cube")
        tooltip: ""
        value_context: root.value_context
        value_key: "craft"
    }

    AppCheckbox {
        caption: qsTr("Perfect Uniques")
        tooltip: ""
        value_context: root.value_context
        value_key: "uniques"
    }

    AppCheckbox {
        caption: qsTr("Perfect Rune Word rolls")
        tooltip: ""
        value_context: root.value_context
        value_key: "runeWords"
    }

    AppCheckbox {
        caption: qsTr("Perfect Set items")
        tooltip: ""
        value_context: root.value_context
        value_key: "setItems"
    }

    AppCheckbox {
        caption: qsTr("Perfect Magic/Rare affixes")
        tooltip: ""
        value_context: root.value_context
        value_key: "affixes"
    }

    AppCheckbox {
        caption: qsTr("Perfect predetermined crafted affixes")
        tooltip: ""
        value_context: root.value_context
        value_key: "crafted"
    }

    AppCheckbox {
        caption: qsTr("Always max defense Armor")
        tooltip: ""
        value_context: root.value_context
        value_key: "armor"
    }

    AppExponentialSlider {
        caption: qsTr("Unique items level 1 power multiply")
        tooltip: ""
        denom: 2
        mult: 5
        value_context: root.value_context
        value_key: "uniques_power_l1"
        isCompact: false
        suffix: "%"
    }

    AppExponentialSlider {
        caption: qsTr("Unique items level 50 power multiply")
        tooltip: ""
        denom: 2
        mult: 5
        value_context: root.value_context
        value_key: "uniques_power_l50"
        isCompact: false
        suffix: "%"
    }

    AppExponentialSlider {
        caption: qsTr("Set items level 1 power multiply")
        tooltip: ""
        denom: 2
        mult: 5
        value_context: root.value_context
        value_key: "set_power_l1"
        isCompact: false
        suffix: "%"
    }

    AppExponentialSlider {
        caption: qsTr("Set items level 50 power multiply")
        tooltip: ""
        denom: 2
        mult: 5
        value_context: root.value_context
        value_key: "set_power_l50"
        isCompact: false
        suffix: "%"
    }

    AppExponentialSlider {
        caption: qsTr("Magic affix level 1 power multiply")
        tooltip: ""
        denom: 2
        mult: 5
        value_context: root.value_context
        value_key: "affix_power_l1"
        isCompact: false
        suffix: "%"
    }

    AppExponentialSlider {
        caption: qsTr("Magic affix level 50 power multiply")
        tooltip: ""
        denom: 2
        mult: 5
        value_context: root.value_context
        value_key: "affix_power_l50"
        isCompact: false
        suffix: "%"
    }

    AppExponentialSlider {
        caption: qsTr("Rune Words power multiply")
        tooltip: ""
        denom: 2
        mult: 5
        value_context: root.value_context
        value_key: "rw_power"
        isCompact: false
        suffix: "%"
    }

    AppExponentialSlider {
        caption: qsTr("Crafted items power multiply")
        tooltip: ""
        denom: 2
        mult: 5
        value_context: root.value_context
        value_key: "crafted_power"
        isCompact: false
        suffix: "%"
    }

    AppExponentialSlider {
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
