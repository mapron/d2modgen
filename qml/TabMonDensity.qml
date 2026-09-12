import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick

WidgetCommonTab {
    id: root
    value_context: "density"
    caption: qsTr("Monster density")
    tooltip: qsTr("Density slider - that will affect increase of density of regular monsters <br>(note that it is higly dependant on map and tiles; max density can be reached on open area tiles.)<br>Boss packs slider icrease number of Champion/Uniques in zones; again, that also not a precise setting.<br>Last Checkbox can be used to quickly setup boss count on Normal and Nightmare exactly equal to Hell count.")

    WidgetSlider {
        caption: qsTr("Increase density, times")
        tooltip: ""
        from: 1
        to: 20
        value_context: root.value_context
        value_key: "density"
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("Increase Boss packs count, times")
        tooltip: ""
        from: 1
        to: 20
        value_context: root.value_context
        value_key: "packs"
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("Increase monster groups population, +count<br><b>Beware! This setting have the most impact on getting screen laggy!<b><br><b>Having value above +3 will work well only if previous options are not maxed!</b>")
        tooltip: ""
        from: 0
        to: 20
        value_context: root.value_context
        value_key: "mon_groups"
        isCompact: false
    }

    WidgetCheckbox {
        caption: qsTr("Use Hell elite pack count on Nightmare and Normal")
        tooltip: ""
        value_context: root.value_context
        value_key: "hellPacks"
    }
}
