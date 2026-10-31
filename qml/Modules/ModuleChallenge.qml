import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick
import "../Widgets"

WidgetCommonTab {
    id: root
    value_context: config.moduleChallenge
    enabled_key: config.enabledModules.moduleChallenge
    caption: qsTr("Challenge")
    tooltip: qsTr("First, you can disable potion drops entirely (that's not the same as drop filter).<br>Second, you can change resistance penalty for each difficulty from default 0/40/100.<br>And the last, you can adjust level area on all maps; <br>note that max value of 85 is still used (so set to 20 to basically make all Hell areas lvl 85).")

    WidgetCheckbox {
        caption: qsTr("Disable drop: All Health pots")
        tooltip: ""
        value_key: root.value_context.nodrop_hpsa
    }

    WidgetCheckbox {
        caption: qsTr("Disable drop: All Mana pots")
        tooltip: ""
        value_key: root.value_context.nodrop_mpsa
    }

    WidgetCheckbox {
        caption: qsTr("Disable drop: Rejuv and Full Rejuv pots")
        tooltip: ""
        value_key: root.value_context.nodrop_rvs
    }

    WidgetRangeSlider {
        caption: qsTr("Normal difficulty resistance penalty, -all%")
        tooltip: ""
        value_key: root.value_context.normal_minus_res
        isCompact: false
    }

    WidgetRangeSlider {
        caption: qsTr("Nightmare difficulty resistance penalty, -all%")
        tooltip: ""
        value_key: root.value_context.nightmare_minus_res
        isCompact: false
    }

    WidgetRangeSlider {
        caption: qsTr("Hell difficulty resistance penalty, -all%")
        tooltip: ""
        value_key: root.value_context.hell_minus_res
        isCompact: false
    }

    WidgetRangeSlider {
        caption: qsTr("Increase Nightmare area levels, +levels")
        tooltip: ""
        value_key: root.value_context.levelIncreaseNightmare
        isCompact: false
    }

    WidgetRangeSlider {
        caption: qsTr("Increase Hell area levels, +levels")
        tooltip: ""
        value_key: root.value_context.levelIncreaseHell
        isCompact: false
    }

    WidgetCheckbox {
        caption: qsTr("Go beyond 85 level for areas")
        tooltip: ""
        value_key: root.value_context.levelIncreaseUltra
    }
}
