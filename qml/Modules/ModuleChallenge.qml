import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick
import "../Widgets"

WidgetCommonTab {
    id: root
    value_context: config.challenge
    enabled_key: null
    caption: qsTr("Challenge")
    tooltip: qsTr("First, you can disable potion drops entirely (that's not the same as drop filter).<br>Second, you can change resistance penalty for each difficulty from default 0/40/100.<br>And the last, you can adjust level area on all maps; <br>note that max value of 85 is still used (so set to 20 to basically make all Hell areas lvl 85).")

    WidgetCheckbox {
        caption: qsTr("Disable drop: All Health pots")
        tooltip: ""
        value_key: root.value_context.disableDropsHealthPot
    }

    WidgetCheckbox {
        caption: qsTr("Disable drop: All Mana pots")
        tooltip: ""
        value_key: root.value_context.disableDropsManaPot
    }

    WidgetCheckbox {
        caption: qsTr("Disable drop: Rejuv and Full Rejuv pots")
        tooltip: ""
        value_key: root.value_context.disableDropsRejuvPot
    }

    WidgetSlider {
        caption: qsTr("Normal difficulty resistance penalty, -all%")
        tooltip: ""
        value_key: root.value_context.normalResPenalty
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("Nightmare difficulty resistance penalty, -all%")
        tooltip: ""
        value_key: root.value_context.nightmareResPenalty
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("Hell difficulty resistance penalty, -all%")
        tooltip: ""
        value_key: root.value_context.hellResPenalty
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("Increase Nightmare area levels, +levels")
        tooltip: ""
        value_key: root.value_context.levelIncreaseNightmare
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("Increase Hell area levels, +levels")
        tooltip: ""
        value_key: root.value_context.levelIncreaseHell
        isCompact: false
    }

    WidgetCheckbox {
        caption: qsTr("Go beyond 85 level for areas")
        tooltip: ""
        value_key: root.value_context.levelIncreaseOverBound
    }
}
