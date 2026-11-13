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
    value_context: config.droprate.items
    enabled_key: config.droprate.items.enabled
    caption: qsTr("Increase Item drops")
    tooltip: qsTr("All help and explanation on this page assume you are familiar with Treasure Class (TC) and NoDrop terminology.<br>Basically all options here modifying TC pick weights - so one preferred option become less rare.<br>First 3 sliders are for increasing chance for Uniques/Sets/Rares. <br>Note that increase is accurate when your chances are low, but with high MF it can have diminishing return.")

    WidgetPreset {
        model: [qsTr("Select preset..."), qsTr("I want to find Tyrael's today!"), qsTr("Just tired of finding my first Shako"), qsTr("A little bit more of good stuff, not crazy"),]
        presetIds: ["", "crazy", "strong", "weak"]
        value_key: config.droprate.items
    }

    WidgetSlider {
        caption: qsTr("Increase Unique Chance")
        tooltip: ""
        value_key: root.value_context.uniqueFactorIncrease
    }

    WidgetSlider {
        caption: qsTr("Increase Set Chance")
        tooltip: ""
        value_key: root.value_context.setFactorIncrease
    }

    WidgetSlider {
        caption: qsTr("Increase Rare Chance")
        tooltip: ""
        value_key: root.value_context.rareFactorIncrease
    }

    WidgetSlider {
        caption: qsTr("Increase Quality (Rare/Set/Unique) for non-elites")
        tooltip: ""
        value_key: root.value_context.noneliteFactorIncrease
    }

    WidgetSlider {
        caption: qsTr("Increase Chance of High-level equipment")
        tooltip: qsTr("Usually when level 85 monster drops an item,<br> there is tiny chance that equpment will be elite base.<br>This increases chance for high-level monster drop high-level base.")
        value_key: root.value_context.chanceForHighLevel
    }

    WidgetSlider {
        caption: qsTr("Champion dropped item count")
        value_key: root.value_context.championItemCount
    }

    WidgetSlider {
        caption: qsTr("Unique dropped item count")
        value_key: root.value_context.uniqueItemCount
    }

    WidgetSlider {
        caption: qsTr("SuperUnique dropped item count")
        value_key: root.value_context.superItemCount
    }

    WidgetCheckbox {
        caption: qsTr("Always quest drops from bosses")
        tooltip: ""
        value_key: root.value_context.forceQuestDrops
    }

    Separator {}
    LabelImportant {
        text: qsTr("Adjust proportion of main drop classes")
    }

    WidgetSlider {
        caption: qsTr("Increse drop count (via NoDrop)")
        tooltip: qsTr("This provides ability to gradually reduce NoDrop picks<br>(it basically similar to increasing players count in the game)<br>This only affect base monster drops")
        value_key: root.value_context.inverseNodropPercent
    }
    WidgetSlider {
        caption: qsTr("Equipment (armor/weapons)")
        tooltip: qsTr("Relative chance of equipment dropping compared to other items (or NoDrop).")
        value_key: root.value_context.equipPercent
    }
}
