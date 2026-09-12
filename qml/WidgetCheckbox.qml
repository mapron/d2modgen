import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

WidgetCommonControl {
    id: root

    isCompact: true
    controlAfterCaption: false
    hideCaption: true

    onAppDataChanged: {
        value = getValue();
        checkBox.checked = value == 1;
    }

    RowLayout {
        CheckBox {
            id: checkBox
            checked: root.value == 1

            indicator.implicitWidth: 22
            indicator.implicitHeight: 22

            font.pointSize: 11

            // Remove default right label behavior from standard control
            text: root.caption
            onClicked: {
                root.value = checked ? 1 : 0;
                root.setValue();
            }
        }

        Item {
            Layout.fillWidth: true
        } // Keeps elements left-aligned
    }
}
