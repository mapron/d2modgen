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
    value_context: config.otherQualityOfLife
    enabled_key: null
    caption: qsTr("Quality of life")
    tooltip: qsTr("Tiny quality things to make you life easier: <br>1. Insreasing stacks for tomes/quivers/keys;<br>2. Remove Unique charm limit (useful with randomizer);<br>3. Allow usage of some skills in the town;<br>4. Reduce item costs. ")

    LabelLarge {
        text: qsTr("Start conditions:")
    }

    WidgetCheckbox {
        caption: qsTr("Add Horadric Cube as starting item")
        tooltip: ""
        value_key: root.value_context.addCube
    }

    WidgetCheckbox {
        caption: qsTr("Replace ID scroll with ID tome")
        tooltip: ""
        value_key: root.value_context.idTome
    }

    WidgetCheckbox {
        caption: qsTr("Replace TP scroll with TP tome")
        tooltip: ""
        value_key: root.value_context.tpTome
    }

    WidgetCheckbox {
        caption: qsTr("Replace starting Health pots with Mana")
        tooltip: ""
        value_key: root.value_context.manaPots
    }

    WidgetCheckbox {
        caption: qsTr("Add Teleport as starting skill for all classes")
        tooltip: ""
        value_key: root.value_context.addTeleport
    }

    Separator {}

    WidgetCheckbox {
        caption: qsTr("Remove limit on Unique charms")
        tooltip: qsTr("You can carry several Gheed's or Torches.")
        value_key: root.value_context.uniqueCharmLimit
    }

    Separator {}

    WidgetSlider {
        caption: qsTr("Tomes (ID/TP) size")
        tooltip: ""
        value_key: root.value_context.tomeSize
    }

    WidgetSlider {
        caption: qsTr("Key chain size")
        tooltip: ""
        value_key: root.value_context.keySize
    }

    WidgetSlider {
        caption: qsTr("Quivers size")
        tooltip: qsTr("D2R already have 500 quiver size, and PD2 have unlimited, so use this for LoD")
        value_key: root.value_context.quiverSize
    }

    WidgetSlider {
        caption: qsTr("Reduce costs of skills and stats on items (affects repair cost mostly)")
        tooltip: ""
        value_key: root.value_context.reduceCost
        isCompact: false
    }

    RowLayout {
        spacing: 6
        Layout.fillWidth: true

        LabelLarge {
            text: qsTr("Make items' names compact - world's smallest loot filter!")
        }

        WidgetHelpIcon {
            tooltipText: qsTr("this will make item names take less space, e.g. '!HP2' for health potion.<br>Have only effect for D2R. Use loot filters for PD2.")
        }

        Item {
            Layout.fillWidth: true
        }
    }

    WidgetCheckbox {
        caption: qsTr("Compact potion names")
        tooltip: ""
        value_key: root.value_context.compactPots
    }

    WidgetCheckbox {
        caption: qsTr("Compact TP/ID scrolls")
        tooltip: ""
        value_key: root.value_context.compactScrolls
    }
    WidgetCheckbox {
        caption: qsTr("Show ilevel in items' titles")
        tooltip: ""
        value_key: root.value_context.showItemLevel
    }
}
