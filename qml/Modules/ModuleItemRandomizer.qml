import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick
import "../Widgets"

WidgetCommonTab {
    id: root
    value_context: config.itemRandomizer
    enabled_key: config.itemRandomizer.enabled
    caption: qsTr("Item Randomizer")
    tooltip: qsTr("What item randomizer does in short - it reads all possible item properties from Uniques, Sets, etc,<br>And then reassign properties back, but in random order (also it does not mean every original will be used)<br>For details, check descriptions of every option.")

    WidgetPreset {
        model: [qsTr("Select preset..."), qsTr("I want to be overpowered machine!"), qsTr("I want crazy 9000+"), qsTr("Want to have fresh experience but balanced if possible"),]
        presetIds: ["", "crazy", "strong", "weak"]
        value_key: config.itemRandomizer
    }

    WidgetSlider {
        caption: qsTr("Buff level range")
        tooltip: qsTr("Crazyness level - determine level difference to be used when selecting new properties for item/rune/etc.<br>For example, with value '10', new item can be 10 levels higher in terms of used properties.<br>That does not affect item level or its requirements.")
        value_key: root.value_context.levelPlus
        isCompact: true
        stretchCaption: true
    }
    WidgetSlider {
        caption: qsTr("Nerf level range")
        tooltip: qsTr("This is just as previous option, but forces item to be sometimes worse than original.")
        value_key: root.value_context.levelMinus
        isCompact: true
        stretchCaption: true
    }

    WidgetSlider {
        caption: qsTr("Item fit")
        tooltip: qsTr("Item fit slider allow you to select how much item affixes will be related to original item type.<br>For example, if you choose 80%, then 4 of 5 affixes will be selected to pool for specific item type<br>Item can have have several pools related to its type - say, scepter is a rod and a melee weapon.<br>Item type-specific properties will be picked in proportion to all types.")
        value_key: root.value_context.itemFit
        isCompact: true
        stretchCaption: true
    }

    WidgetSlider {
        caption: qsTr("Caster items")
        tooltip: qsTr("How many generated items will be FORCED caster-specific.<br>Note that some item bases like sorc orbs already force item to be caster (not melee damage rolls)")
        value_key: root.value_context.forceCasterType
        isCompact: true
        stretchCaption: true
    }

    WidgetSlider {
        caption: qsTr("Number of versions of each unique")
        tooltip: qsTr("allow you to have different uniques with same name and level, but different properties,<br>you will have N different uniques with differnet stats;<br>so you have an opportunity to pick same item again to check it out.<br>This works only with Uniques, not Sets.")
        value_key: root.value_context.repeatUniques
        isCompact: true
        stretchCaption: true
    }

    WidgetSlider {
        caption: qsTr("Number of different incompatible Sets")
        tooltip: qsTr("Whis will create additional sets (Angelic 2, Angelic 3) with different properties.")
        value_key: root.value_context.repeatSets
        isCompact: true
        stretchCaption: true
    }

    RowLayout {
        ColumnLayout {
            WidgetCheckbox {
                caption: qsTr("Randomize magix/rare affixes")
                value_key: root.value_context.affixRandom
            }

            WidgetCheckbox {
                caption: qsTr("Randomize gem and runes properties")
                value_key: root.value_context.gemsRandom
            }
            WidgetCheckbox {
                caption: qsTr("Randomize Cube crafted affixes")
                value_key: root.value_context.craftedRandom
            }
            WidgetCheckbox {
                caption: qsTr("Update existing properties")
                tooltip: qsTr("Properties that are selected to remain will be updated with new value range.")
                value_key: root.value_context.updateExisting
            }
        }
        ColumnLayout {
            WidgetCheckbox {
                caption: qsTr("Replace skills with oskills")
                tooltip: qsTr("Skill means: +SomeSkill (Class only)<br>Oskill means 'open' skill, +SomeSkill with no restriction.<br>Note that same class can only have up to +3 from oskill bonus.<br>For example, Trang set give sorceress only +3 to fireball.")
                value_key: root.value_context.replaceSkills
            }
            WidgetCheckbox {
                caption: qsTr("Replace charges with oskills")
                tooltip: ""
                value_key: root.value_context.replaceCharges
            }

            WidgetCheckbox {
                caption: qsTr("Remove Knockback/Monster flee")
                tooltip: ""
                value_key: root.value_context.removeKnock
            }
            WidgetCheckbox {
                caption: qsTr("Make all Uniques have equal rarity on same base")
                tooltip: qsTr("Now Uniques with equal item base will have equal chance to drop.<br>For example Tyrael's and Templar's will have equal chance. (and all rings too)")
                value_key: root.value_context.sameRarityForSetUniqueItems
            }
        }
    }

    WidgetComboBox {
        isCompact: true
        caption: qsTr("Set items partial bonus")
        tooltip: qsTr("How generated items will use partial set bonuses:<br>Keep original - Existing item logic is preserved;<br>No partial - space for extra attributes will used for 'Blue' properties<br>Use partial - extra set attributes will require other pieces, 'Green' props.")
        model: [qsTr("Keep original"), qsTr("No partial"), qsTr("Use partial")]
        value_key: root.value_context.setBonusMode
    }

    Rectangle {
        Layout.fillWidth: true
        Layout.preferredHeight: 1
        color: palette.accent
    }
    Label {
        text: qsTr("Adjust amount of different attribute rolls:")
    }

    WidgetSlider {
        caption: qsTr("Resistances:")
        tooltip: ""
        value_key: root.value_context.frequencyResistances
        isCompact: true
        stretchCaption: true
    }
    WidgetSlider {
        caption: qsTr("Aura when equip:")
        tooltip: ""
        value_key: root.value_context.frequencyAuras
        isCompact: true
        stretchCaption: true
    }
    WidgetSlider {
        caption: qsTr("On hit/attack/struck procs:")
        tooltip: ""
        value_key: root.value_context.frequencyProcs
        isCompact: true
        stretchCaption: true
    }
    WidgetSlider {
        caption: qsTr("+skills:")
        tooltip: ""
        value_key: root.value_context.frequencySkills
        isCompact: true
        stretchCaption: true
    }
    WidgetSlider {
        caption: qsTr("Stats - life/mana/attributes:")
        tooltip: ""
        value_key: root.value_context.frequencyCharAttributes
        isCompact: true
        stretchCaption: true
    }
}
