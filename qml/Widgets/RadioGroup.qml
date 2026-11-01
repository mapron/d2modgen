import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

RowLayout {
    id: root
    spacing: 5

    //Layout.fillWidth: true

    property alias model: repeater.model

    property QtObject value_key: null

    property int currentIndex: value_key.value

    onCurrentIndexChanged: {
        if (currentIndex >= 0 && currentIndex < repeater.count) {
            var item = repeater.itemAt(currentIndex);
            if (item && !item.checked) {
                item.checked = true;
            }
        }

        value_key.value = currentIndex;
    }

    ButtonGroup {
        id: buttonGroup

        onCheckedButtonChanged: {
            if (checkedButton) {
                for (var i = 0; i < repeater.count; ++i) {
                    if (repeater.itemAt(i) === checkedButton) {
                        root.value_key.value = i;
                        break;
                    }
                }
            }
        }
    }

    Repeater {
        id: repeater

        delegate: RadioButton {
            text: modelData
            ButtonGroup.group: buttonGroup
            // Layout.fillWidth: true

            Component.onCompleted: {
                if (index === root.currentIndex) {
                    checked = true;
                }
            }
        }
    }

    //Item {
    //    Layout.fillWidth: true
    //}
}
