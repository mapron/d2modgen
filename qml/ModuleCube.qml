import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick

WidgetCommonTab {
    id: root
    value_context: appModuleCube
    caption: qsTr("Horadric Cube")
    tooltip: qsTr("Create some recipes to cheese/cut corners around the game. <br>Input and results are self-explanatory.")

    WidgetCheckbox {
        caption: qsTr("Remove gem component from Rune upgrade recipes (Rune x3,x2 => next Rune)")
        tooltip: ""
        value_context: root.value_context
        value_key: "noGemUpgrade"
    }

    WidgetCheckbox {
        caption: qsTr("Add quick portal access recipes")
        tooltip: qsTr("1. TP book + Id scroll = Cow Portal<br>2. TP book + Id scroll x2 = Minor Uber Portal<br>3. TP book + Id scroll x3 = Uber Tristram Portal<br>4. TP book + Id scroll x4 = Colossal Ancients Portal")
        value_context: root.value_context
        value_key: "quickPortals"
    }

    WidgetCheckbox {
        caption: qsTr("Add quick quest recipes (Horadric Staff + Khalim Will)")
        tooltip: qsTr("1. key + Id scroll = Horadric Staff<br>2. key + TP scroll = Khalim Will")
        value_context: root.value_context
        value_key: "quickQuests"
    }

    WidgetCheckbox {
        caption: qsTr("Add socketing recipes")
        tooltip: qsTr("1. Normal item + TP scroll x1 + Id scroll x1 = Add 3 sockets<br>2. Normal item + TP scroll x1 + Id scroll x2 = Add 4 sockets<br>3. Normal item + TP scroll x2 + Id scroll x1 = Add 5 sockets<br>4. Normal item + TP scroll x2 + Id scroll x2 = Add 6 sockets<br>5. Unique/Set/Rare item + TP scroll x1 + Id scroll x1 = Add 1 socket<br>6. Socketed item + TP scroll x1 = Clear sockets")
        value_context: root.value_context
        value_key: "socketing"
    }

    WidgetCheckbox {
        caption: qsTr("Add quality upgrade recipes")
        tooltip: qsTr("1. Normal item (normal,magic,rare,unique,set) + Antidote = Exceptional item<br>2. Exceptional item (normal,magic,rare,unique,set) + Antidote = Elite item<br>3. Magic (magic,rare,unique,set) item  + Stamina = Normal item of same type")
        value_context: root.value_context
        value_key: "upgrading"
    }

    WidgetCheckbox {
        caption: qsTr("Add cheat Small Charm recipies")
        tooltip: qsTr("Any small charm + one of (TP scroll x1, Id scroll x1 , key x1) + one of (h pot, mana pot, stamina, antidote, thawing) = crafted char (see below all recipes)<br>Just two charms of each kind suppose to provide reasonable endgame stat.<br>Item level is 1, so try several times to craft charm with 0 requirements.") + //
        "<br>SC + key + stamina = all skills + all stats" + //
        "<br>SC + key + antidote = damage reduction" + //
        "<br>SC + key + thawing = absorb" + //
        "<br>SC + key + any hp = HP and vitality" + //
        "<br>SC + key + any mana = Mana and energy" + //
        "<br>SC + TP + stamina = faster frames" + //
        "<br>SC + TP + antidote = max res + poison len" + //
        "<br>SC + TP + thawing = all res" + //
        "<br>SC + TP + any hp = hp regen" + //
        "<br>SC + TP + any mana = mana regen" + //
        "<br>SC + ID + stamina = pierce res" + //
        "<br>SC + ID + antidote = +elemental dmg" + //
        "<br>SC + ID + thawing = OW/CB/DS" + //
        "<br>SC + ID + any hp = IAS and Enh Dmg" + //
        "<br>SC + ID + any mana = attack rating and defence"

        value_context: root.value_context
        value_key: "cheatCharms"
    }

    Label {
        text: qsTr("Simplyfy crafting:")
    }

    WidgetCheckbox {
        caption: qsTr("Remove strict item types for Crafted")
        tooltip: qsTr("Now any kind of gloves will be suitable, not specific type of gloves.<br>Same for helm, shield, weapon.")
        value_context: root.value_context
        value_key: "craftNoStrict"
    }

    WidgetCheckbox {
        caption: qsTr("Remove Rune/Jewel for Crafted")
        tooltip: qsTr("So any craft recipe is 'item + gem'")
        value_context: root.value_context
        value_key: "craftNoRunes"
    }

    WidgetCheckbox {
        caption: qsTr("Make Item Level always 99")
        tooltip: ""
        value_context: root.value_context
        value_key: "craftHighIlvl"
    }
}
