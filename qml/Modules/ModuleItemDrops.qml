import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick
import "../Widgets"

WidgetCommonTab {
    id: root
    value_context: config.treasureClasses.items
    enabled_key: config.treasureClasses.items.enabled
    caption: qsTr("Increase Item drops")
    tooltip: qsTr("All help and explanation on this page assume you are familiar with Treasure Class (TC) and NoDrop terminology.<br>Basically all options here modifying TC pick weights - so one preferred option become less rare.<br>First 3 sliders are for increasing chance for Uniques/Sets/Rares. <br>Note that increase is accurate when your chances are low, but with high MF it can have diminishing return.")

    WidgetPreset {
        model: [qsTr("Select preset..."), qsTr("I want to find Tyrael's today!"), qsTr("Just tired of finding my first Shako"), qsTr("A little bit more of good stuff, not crazy"),]
        value_key: config.treasureClasses.items
    }

    WidgetSlider {
        caption: qsTr("Increase Unique Chance")
        tooltip: ""
        value_key: root.value_context.uniqueFactorIncrease
        isCompact: true
        stretchCaption: true
    }

    WidgetSlider {
        caption: qsTr("Increase Set Chance")
        tooltip: ""
        value_key: root.value_context.setFactorIncrease
        isCompact: true
        stretchCaption: true
    }

    WidgetSlider {
        caption: qsTr("Increase Rare Chance")
        tooltip: ""
        value_key: root.value_context.rareFactorIncrease
        isCompact: true
        stretchCaption: true
    }

    WidgetSlider {
        caption: qsTr("Increase Quality (Rare/Set/Unique) for non-elites")
        tooltip: ""
        value_key: root.value_context.noneliteFactorIncrease
        isCompact: true
        stretchCaption: true
    }

    WidgetSlider {
        caption: qsTr("Increase Chance of High-level equipment")
        tooltip: qsTr("Usually when level 85 monster drops an item,<br> there is tiny chance that equpment will be elite base.<br>This increases chance for high-level monster drop high-level base.")
        value_key: root.value_context.chanceForHighLevel
        isCompact: true
        stretchCaption: true
    }

    WidgetSlider {
        caption: qsTr("Champion dropped item count")
        value_key: root.value_context.championItemCount
        isCompact: true
        stretchCaption: true
    }

    WidgetSlider {
        caption: qsTr("Unique dropped item count")
        value_key: root.value_context.uniqueItemCount
        isCompact: true
        stretchCaption: true
    }

    WidgetSlider {
        caption: qsTr("SuperUnique dropped item count")
        value_key: root.value_context.superItemCount
        isCompact: true
        stretchCaption: true
    }

    WidgetCheckbox {
        caption: qsTr("Always quest drops from bosses")
        tooltip: ""
        value_key: root.value_context.forceQuestDrops
    }
    WidgetCheckbox {
        caption: qsTr("Remove gold from Champion drops")
        tooltip: ""
        value_key: root.value_context.removeChampGold
    }
    WidgetCheckbox {
        caption: qsTr("Remove all potions from elites")
        tooltip: ""
        value_key: root.value_context.removeElitePotions
    }

    Label {
        text: qsTr("Adjust proportion of main drop classes")
    }

    WidgetSlider {
        caption: qsTr("Increse drop count (via NoDrop)")
        tooltip: qsTr("This provides ability to gradually reduce NoDrop picks<br>(it basically similar to increasing players count in the game)<br>This only affect base monster drops")
        value_key: root.value_context.inverseNodropPercent
        isCompact: true
        stretchCaption: true
    }

    WidgetSlider {
        caption: qsTr("Good TC (Runes/Gems/Jewellery)")
        tooltip: qsTr("That will make Gems/Runes/Rings/Amulets/Charms drop far more often<br>compared to equipment (armor/weapons).")
        value_key: root.value_context.goodPercent
        isCompact: true
        stretchCaption: true
    }

    WidgetSlider {
        caption: qsTr("Equipment (armor/weapons)")
        tooltip: qsTr("Relative chance of equipment dropping compared to other items (or NoDrop).")
        value_key: root.value_context.equipPercent
        isCompact: true
        stretchCaption: true
    }

    WidgetSlider {
        caption: qsTr("Gold (higher=MORE gold)")
        tooltip: qsTr("Relative chance of gold compared to other items.")
        value_key: root.value_context.goldPercent
        isCompact: true
        stretchCaption: true
    }

    WidgetSlider {
        caption: qsTr("Junk (keys/bolts/etc) (higher=MORE junk)")
        tooltip: qsTr("Relative chance of junk items compared to other items.")
        value_key: root.value_context.junkPercent
        isCompact: true
        stretchCaption: true
    }

    WidgetCheckbox {
        caption: qsTr("Make all Uniques have equal rarity on same base")
        tooltip: qsTr("Now Uniques with equal item base will have equal chance to drop.<br>For example Tyrael's and Templar's will have equal chance. (and all rings too)")
        value_key: root.value_context.sameRarityForSetUniqueItems
    }
}
