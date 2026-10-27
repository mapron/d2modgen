import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick

WidgetCommonTab {
    id: root
    value_context: appModuleItemRandomizer
    caption: qsTr("Item Randomizer")
    tooltip: qsTr("What item randomizer does in short - it reads all possible item properties from Uniques, Sets, etc,<br>And then reassign properties back, but in random order (also it does not mean every original will be used)<br>For details, check descriptions of every option.")

    WidgetPreset {
        model: [qsTr("Select preset..."), qsTr("I want to be overpowered machine!"), qsTr("I want crazy 9000+"), qsTr("Want to have fresh experience but balanced if possible"),]
        value_context: root.value_context
    }

    WidgetRangeSlider {
        caption: qsTr("Buff level range")
        tooltip: qsTr("Crazyness level - determine level difference to be used when selecting new properties for item/rune/etc.<br>For example, with value '10', new item can be 10 levels higher in terms of used properties.<br>That does not affect item level or its requirements.")
        from: 0
        to: 100
        value_context: root.value_context
        value_key: "levelPlus"
        isCompact: false
    }
    WidgetRangeSlider {
        caption: qsTr("Nerf level range")
        tooltip: qsTr("This is just as previous option, but forces item to be sometimes worse than original.")
        from: 0
        to: 100
        value_context: root.value_context
        value_key: "levelMinus"
        isCompact: false
    }

    WidgetRangeSlider {
        caption: qsTr("Item type fit percent (0% = fully random, 100% = all according to item type)")
        tooltip: qsTr("Item fit slider allow you to select how much item affixes will be related to original item type.<br>For example, if you choose 80%, then 4 of 5 affixes will be selected to pool for specific item type<br>Item can have have several pools related to its type - say, scepter is a rod and a melee weapon.<br>Item type-specific properties will be picked in proportion to all types.")
        from: 0
        to: 100
        value_context: root.value_context
        value_key: "itemFitPercent"
        isCompact: false
    }

    WidgetRangeSlider {
        caption: qsTr("Caster items percent")
        tooltip: qsTr("How many generated items will be caster-specific.")
        from: 0
        to: 100
        value_context: root.value_context
        value_key: "casterTypePercent"
        isCompact: false
    }

    WidgetRangeSlider {
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
        stretchCaption: true
    }

    WidgetExponentialSlider {
        caption: qsTr("Uniques/Gems/Runes/Affixes, Max")
        tooltip: ""
        denom: 5
        mult: 10
        value_context: root.value_context
        value_key: "relativeCountMax"
        isCompact: true
        stretchCaption: true
    }

    WidgetExponentialSlider {
        caption: qsTr("Set items, Min")
        tooltip: ""
        denom: 5
        mult: 10
        value_context: root.value_context
        value_key: "setRelativeCountMin"
        isCompact: true
        stretchCaption: true
    }

    WidgetExponentialSlider {
        caption: qsTr("Set items, Max")
        tooltip: ""
        denom: 5
        mult: 10
        value_context: root.value_context
        value_key: "setRelativeCountMax"
        isCompact: true
        stretchCaption: true
    }

    WidgetRangeSlider {
        caption: qsTr("Number of versions of each unique")
        tooltip: qsTr("allow you to have different uniques with same name and level, but different properties,<br>you will have N different uniques with differnet stats;<br>so you have an opportunity to pick same item again to check it out.<br>This works only with Uniques, not Sets.")
        from: 1
        to: 5
        value_context: root.value_context
        value_key: "repeat_uniques"
        isCompact: false
    }

    WidgetRangeSlider {
        caption: qsTr("Number of different incompatible Sets")
        tooltip: qsTr("Whis will create additional sets (Angelic 2, Angelic 3) with different properties.")
        from: 1
        to: 5
        value_context: root.value_context
        value_key: "repeat_sets"
        isCompact: false
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

    WidgetCheckbox {
        caption: qsTr("Update existing properties")
        tooltip: qsTr("Properties that are selected to remain will be updated with new value range.")
        value_context: root.value_context
        value_key: "updateExisting"
    }

    WidgetComboBox {
        isCompact: true
        caption: qsTr("Set items partial bonus")
        tooltip: qsTr("How generated items will use partial set bonuses:<br>Keep original - Existing item logic is preserved;<br>No partial - space for extra attributes will used for 'Blue' properties<br>Use partial - extra set attributes will require other pieces, 'Green' props.")
        model: [qsTr("Keep original"), qsTr("No partial"), qsTr("Use partial")]
        value_context: root.value_context
        value_key: "setBonusMode"
    }

    Rectangle {
        Layout.fillWidth: true
        Layout.preferredHeight: 1
        color: palette.accent
    }
    Label {
        text: qsTr("Adjust amount of different attribute rolls:")
    }

    WidgetExponentialSlider {
        caption: qsTr("Resistances:")
        tooltip: ""
        denom: 20
        mult: 10
        value_context: root.value_context
        value_key: "frequencyResistances"
        isCompact: true
        stretchCaption: true
    }
    WidgetExponentialSlider {
        caption: qsTr("Aura when equip:")
        tooltip: ""
        denom: 20
        mult: 10
        value_context: root.value_context
        value_key: "frequencyAuras"
        isCompact: true
        stretchCaption: true
    }
    WidgetExponentialSlider {
        caption: qsTr("On hit/attack/struck procs:")
        tooltip: ""
        denom: 20
        mult: 10
        value_context: root.value_context
        value_key: "frequencyProcs"
        isCompact: true
        stretchCaption: true
    }
    WidgetExponentialSlider {
        caption: qsTr("+skills:")
        tooltip: ""
        denom: 20
        mult: 10
        value_context: root.value_context
        value_key: "frequencySkills"
        isCompact: true
        stretchCaption: true
    }
    WidgetExponentialSlider {
        caption: qsTr("Stats - life/mana/attributes:")
        tooltip: ""
        denom: 20
        mult: 10
        value_context: root.value_context
        value_key: "frequencyCharAttributes"
        isCompact: true
        stretchCaption: true
    }
}
