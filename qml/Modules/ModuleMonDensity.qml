import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick
import "../Widgets"

WidgetCommonTab {
    id: root
    value_context: config.moduleMonDensity
    enabled_key: config.enabledModules.moduleMonDensity
    caption: qsTr("Monster density")
    tooltip: qsTr("Density slider - that will affect increase of density of regular monsters <br>(note that it is higly dependant on map and tiles; max density can be reached on open area tiles.)<br>Boss packs slider icrease number of Champion/Uniques in zones; again, that also not a precise setting.<br>Last Checkbox can be used to quickly setup boss count on Normal and Nightmare exactly equal to Hell count.")

    WidgetRangeSlider {
        caption: qsTr("Increase density, times")
        tooltip: ""
        value_key: root.value_context.density
        isCompact: false
    }

    WidgetRangeSlider {
        caption: qsTr("Increase Boss packs count, times")
        tooltip: ""
        value_key: root.value_context.packs
        isCompact: false
    }

    WidgetRangeSlider {
        caption: qsTr("Increase monster groups population, +count<br><b>Beware! This setting have the most impact on getting screen laggy!<b><br><b>Having value above +3 will work well only if previous options are not maxed!</b>")
        tooltip: ""
        value_key: root.value_context.mon_groups
        isCompact: false
    }

    WidgetCheckbox {
        caption: qsTr("Use Hell elite pack count on Nightmare and Normal")
        tooltip: ""
        value_key: root.value_context.hellPacks
    }
}
