/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * Copyright (C) 2026 Bertus Steenberg / bertus.steenberg@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

Dialog {
    id: root
    property var matches: []
    property int selected: 0
    property bool isD2R: appConfig.versionMajor.value === 1

    function show(list, preselected) {
        matches = list;
        selected = preselected;
        open();
    }

    title: qsTr("Several Wine prefixes contain Diablo II")
    modal: true
    anchors.centerIn: Overlay.overlay
    width: Math.min(Overlay.overlay.width - 40, 600)
    standardButtons: Dialog.Ok | Dialog.Cancel

    onAccepted: appui.applyMatch(root.selected)

    ButtonGroup {
        id: group
    }

    contentItem: ScrollView {
        id: scrollView
        readonly property int maxHeight: 300
        // Sizes to the rows up to maxHeight, then scrolls. Fusion hides an idle scroll bar and draws it over the
        // content, so keep it shown when rows are cut off, and keep the rows clear of it.
        implicitHeight: Math.min(rows.implicitHeight, maxHeight)
        contentWidth: availableWidth - (ScrollBar.vertical.policy === ScrollBar.AlwaysOn ? ScrollBar.vertical.width : 0)
        clip: true

        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: rows.implicitHeight > maxHeight ? ScrollBar.AlwaysOn : ScrollBar.AsNeeded

        ColumnLayout {
            id: rows
            width: scrollView.contentWidth
            spacing: 10

            Repeater {
                model: root.matches

                RowLayout {
                    id: row
                    required property var modelData
                    required property int index
                    Layout.fillWidth: true
                    spacing: 6

                    RadioButton {
                        ButtonGroup.group: group
                        checked: row.index === root.selected
                        onClicked: root.selected = row.index
                        Layout.alignment: Qt.AlignTop
                    }
                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 2
                        Label {
                            text: row.modelData.prefix
                            font.bold: true
                            Layout.fillWidth: true
                            wrapMode: Text.Wrap
                        }
                        Label {
                            text: {
                                let details = [qsTr("Game: %1").arg(row.modelData.installPath), row.modelData.hasSavedGames ? qsTr("Saved Games found") : qsTr("No Saved Games")];
                                if (root.isD2R)
                                    details.push(row.modelData.hasBattleNetConfig ? qsTr("Battle.net settings found") : qsTr("No Battle.net settings"));
                                return details.join(", ");
                            }
                            Layout.fillWidth: true
                            wrapMode: Text.Wrap
                        }
                    }
                }
            }
        }
    }
}
