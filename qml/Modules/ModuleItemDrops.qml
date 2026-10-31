import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick
import "../Widgets"

WidgetCommonTab {
    id: root
    value_context: config.moduleItemDrops
    enabled_key: config.enabledModules.moduleItemDrops
    caption: qsTr("Increase Item drops")
    tooltip: qsTr("All help and explanation on this page assume you are familiar with Treasure Class (TC) and NoDrop terminology.<br>Basically all options here modifying TC pick weights - so one preferred option become less rare.<br>First 3 sliders are for increasing chance for Uniques/Sets/Rares. <br>Note that increase is accurate when your chances are low, but with high MF it can have diminishing return.")

    WidgetPreset {
        model: [qsTr("Select preset..."), qsTr("I want to find Tyrael's today!"), qsTr("Just tired of finding my first Shako"), qsTr("A little bit more of good stuff, not crazy"),]
        value_key: config.moduleItemDrops
    }

    WidgetRangeSlider {
        caption: qsTr("Increase Unique Chance")
        tooltip: ""
        value_key: root.value_context.chance_uni
        isCompact: true
        stretchCaption: true
    }

    WidgetRangeSlider {
        caption: qsTr("Increase Set Chance")
        tooltip: ""
        value_key: root.value_context.chance_set
        isCompact: true
        stretchCaption: true
    }

    WidgetRangeSlider {
        caption: qsTr("Increase Rare Chance")
        tooltip: ""
        value_key: root.value_context.chance_rare
        isCompact: true
        stretchCaption: true
    }

    WidgetRangeSlider {
        caption: qsTr("Increase Chance of High-level equipment")
        tooltip: qsTr("This will reduce the reduce the chance that equipment of lower level will drop as a fallback.<br>For example, for level 85 TC there is a 98% chance that it will drop from 84 level TC, and so on.")
        value_key: root.value_context.chance_highlevel
        isCompact: true
        stretchCaption: true
    }

    WidgetCheckbox {
        caption: qsTr("Increase Champion/Unique/Superunique dropped item count")
        tooltip: qsTr("Change item+potion ratio:<br>Champion: 2 items + 2 potions<br>Unique: 4 items + 2 potions<br>Superuniques: 6 items + 0 potions")
        value_key: root.value_context.high_elite_drops
    }

    WidgetCheckbox {
        caption: qsTr("Always quest drops from bosses")
        tooltip: ""
        value_key: root.value_context.boss_quest_drops
    }

    Label {
        text: qsTr("Adjust proportion of main drop classes")
    }

    WidgetExponentialSlider {
        caption: qsTr("Increse drop count (via NoDrop)")
        tooltip: qsTr("This provides ability to gradually reduce NoDrop picks<br>(it basically similar to increasing players count in the game)<br>This only affect base monster drops")
        value_key: root.value_context.drop_percent
        isCompact: true
        stretchCaption: true
    }

    WidgetExponentialSlider {
        caption: qsTr("Good TC (Runes/Gems/Jewellery)")
        tooltip: qsTr("That will make Gems/Runes/Rings/Amulets/Charms drop far more often<br>compared to equipment (armor/weapons).")
        value_key: root.value_context.good_percent
        isCompact: true
        stretchCaption: true
    }

    WidgetExponentialSlider {
        caption: qsTr("Equipment (armor/weapons)")
        tooltip: qsTr("Relative chance of equipment dropping compared to other items (or NoDrop).")
        value_key: root.value_context.equip_percent
        isCompact: true
        stretchCaption: true
    }

    WidgetExponentialSlider {
        caption: qsTr("Gold (higher=MORE gold)")
        tooltip: qsTr("Relative chance of gold compared to other items.")
        value_key: root.value_context.gold_percent
        isCompact: true
        stretchCaption: true
    }

    WidgetExponentialSlider {
        caption: qsTr("Junk (keys/bolts/etc) (higher=MORE junk)")
        tooltip: qsTr("Relative chance of junk items compared to other items.")
        value_key: root.value_context.junk_percent
        isCompact: true
        stretchCaption: true
    }

    WidgetCheckbox {
        caption: qsTr("Make all Uniques have equal rarity on same base")
        tooltip: qsTr("Now Uniques with equal item base will have equal chance to drop.<br>For example Tyrael's and Templar's will have equal chance. (and all rings too)")
        value_key: root.value_context.equal_uniques
    }
}
