/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion
import QtQuick.Dialogs

import CustomValidators 1.0

WidgetCommonControl {
    id: root
    property QtObject value_key: null
    property bool showDetect: false
    property bool validateNonEmpty: false

    isCompact: false
    control: RowLayout {
        Layout.fillWidth: true

        DynamicLineEdit {
            validator: BasicPathValidator {
                enabled: root.validateNonEmpty
            }
            Layout.fillWidth: true
            value_key: root.value_key
        }
        Button {
            visible: root.showDetect // not ideal but...
            text: qsTr("Detect!")
            onClicked: root.value_key.value = appui.detectPath()
        }
        Button {
            text: qsTr("Browse...")
            onClicked: {
                folderDialogInputPath.currentFolder = appui.pathToUrl(root.value_key.value);
                folderDialogInputPath.open();
            }
        }
    }
    FolderDialog {
        id: folderDialogInputPath
        onAccepted: root.value_key.value = appui.urlToPath(folderDialogInputPath.selectedFolder)
    }
}
