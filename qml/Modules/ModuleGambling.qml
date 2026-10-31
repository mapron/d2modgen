import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick
import "../Widgets"

WidgetCommonTab {
    id: root
    value_context: config.moduleGambling
    enabled_key: config.enabledModules.moduleGambling
    caption: qsTr("Gambling")
    tooltip: qsTr("First option enables Charms in Gambling window, but only for D2 Legacy.<br>Next sliders allow you increase basic chance of getting Unique/Rare/Set/High-quality item.")

    WidgetCheckbox {
        caption: qsTr("Allow gambling for charms and jewels (Legacy only)<br>Note: crashes D2R - so enabled only for D2.")
        tooltip: ""
        value_key: root.value_context.charmGamble
    }

    WidgetRangeSlider {
        caption: qsTr("Increase Unique chance, times<br>Note: you still can get a lot of failed uniques for no known reason.")
        tooltip: ""
        value_key: root.value_context.ratioUnique
        isCompact: false
    }

    WidgetRangeSlider {
        caption: qsTr("Increase Set chance, times")
        tooltip: ""
        value_key: root.value_context.ratioSet
        isCompact: false
    }

    WidgetRangeSlider {
        caption: qsTr("Increase Rare chance, times")
        tooltip: ""
        value_key: root.value_context.ratioRare
        isCompact: false
    }

    WidgetRangeSlider {
        caption: qsTr("Increase Exceptional upgrade chance, times")
        tooltip: ""
        value_key: root.value_context.ratioExc
        isCompact: false
    }

    WidgetRangeSlider {
        caption: qsTr("Increase Elite upgrade chance, times<br>Note: seems like Exc. check is done beforehead, so if it&apos;s high enough, elite won&apos;t generate.")
        tooltip: ""
        value_key: root.value_context.ratioElite
        isCompact: false
    }
}
