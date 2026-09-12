import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick

AppCommonTab {
    id: root
    value_context: "challenge"
    caption: qsTr("Challenge")
    tooltip: qsTr("First, you can disable potion drops entirely (that's not the same as drop filter).<br>Second, you can change resistance penalty for each difficulty from default 0/40/100.<br>And the last, you can adjust level area on all maps; <br>note that max value of 85 is still used (so set to 20 to basically make all Hell areas lvl 85).")

    AppCheckbox {
        caption: qsTr("Disable drop: All Health pots")
        tooltip: ""
        value_context: root.value_context
        value_key: "nodrop_hpsa"
    }

    AppCheckbox {
        caption: qsTr("Disable drop: All Mana pots")
        tooltip: ""
        value_context: root.value_context
        value_key: "nodrop_mpsa"
    }

    AppCheckbox {
        caption: qsTr("Disable drop: Rejuv and Full Rejuv pots")
        tooltip: ""
        value_context: root.value_context
        value_key: "nodrop_rvs"
    }

    AppSlider {
        caption: qsTr("Normal difficulty resistance penalty, -all%")
        tooltip: ""
        from: 0
        to: 250
        value_context: root.value_context
        value_key: "normal_minus_res"
        isCompact: false
    }

    AppSlider {
        caption: qsTr("Nightmare difficulty resistance penalty, -all%")
        tooltip: ""
        from: 0
        to: 250
        value_context: root.value_context
        value_key: "nightmare_minus_res"
        isCompact: false
    }

    AppSlider {
        caption: qsTr("Hell difficulty resistance penalty, -all%")
        tooltip: ""
        from: 0
        to: 250
        value_context: root.value_context
        value_key: "hell_minus_res"
        isCompact: false
    }

    AppSlider {
        caption: qsTr("Increase Nightmare area levels, +levels")
        tooltip: ""
        from: 0
        to: 20
        value_context: root.value_context
        value_key: "levelIncreaseNightmare"
        isCompact: false
    }

    AppSlider {
        caption: qsTr("Increase Hell area levels, +levels")
        tooltip: ""
        from: 0
        to: 30
        value_context: root.value_context
        value_key: "levelIncreaseHell"
        isCompact: false
    }

    AppCheckbox {
        caption: qsTr("Go beyond 85 level for areas")
        tooltip: ""
        value_context: root.value_context
        value_key: "levelIncreaseUltra"
    }
}
