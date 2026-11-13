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
    value_context: config.droprate.misc
    enabled_key: config.droprate.misc.enabled
    caption: qsTr("Increase Misc drops")

    WidgetCheckbox {
        caption: qsTr("Remove gold from Champion drops")
        tooltip: ""
        value_key: root.value_context.removeChampGold
    }
    WidgetCheckbox {
        caption: qsTr("Remove all potions from elites")
        tooltip: ""
        value_key: root.value_context.removeElitePotions
    }

    WidgetSlider {
        caption: qsTr("Good TC (Runes/Gems/Jewellery)")
        tooltip: qsTr("That will make Gems/Runes/Rings/Amulets/Charms drop far more often<br>compared to equipment (armor/weapons).")
        value_key: root.value_context.goodPercent
    }

    WidgetSlider {
        caption: qsTr("Gold (higher=MORE gold)")
        tooltip: qsTr("Relative chance of gold compared to other items.")
        value_key: root.value_context.goldPercent
    }

    WidgetSlider {
        caption: qsTr("Junk (keys/bolts/etc) (higher=MORE junk)")
        tooltip: qsTr("Relative chance of junk items compared to other items.")
        value_key: root.value_context.junkPercent
    }
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
}
