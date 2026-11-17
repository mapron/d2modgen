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
    value_context: config.balanceGambling
    enabled_key: null
    caption: qsTr("Gambling")
    tooltip: qsTr("First option enables Charms in Gambling window, but only for D2 Legacy.<br>Next sliders allow you increase basic chance of getting Unique/Rare/Set/High-quality item.")

    WidgetCheckbox {
        caption: qsTr("Allow gambling for charms and jewels (Legacy only)")
        tooltip: qsTr("Note: buying charm in D2R crashes the game. So this have no effect in D2R output.")
        value_key: root.value_context.charmGamble
    }

    WidgetSlider {
        caption: qsTr("Increase Unique chance, times")
        tooltip: qsTr("Note: you still can get a lot of failed uniques for no known reason.")
        value_key: root.value_context.ratioUnique
    }

    WidgetSlider {
        caption: qsTr("Increase Set chance, times")
        tooltip: ""
        value_key: root.value_context.ratioSet
    }

    WidgetSlider {
        caption: qsTr("Increase Rare chance, times")
        tooltip: ""
        value_key: root.value_context.ratioRare
    }

    WidgetSlider {
        caption: qsTr("Increase Exceptional upgrade chance, times")
        tooltip: ""
        value_key: root.value_context.ratioExc
    }

    WidgetSlider {
        caption: qsTr("Increase Elite upgrade chance, times")
        tooltip: qsTr("Note: seems like Exc. check is done beforehead, so if it&apos;s high enough, elite won&apos;t generate.")
        value_key: root.value_context.ratioElite
    }
}
