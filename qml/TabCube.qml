import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick

AppCommonTab {
    id: root
    value_context: "cube"
    caption: qsTr("Horadric Cube")
    tooltip: qsTr("Create some recipes to cheese/cut corners around the game. <br>Input and results are self-explanatory.")

    AppCheckbox {
        caption: qsTr("Remove gem component from Rune upgrade recipes (Rune x3,x2 =&gt; next Rune)")
        tooltip: ""
        value_context: root.value_context
        value_key: "noGemUpgrade"
    }

    AppCheckbox {
        caption: qsTr("Add quick portal access recipes:<br>1. TP book + Id scroll = Cow Portal<br>2. TP book + Id scroll x2 = Minor Uber Portal<br>3. TP book + Id scroll x3 = Uber Tristram Portal")
        tooltip: ""
        value_context: root.value_context
        value_key: "quickPortals"
    }

    AppCheckbox {
        caption: qsTr("Add quick quest recipes:<br>1. key + Id scroll = Horadric Staff<br>2. key + TP scroll = Khalim Will")
        tooltip: ""
        value_context: root.value_context
        value_key: "quickQuests"
    }

    AppCheckbox {
        caption: qsTr("Add socketing recipes:<br>1. Normal item + TP scroll x1 + Id scroll x1 = Add 3 sockets<br>2. Normal item + TP scroll x1 + Id scroll x2 = Add 4 sockets<br>3. Normal item + TP scroll x2 + Id scroll x1 = Add 5 sockets<br>4. Normal item + TP scroll x2 + Id scroll x2 = Add 6 sockets<br>5. Unique/Set/Rare item + TP scroll x1 + Id scroll x1 = Add 1 socket<br>6. Socketed item + TP scroll x1 = Clear sockets")
        tooltip: ""
        value_context: root.value_context
        value_key: "socketing"
    }

    AppCheckbox {
        caption: qsTr("Add upgrade recipes:<br>1. Normal item (normal,magic,rare,unique,set) + Antidote = Exceptional item<br>2. Exceptional item (normal,magic,rare,unique,set) + Antidote = Elite item<br>3. Any item + Stamina x2 = Add Ethereal<br>4. Magic (magic,rare,unique,set) item  + Stamina = Normal item of same type")
        tooltip: ""
        value_context: root.value_context
        value_key: "upgrading"
    }

    AppCheckbox {
        caption: qsTr("Add cheat Small Charm recipies:<br>1. Any small charm + TP scroll x1 + one of (h pot, mana pot, stamina, antidote, thawing)<br>2. Any small charm + Id scroll x1 + one of (h pot, mana pot, stamina, antidote, thawing)<br>3. Any small charm + key       x1 + one of (h pot, mana pot, stamina, antidote, thawing)")
        tooltip: qsTr("Different combinations provide one of 10 stat combinations.<br>Just two charms of each kind suppose to provide reasonable endgame stat.<br>Item level is 1, so try several times to craft charm with 0 requirements.")
        value_context: root.value_context
        value_key: "cheatCharms"
    }

    Label {
        text: qsTr("Simplyfy crafting:")
    }

    AppCheckbox {
        caption: qsTr("Remove strict item types for Crafted (any helm, any gloves etc)")
        tooltip: ""
        value_context: root.value_context
        value_key: "craftNoStrict"
    }

    AppCheckbox {
        caption: qsTr("Remove Rune/Jewel requirement for Crafted<br>(So any recipe is 'item + gem')")
        tooltip: ""
        value_context: root.value_context
        value_key: "craftNoRunes"
    }

    AppCheckbox {
        caption: qsTr("Make Item Level always 99")
        tooltip: ""
        value_context: root.value_context
        value_key: "craftHighIlvl"
    }
}
