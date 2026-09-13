import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

ScrollView {
    id: rootScrollView
    clip: true
    // Hides scrollbar until the layout height drops below contents height requirements
    ScrollBar.vertical.policy: ScrollBar.AsNeeded
    ScrollBar.horizontal.policy: ScrollBar.AsNeeded

    ScrollBar.vertical.contentItem.opacity: rootScrollView.contentHeight > rootScrollView.height ? 1.0 : 0.0

    // Prevent the Fusion style from forcing a fade-out animation when unhovered
    Component.onCompleted: {
        ScrollBar.vertical.contentItem.opacityChanged.connect(function () {
            if (rootScrollView.contentHeight > rootScrollView.height) {
                ScrollBar.vertical.contentItem.opacity = 1.0;
            }
        });
    }
    contentWidth: availableWidth
    rightPadding: 30

    default property alias contentSlot: scrollViewContainer.data

    ColumnLayout {
        width: rootScrollView.availableWidth
        ColumnLayout {
            id: scrollViewContainer
            spacing: 16
        }
    }
}
