/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
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

    Label {
        text: qsTr("Disable potion drops entirely:")
    }

    RowLayout {
        WidgetCheckbox {
            caption: qsTr("All Health")
            tooltip: ""
            value_key: root.value_context.disableDropsHealthPot
        }

        WidgetCheckbox {
            caption: qsTr("All Mana")
            tooltip: ""
            value_key: root.value_context.disableDropsManaPot
        }

        WidgetCheckbox {
            caption: qsTr("Rejuv and Full Rejuv")
            tooltip: ""
            value_key: root.value_context.disableDropsRejuvPot
        }
    }
    Separator {}

    WidgetSlider {
        caption: qsTr("Normal difficulty resistance penalty, -all%")
        tooltip: ""
        value_key: root.value_context.normalResPenalty
    }

    WidgetSlider {
        caption: qsTr("Nightmare difficulty resistance penalty, -all%")
        tooltip: ""
        value_key: root.value_context.nightmareResPenalty
    }

    WidgetSlider {
        caption: qsTr("Hell difficulty resistance penalty, -all%")
        tooltip: ""
        value_key: root.value_context.hellResPenalty
    }

    WidgetSlider {
        caption: qsTr("Increase Nightmare area levels, +levels")
        tooltip: ""
        value_key: root.value_context.levelIncreaseNightmare
    }

    WidgetSlider {
        caption: qsTr("Increase Hell area levels, +levels")
        tooltip: ""
        value_key: root.value_context.levelIncreaseHell
    }

    WidgetCheckbox {
        caption: qsTr("Go beyond 85 level for areas")
        tooltip: ""
        value_key: root.value_context.levelIncreaseOverBound
    }
}
