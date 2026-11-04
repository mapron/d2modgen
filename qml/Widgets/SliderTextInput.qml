/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

TextField {
    id: root

    property QtObject value_key: null

    property int from: value_key.min
    property int to: value_key.max

    property int visualWidth: maxTextMetrics.width + leftPadding + rightPadding + 4

    Layout.fillWidth: false
    Layout.fillHeight: true
    Layout.preferredWidth: root.visualWidth

    validator: IntValidator {
        bottom: root.from
        top: root.to
    }
    topPadding: 2
    bottomPadding: 2

    background: null

    selectByMouse: true
    verticalAlignment: TextInput.AlignVCenter

    text: value_key.value.toString()

    onEditingFinished: value_key.value = parseInt(text)

    TextMetrics {
        id: maxTextMetrics
        font: root.font

        text: {
            let minStr = root.from.toString();
            let maxStr = root.to.toString();
            return minStr.length > maxStr.length ? minStr : maxStr;
        }
    }
}
