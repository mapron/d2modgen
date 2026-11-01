import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

RowLayout {
    property alias model: internalCombo.model
    property alias presetIds: internalCombo.presetIds
    property QtObject value_key: null

    spacing: 6

    Item {
        Layout.fillWidth: true
    }

    Label {
        text: qsTr("Don't know where to start? Select a preset:")
    }

    ComboBox {
        id: internalCombo
        Layout.fillWidth: false
        // Layout.preferredWidth: internalCombo.implicitWidth
        // Sync the visual selection with the actual backend numerical index value
        currentIndex: 0
        implicitContentWidthPolicy: ComboBox.WidestTextWhenCompleted

        property var presetIds: []

        topPadding: 2
        bottomPadding: 2
        leftPadding: 4

        onActivated: index => {
            let selectedId = presetIds[index];
            value_key.activatePreset(selectedId);
        }
    }
}
