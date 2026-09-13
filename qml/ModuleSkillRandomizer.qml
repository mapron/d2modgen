import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick

WidgetCommonTab {
    id: root
    value_context: appModuleSkillRandomizer
    caption: qsTr("Skill Randomizer")
    tooltip: qsTr("This allow to bring 'some' randomization into character skills.")

    WidgetCheckbox {
        caption: qsTr("Randomize skill tabs within each character")
        tooltip: qsTr("That DOES not move skills between characters! What it can do:<br>-Move a skill to a different skill tab;<br>-Move a skill to a higher/lower level;<br>-Change skill requirements.<br>So, after level 50 there is not much of a difference. Synergies are also untouched.")
        value_context: root.value_context
        value_key: "skillTree"
    }

    WidgetCheckbox {
        caption: qsTr("Randomize element types for some damage skills")
        tooltip: qsTr("For some skills, where it is easy to change damage type, it will randomize between 5 different elements:<br>Cold, Fire, Lightning, Magic and Poison<br>WARNING! Skill descriptions won't be updated and will be completely wrong!<br>Also, character info screen does not show difference between cold and magic correctly.<br>You need to find by yourself what damage (magic or cold) was rolled.<br>For poison, all damage is dealt over 2 seconds and slightly more than original.")
        value_context: root.value_context
        value_key: "skillDamage"
    }

    WidgetCheckbox {
        caption: qsTr("Ensure that element types are different from original")
        tooltip: qsTr("If the source element is Cold, then new randomized element never should be Cold.<br>If this turned off, ~20% of skills will remain original behaviour.")
        value_context: root.value_context
        value_key: "ensureDifferent"
    }
}
