import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick

WidgetCommonTab {
    id: root
    value_context: appModuleMonStats
    caption: qsTr("Monster stats")
    tooltip: qsTr("Sliders allow you to change monster stats on all difficulties.<br>You can either lower or raise stats up 10x or 10x times.<br>That is orthogonal to /playersX setting - it will multiply stats independent.")

    WidgetExponentialSlider {
        caption: qsTr("Monster Attack Rating, multiply by")
        tooltip: ""
        denom: 10
        mult: 10
        value_context: root.value_context
        value_key: "mon_ar"
        isCompact: false
        suffix: "%"
    }

    WidgetExponentialSlider {
        caption: qsTr("Monster Defense, multiply by")
        tooltip: ""
        denom: 10
        mult: 10
        value_context: root.value_context
        value_key: "mon_def"
        isCompact: false
        suffix: "%"
    }

    WidgetExponentialSlider {
        caption: qsTr("Monster HP, multiply by")
        tooltip: ""
        denom: 10
        mult: 10
        value_context: root.value_context
        value_key: "mon_hp"
        isCompact: false
        suffix: "%"
    }

    WidgetExponentialSlider {
        caption: qsTr("Monster Damage, multiply by")
        tooltip: ""
        denom: 10
        mult: 10
        value_context: root.value_context
        value_key: "mon_dam"
        isCompact: false
        suffix: "%"
    }

    WidgetExponentialSlider {
        caption: qsTr("Monster EXP gain, multiply by")
        tooltip: ""
        denom: 10
        mult: 10
        value_context: root.value_context
        value_key: "mon_xp"
        isCompact: false
        suffix: "%"
    }

    WidgetSlider {
        caption: qsTr("Maximum resistance allowed for base monsters, percent:")
        tooltip: qsTr("If you make this below 100, then regular monsters will have NO IMMUNITIES at all!<br>Use with caution. You can set this to 110-115, to make cold immunes breakable.<br>Also this do not prevent Enchanted monsters to have immunities.")
        from: 90
        to: 250
        value_context: root.value_context
        value_key: "max_resist"
        isCompact: false
    }

    WidgetExponentialSlider {
        caption: qsTr("Change monster resistances, by multiplying damage from magic they take")
        tooltip: qsTr("DO NOT affect IMMUNE monsters and monsters with 0 resistance!<br>It affects resistance perent indirectly, through this calculation:<br>Lets say you have 200% selected for multiply, and monster has 80% basic Fire resist.<br>That means, he has 20% damage taken from fire, and new value will be 40% damage, or 60% resist.<br>Damage is capped at 100%, so you won't get negative reistance.<br>Also monster can't become immune to element, maximum resistance is 99%")
        denom: 10
        mult: 10
        value_context: root.value_context
        value_key: "rel_resist"
        isCompact: false
        suffix: "%"
    }

    Label {
        text: qsTr("Apply resistance options to following:")
    }

    WidgetCheckbox {
        caption: qsTr("Elemental (Fire/Cold/Lightning)")
        tooltip: ""
        value_context: root.value_context
        value_key: "resist_apply_elem"
    }

    WidgetCheckbox {
        caption: qsTr("Poison")
        tooltip: ""
        value_context: root.value_context
        value_key: "resist_apply_poison"
    }

    WidgetCheckbox {
        caption: qsTr("Magic (Non-elemental)")
        tooltip: ""
        value_context: root.value_context
        value_key: "resist_apply_magic"
    }

    WidgetCheckbox {
        caption: qsTr("Physical")
        tooltip: ""
        value_context: root.value_context
        value_key: "resist_apply_physical"
    }
}
