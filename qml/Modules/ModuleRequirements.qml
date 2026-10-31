import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick
import "../Widgets"

WidgetCommonTab {
    id: root
    value_context: config.moduleRequirements
    enabled_key: null
    caption: qsTr("Requirements")
    tooltip: qsTr("Options for lowering/raising requirements for equimpemnt. ")

    WidgetSlider {
        caption: qsTr("Change Strength requirements on items")
        tooltip: qsTr("If you set value above 100%, Strength requirements <br> become higher compared to the original.")
        value_key: root.value_context.reqStr
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("Change Dexterity requirements on items")
        tooltip: ""
        value_key: root.value_context.reqDex
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("Change Character level requirement on regular items")
        tooltip: qsTr("That controls 'Level required:' you see on the items.")
        value_key: root.value_context.equipReqLevel
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("Change Character level requirement on Unique/Set items")
        tooltip: ""
        value_key: root.value_context.uniquesReqLevel
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("Change Character level requirement on Magic/Rare items")
        tooltip: ""
        value_key: root.value_context.affixReqLevel
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("Change Minimal level required to spent points into skill")
        tooltip: qsTr("For example, if you select 50%, you can get level 30 skill on level 15 instead.")
        value_key: root.value_context.skillsReqLevel
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("Change quality level on regular items")
        tooltip: qsTr("That controls how valueable item is from drop calculation process.<br> Lower value means you get item dropped much earlier in the game.")
        value_key: root.value_context.equipItemLevel
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("Change quality level on Unique/Set items")
        tooltip: ""
        value_key: root.value_context.uniquesItemLevel
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("Change quality level on Magic/Rare items")
        tooltip: ""
        value_key: root.value_context.affixItemLevel
        isCompact: false
    }
}
