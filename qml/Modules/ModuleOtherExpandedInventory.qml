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
    value_context: config.otherExpandedInventory
    enabled_key: null
    caption: qsTr("Expanded inventory")

    WidgetSlider {
        caption: qsTr("Inventory height")
        value_key: root.value_context.inventoryHeight
    }
    WidgetSlider {
        caption: qsTr("Inventory width")
        value_key: root.value_context.inventoryWidth
    }
}
