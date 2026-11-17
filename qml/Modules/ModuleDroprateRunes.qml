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
    value_context: config.droprate.runes
    enabled_key: config.droprate.runes.enabled
    caption: qsTr("Increase Rune drops")
    tooltip: qsTr("All help and explanation on this page assume you are familiar with Treasure Class (TC) and NoDrop terminology.<br>Basically all options here modifying TC pick weights - so one preferred option become less rare.")

    WidgetPreset {
        model: [qsTr("Select preset..."), qsTr("Gimme pile of Ber runes!"), qsTr("I want to make Infinity at least once in my life"), qsTr("I want high runes to be rare, but want more rune drops overall"),]
        presetIds: ["", "crazy", "strong", "weak"]
        value_key: config.droprate.runes
    }

    WidgetSlider {
        caption: qsTr("Increase Rare Rune drops")
        tooltip: qsTr("That will drastically improve High Rune drop rates! You probably don&apos;t need more than 50x.<br>Value here increases chance of dropping Zod in 'Runes 17' TC<br>Rarity of other runes will change proportionally <br>(so High Runes still be more rare in the same manner).")
        value_key: root.value_context.zodFactor
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("Increase Runes chance in Good TC")
        tooltip: qsTr("When dropping from Good TC, that will make Runes drop more often <br>compared to other jewellery (Gems/Rings/Amulets/Charms)")
        value_key: root.value_context.runeInGoodTC
        isCompact: false
    }

    WidgetCheckbox {
        caption: qsTr("Make Countess Drop higher runes (up to Runes 17=Zod on Hell)")
        tooltip: ""
        value_key: root.value_context.countessSpecialRuneLevel
    }

    WidgetCheckbox {
        caption: qsTr("Increase Countess Runes drop count to 5")
        tooltip: ""
        value_key: root.value_context.countessSpecialRuneMore
    }

    WidgetCheckbox {
        caption: qsTr("Increase Wraiths Runes drop")
        tooltip: ""
        value_key: root.value_context.wraithOnlyRunes
    }
}
