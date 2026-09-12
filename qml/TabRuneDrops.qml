import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick

AppCommonTab {
    id: root
    value_context: "rune_drops"
    caption: qsTr("Increase Rune drops")
    tooltip: qsTr("All help and explanation on this page assume you are familiar with Treasure Class (TC) and NoDrop terminology.<br>Basically all options here modifying TC pick weights - so one preferred option become less rare.")

    AppSlider {
        caption: qsTr("Increase Rare Rune drops")
        tooltip: qsTr("That will drastically improve High Rune drop rates! You probably don&apos;t need more than 50x.<br>Value here increases chance of dropping Zod in 'Runes 17' TC<br>Rarity of other runes will change proportionally <br>(so High Runes still be more rare in the same manner).")
        from: 1
        to: 1000
        value_context: root.value_context
        value_key: "zod_factor"
        isCompact: false
    }

    AppSlider {
        caption: qsTr("Increase Runes chance in Good TC")
        tooltip: qsTr("When dropping from Good TC, that will make Runes drop more often <br>compared to other jewellery (Gems/Rings/Amulets/Charms)")
        from: 1
        to: 10
        value_context: root.value_context
        value_key: "rune_factor"
        isCompact: false
    }

    AppCheckbox {
        caption: qsTr("Switch (Ber,Jah) with (Cham,Zod) in rarity")
        tooltip: qsTr("When game desides to drop Jah rune, it drops Zod instead. And vice versa.")
        value_context: root.value_context
        value_key: "highrune_switch"
    }

    AppCheckbox {
        caption: qsTr("Make Countess Drop higher runes (up to Runes 17=Zod on Hell)")
        tooltip: ""
        value_context: root.value_context
        value_key: "countess_rune_higher"
    }

    AppCheckbox {
        caption: qsTr("Increase Countess Runes drop count to 5")
        tooltip: ""
        value_context: root.value_context
        value_key: "countess_rune_more"
    }

    AppCheckbox {
        caption: qsTr("Increase Wraiths Runes drop")
        tooltip: ""
        value_context: root.value_context
        value_key: "wraith_runes"
    }
}
