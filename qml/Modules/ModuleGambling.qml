import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick
import "../Widgets"

WidgetCommonTab {
    id: root
    value_context: config.gambling
    enabled_key: null
    caption: qsTr("Gambling")
    tooltip: qsTr("First option enables Charms in Gambling window, but only for D2 Legacy.<br>Next sliders allow you increase basic chance of getting Unique/Rare/Set/High-quality item.")

    WidgetCheckbox {
        caption: qsTr("Allow gambling for charms and jewels (Legacy only)<br>Note: crashes D2R - so enabled only for D2.")
        tooltip: ""
        value_key: root.value_context.charmGamble
    }

    WidgetSlider {
        caption: qsTr("Increase Unique chance, times<br>Note: you still can get a lot of failed uniques for no known reason.")
        tooltip: ""
        value_key: root.value_context.ratioUnique
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("Increase Set chance, times")
        tooltip: ""
        value_key: root.value_context.ratioSet
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("Increase Rare chance, times")
        tooltip: ""
        value_key: root.value_context.ratioRare
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("Increase Exceptional upgrade chance, times")
        tooltip: ""
        value_key: root.value_context.ratioExc
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("Increase Elite upgrade chance, times<br>Note: seems like Exc. check is done beforehead, so if it&apos;s high enough, elite won&apos;t generate.")
        tooltip: ""
        value_key: root.value_context.ratioElite
        isCompact: false
    }
}
