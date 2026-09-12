import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick

AppCommonTab {
    id: root
    value_context: "gambling"
    caption: qsTr("Gambling")
    tooltip: qsTr("First option enables Charms in Gambling window, but only for D2 Legacy.<br>Next sliders allow you increase basic chance of getting Unique/Rare/Set/High-quality item.")

    AppCheckbox {
        caption: qsTr("Allow gambling for charms and jewels (Legacy only)<br>Note: crashes D2R - so enabled only for D2.")
        tooltip: ""
        value_context: root.value_context
        value_key: "charmGamble"
    }

    AppSlider {
        caption: qsTr("Increase Unique chance, times<br>Note: you still can get a lot of failed uniques for no known reason.")
        tooltip: ""
        from: 1
        to: 500
        value_context: root.value_context
        value_key: "ratioUnique"
        isCompact: false
    }

    AppSlider {
        caption: qsTr("Increase Set chance, times")
        tooltip: ""
        from: 1
        to: 250
        value_context: root.value_context
        value_key: "ratioSet"
        isCompact: false
    }

    AppSlider {
        caption: qsTr("Increase Rare chance, times")
        tooltip: ""
        from: 1
        to: 4
        value_context: root.value_context
        value_key: "ratioRare"
        isCompact: false
    }

    AppSlider {
        caption: qsTr("Increase Exceptional upgrade chance, times")
        tooltip: ""
        from: 1
        to: 4
        value_context: root.value_context
        value_key: "ratioExc"
        isCompact: false
    }

    AppSlider {
        caption: qsTr("Increase Elite upgrade chance, times<br>Note: seems like Exc. check is done beforehead, so if it&apos;s high enough, elite won&apos;t generate.")
        tooltip: ""
        from: 1
        to: 10
        value_context: root.value_context
        value_key: "ratioElite"
        isCompact: false
    }
}
