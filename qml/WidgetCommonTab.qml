import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

ScrollView {
    clip: true
    // Hides scrollbar until the layout height drops below contents height requirements
    ScrollBar.vertical.policy: ScrollBar.AsNeeded
    ScrollBar.horizontal.policy: ScrollBar.AsNeeded

    property string value_context: ""
    property string caption: ""
    property string tooltip: ""

    default property alias contentSlot: contentContainer.data

    ColumnLayout {
        spacing: 16

        RowLayout {
            id: pageHeaderRow
            spacing: 6

            Label {
                text: root.caption
                font.pointSize: 12
            }

            WidgetHelpIcon {
                tooltipText: root.tooltip
            }

            Item {
                Layout.fillWidth: true
            }

            CheckBox {
                id: headerCheckbox

                checked: appui.getEnabled(value_context)
                text: qsTr("Enable this tab")

                Connections {
                    target: appui
                    function onDataChanged() {
                        headerCheckbox.checked = appui.getEnabled(value_context);
                        contentContainer.enabled = headerCheckbox.checked;
                    }
                }

                onClicked: {
                    appui.setEnabled(value_context, checked);
                }
            }
            Button {
                text: qsTr("Reset to default")
                onClicked: {
                    appui.resetToDefault(value_context);
                }
            }
        }

        ColumnLayout {
            id: contentContainer
            enabled: appui.getEnabled(value_context)
        }
    }
}
