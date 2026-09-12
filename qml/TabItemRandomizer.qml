import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick

WidgetCommonTab {
    id: root
    value_context: "randomizer"
    caption: qsTr("Item Randomizer")
    tooltip: qsTr("What item randomizer does in short - it reads all possible item properties from Uniques, Sets, etc,<br>And then reassign properties back, but in random order (also it does not mean every original will be used)<br>For details, check descriptions of every option.")

    WidgetPreset {
        model: [qsTr("Select preset..."), qsTr("I want to be overpowered machine!"), qsTr("Want to have some fun without flying to space"), qsTr("Want to have fresh experience but balanced if possible"),]
        value_context: root.value_context
    }

    WidgetSlider {
        caption: qsTr("Crazy-ness (or 'NON-balance level', lower = more balance, 100=chaos)")
        tooltip: qsTr("Crazyness level - determine level difference to be used when selecting new properties for item/rune/etc.<br>With '10' it will select between level-10 and level+10 at first, if there are <50 candidates,<br>then it will select level-30..level+30, and finally it will try fully random. <br>In short, lower value = more balance in terms of original affix level and item level.")
        from: 5
        to: 100
        value_context: root.value_context
        value_key: "crazyLevel"
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("Item type fit percent (0% = fully random, 100% = all according to item type)")
        tooltip: qsTr("Item fit slider allow you to select how much item affixes will be related to original item type.<br>For example, if you choose 80%, then 4 of 5 affixes will be selected to pool for specific item type<br>Item can have have several pools related to its type - say, scepter is a rod and a melee weapon.<br>Item type-specific properties will be picked in proportion to all types.")
        from: 0
        to: 100
        value_context: root.value_context
        value_key: "itemFitPercent"
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("How many original properties to keep, percent")
        tooltip: qsTr("You can select how many properties of original item you want to keep.<br>If 0, then every item will be fully randomized.<br>If 50, then half of genereted properties will be original, and half randomized.<br>If 100, then every property will be property of original item. (you can reduce an amount of props)")
        from: 0
        to: 100
        value_context: root.value_context
        value_key: "keepOriginalPercent"
        isCompact: false
    }

    Label {
        text: qsTr("Relative property counts (min/max), compared to non-randomized original:")
    }

    WidgetExponentialSlider {
        caption: qsTr("Uniques/Gems/Runes/Affixes, Min")
        tooltip: qsTr("This and the next options determine new property count will be after generation.<br>If Min=Max=100%%, then property count will be exactly as original (except rare corner cases).<br>If Min=50%% and Max=200%%, then new property count will be at least half as original, and twice at best.<br>For example, if item has 5 properties, then worst case is 2, and best case is 9 (because 9 is maximum for Uniques)")
        denom: 5
        mult: 10
        value_context: root.value_context
        value_key: "relativeCountMin"
        isCompact: true
        suffix: "%"
    }

    WidgetExponentialSlider {
        caption: qsTr("Uniques/Gems/Runes/Affixes, Max")
        tooltip: ""
        denom: 5
        mult: 10
        value_context: root.value_context
        value_key: "relativeCountMax"
        isCompact: true
        suffix: "%"
    }

    WidgetExponentialSlider {
        caption: qsTr("Set items, Min")
        tooltip: ""
        denom: 5
        mult: 10
        value_context: root.value_context
        value_key: "setRelativeCountMin"
        isCompact: true
        suffix: "%"
    }

    WidgetExponentialSlider {
        caption: qsTr("Set items, Max")
        tooltip: ""
        denom: 5
        mult: 10
        value_context: root.value_context
        value_key: "setRelativeCountMax"
        isCompact: true
        suffix: "%"
    }

    WidgetSlider {
        caption: qsTr("Number of versions of each unique")
        tooltip: qsTr("allow you to have different uniques with same name and level, but different properties,<br>you will have N different uniques with differnet stats;<br>so you have an opportunity to pick same item again to check it out.<br>This works only with Uniques, not Sets.")
        from: 1
        to: 20
        value_context: root.value_context
        value_key: "repeat_uniques"
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("Number of different incompatible Sets")
        tooltip: qsTr("Whis will create additional sets (Angelic 2, Angelic 3) with different properties.")
        from: 1
        to: 20
        value_context: root.value_context
        value_key: "repeat_sets"
        isCompact: false
    }

    WidgetCheckbox {
        caption: qsTr("Prevent duplicate properties on items")
        tooltip: ""
        value_context: root.value_context
        value_key: "noDuplicates"
    }

    WidgetCheckbox {
        caption: qsTr("Randomize magix/rare affixes")
        tooltip: qsTr("This will modify rare and magic suffixes - <br>so they can include properties of any other item in the game. <br>Note that their properties are read even without this option.")
        value_context: root.value_context
        value_key: "affixRandom"
    }

    WidgetCheckbox {
        caption: qsTr("Randomize gem and runes properties")
        tooltip: qsTr("This will modify gem and rune properties - <br>so they can include properties of any other item in the game. <br>Note that their properties are read even without this option.")
        value_context: root.value_context
        value_key: "gemsRandom"
    }

    WidgetCheckbox {
        caption: qsTr("Replace skills with oskills")
        tooltip: ""
        value_context: root.value_context
        value_key: "replaceSkills"
    }

    WidgetCheckbox {
        caption: qsTr("Replace charges with oskills")
        tooltip: ""
        value_context: root.value_context
        value_key: "replaceCharges"
    }

    WidgetCheckbox {
        caption: qsTr("Remove Knockback/Monster flee")
        tooltip: ""
        value_context: root.value_context
        value_key: "removeKnock"
    }
}
