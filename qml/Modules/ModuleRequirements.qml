import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick
import "../Widgets"

WidgetCommonTab {
    id: root
    value_context: config.moduleRequirements
    enabled_key: config.enabledModules.moduleRequirements
    caption: qsTr("Requirements")
    tooltip: qsTr("Options for lowering/raising requirements for equimpemnt. ")

    WidgetRangeSlider {
        caption: qsTr("Change Strength requirements on items, %")
        tooltip: qsTr("If you set value above 100%, Strength requirements <br> become higher compared to the original.")
        from: 5
        to: 150
        value_key: root.value_context.reqStr
        isCompact: false
        suffix: "%"
    }

    WidgetRangeSlider {
        caption: qsTr("Change Dexterity requirements on items, %")
        tooltip: ""
        from: 5
        to: 150
        value_key: root.value_context.reqDex
        isCompact: false
        suffix: "%"
    }

    WidgetRangeSlider {
        caption: qsTr("Change Character level requirement on regular items, %")
        tooltip: qsTr("That controls 'Level required:' you see on the items.")
        from: 5
        to: 150
        value_key: root.value_context.equipReqLevel
        isCompact: false
        suffix: "%"
    }

    WidgetRangeSlider {
        caption: qsTr("Change Character level requirement on Unique/Set items, %")
        tooltip: ""
        from: 5
        to: 150
        value_key: root.value_context.uniquesReqLevel
        isCompact: false
        suffix: "%"
    }

    WidgetRangeSlider {
        caption: qsTr("Change Character level requirement on Magic/Rare items, %")
        tooltip: ""
        from: 5
        to: 150
        value_key: root.value_context.affixReqLevel
        isCompact: false
        suffix: "%"
    }

    WidgetRangeSlider {
        caption: qsTr("Change Minimal level required to spent points into skill, %")
        tooltip: qsTr("For example, if you select 50%, you can get level 30 skill on level 15 instead.")
        from: 5
        to: 150
        value_key: root.value_context.skillsReqLevel
        isCompact: false
        suffix: "%"
    }

    WidgetRangeSlider {
        caption: qsTr("Change quality level on regular items, %")
        tooltip: qsTr("That controls how valueable item is from drop calculation process.<br> Lower value means you get item dropped much earlier in the game.")
        from: 5
        to: 150
        value_key: root.value_context.equipItemLevel
        isCompact: false
        suffix: "%"
    }

    WidgetRangeSlider {
        caption: qsTr("Change quality level on Unique/Set items, %")
        tooltip: ""
        from: 5
        to: 150
        value_key: root.value_context.uniquesItemLevel
        isCompact: false
        suffix: "%"
    }

    WidgetRangeSlider {
        caption: qsTr("Change quality level on Magic/Rare items, %")
        tooltip: ""
        from: 5
        to: 150
        value_key: root.value_context.affixItemLevel
        isCompact: false
        suffix: "%"
    }
}
