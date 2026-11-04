/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

CheckBox {
    id: checkBox

    property QtObject value_key: null

    checked: value_key.value

    indicator.implicitWidth: 18
    indicator.implicitHeight: 18

    onClicked: value_key.value = checked
}
