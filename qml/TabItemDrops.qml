import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick

WidgetCommonTab {
    id: root
    value_context: "item_drops"
    caption: qsTr("Increase Item drops")
    tooltip: qsTr("All help and explanation on this page assume you are familiar with Treasure Class (TC) and NoDrop terminology.<br>Basically all options here modifying TC pick weights - so one preferred option become less rare.<br>First 3 sliders are for increasing chance for Uniques/Sets/Rares. <br>Note that increase is accurate when your chances are low, but with high MF it can have diminishing return.")

    WidgetSlider {
        caption: qsTr("Increase Unique Chance")
        tooltip: ""
        from: 1
        to: 50
        value_context: root.value_context
        value_key: "chance_uni"
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("Increase Set Chance")
        tooltip: ""
        from: 1
        to: 30
        value_context: root.value_context
        value_key: "chance_set"
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("Increase Rare Chance")
        tooltip: ""
        from: 1
        to: 15
        value_context: root.value_context
        value_key: "chance_rare"
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("Increase Chance of High-level equipment")
        tooltip: qsTr("This will reduce the reduce the chance that equipment of lower level will drop as a fallback.<br>For example, for level 85 TC there is a 98% chance that it will drop from 84 level TC, and so on.")
        from: 1
        to: 15
        value_context: root.value_context
        value_key: "chance_highlevel"
        isCompact: false
    }

    WidgetCheckbox {
        caption: qsTr("Increase Champion/Unique item count")
        tooltip: qsTr("This will make Unique bosses to drop 4 items instead of just 1, and Champions drop 2 items instead of one.<br>Note that potion drops are slightly reduced.")
        value_context: root.value_context
        value_key: "high_elite_drops"
    }

    WidgetCheckbox {
        caption: qsTr("Always quest drops from bosses")
        tooltip: ""
        value_context: root.value_context
        value_key: "boss_quest_drops"
    }

    Label {
        text: qsTr("Adjust proportion of main drop classes")
    }

    WidgetExponentialSlider {
        caption: qsTr("NoDrop (higher=LESS drops)")
        tooltip: qsTr("NoDrop slider provides ability to gradually reduce NoDrop picks<br>(it basically similar to increasing players count in the game)")
        denom: 20
        mult: 5
        value_context: root.value_context
        value_key: "nodrop_percent"
        isCompact: true
        suffix: "%"
    }

    WidgetExponentialSlider {
        caption: qsTr("Good TC (Runes/Gems/Jewellery)")
        tooltip: qsTr("That will make Gems/Runes/Rings/Amulets/Charms drop far more often<br>compared to equipment (armor/weapons).")
        denom: 2
        mult: 10
        value_context: root.value_context
        value_key: "good_percent"
        isCompact: true
        suffix: "%"
    }

    WidgetExponentialSlider {
        caption: qsTr("Equipment (armor/weapons)")
        tooltip: qsTr("Relative chance of equipment dropping compared to other items (or NoDrop).")
        denom: 5
        mult: 10
        value_context: root.value_context
        value_key: "equip_percent"
        isCompact: true
        suffix: "%"
    }

    WidgetExponentialSlider {
        caption: qsTr("Gold (higher=MORE gold)")
        tooltip: qsTr("Relative chance of gold compared to other items.")
        denom: 20
        mult: 10
        value_context: root.value_context
        value_key: "gold_percent"
        isCompact: true
        suffix: "%"
    }

    WidgetExponentialSlider {
        caption: qsTr("Junk (keys/bolts/etc) (higher=MORE junk)")
        tooltip: qsTr("Relative chance of junk items compared to other items.")
        denom: 20
        mult: 10
        value_context: root.value_context
        value_key: "junk_percent"
        isCompact: true
        suffix: "%"
    }

    WidgetCheckbox {
        caption: qsTr("Make all Uniques have equal rarity on same base")
        tooltip: qsTr("Now Uniques with equal item base will have equal chance to drop.<br>For example Tyrael's and Templar's will have equal chance. (and all rings too)")
        value_context: root.value_context
        value_key: "equal_uniques"
    }
}
