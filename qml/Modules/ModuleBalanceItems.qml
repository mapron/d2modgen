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
    value_context: config.balanceItems
    enabled_key: null
    caption: qsTr("Item balance")
    tooltip: qsTr("Remove random effect when rolling between min and max value for something.")

    ModuleBalanceItemsCommon {
        caption: qsTr("Common settings")
        value_context: config.balanceItems.defaults
        isDefault: true
    }
    ModuleBalanceItemsCommon {
        caption: qsTr("Armor and weapons bases")
        isEquipment: true
        value_context: config.balanceItems.equipment
    }
    ModuleBalanceItemsCommon {
        caption: qsTr("Uniques")
        value_context: config.balanceItems.uniques
    }
    ModuleBalanceItemsCommon {
        caption: qsTr("Set items")
        value_context: config.balanceItems.setitems
    }
    ModuleBalanceItemsCommon {
        caption: qsTr("Sets")
        value_context: config.balanceItems.sets
    }
    ModuleBalanceItemsCommon {
        caption: qsTr("Runewords")
        value_context: config.balanceItems.runewords
    }
    ModuleBalanceItemsCommon {
        caption: qsTr("Magic/rare affixes")
        value_context: config.balanceItems.sets
    }
    ModuleBalanceItemsCommon {
        caption: qsTr("Cube crafted affixes")
        value_context: config.balanceItems.crafted
    }
    ModuleBalanceItemsCommon {
        caption: qsTr("Gems and runes")
        value_context: config.balanceItems.gems
    }

    Separator {}

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
}
