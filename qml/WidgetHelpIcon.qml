import QtQuick
import QtQuick.Controls.Fusion

Label {
    id: root
    property string tooltipText: ""

    // Only show if tooltip text is provided
    visible: tooltipText !== ""
    text: "🛈"
    font.pointSize: 14
    color: window.palette.highlight

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true

        ToolTip {
            id: instantTip
            text: root.tooltipText
            visible: parent.containsMouse
            delay: 0
            timeout: 5000

            // Automatically flips text color matching light/dark states
            contentItem: Text {
                text: instantTip.text
                color: window.palette.windowText
                font.pointSize: 9
                textFormat: Text.RichText
            }

            // Smooth, adaptive background frame
            background: Rectangle {
                color: window.palette.window

                border.color: window.palette.highlight
                border.width: 2
                radius: 4
            }
        }
    }
}
