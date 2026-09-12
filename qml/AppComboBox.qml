import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

AppCommonControl {
    id: root

    property alias model: internalCombo.model
    property alias textRole: internalCombo.textRole

    onAppDataChanged: {
        value = getValue();
        internalCombo.currentIndex = value;
    }

    ComboBox {
        id: internalCombo
        Layout.fillWidth: false
        // Layout.preferredWidth: internalCombo.implicitWidth
        // Sync the visual selection with the actual backend numerical index value
        currentIndex: root.value
        implicitContentWidthPolicy: ComboBox.WidestTextWhenCompleted

        topPadding: 2
        bottomPadding: 2
        leftPadding: 4

        // Triggered only when the user physically clicks and selects a new option
        onActivated: index => {
            root.value = index;
            root.setValue(); // Fires your appui.set method automatically
        }
    }
}
