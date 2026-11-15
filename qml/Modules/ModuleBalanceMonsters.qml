/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick
import "../Widgets"

WidgetCommonTab {
    id: root
    value_context: config.balanceMonsters
    enabled_key: null
    caption: qsTr("Monster stats")
    tooltip: qsTr("Sliders allow you to change monster stats on all difficulties.<br>You can either lower or raise stats up 10x or 10x times.<br>That is orthogonal to /playersX setting - it will multiply stats independent.")

    WidgetSlider {
        caption: qsTr("Monster Attack Rating, multiply by")
        tooltip: ""
        value_key: root.value_context.monsterAttack
    }

    WidgetSlider {
        caption: qsTr("Monster Defense, multiply by")
        tooltip: ""
        value_key: root.value_context.monsterDefense
    }

    WidgetSlider {
        caption: qsTr("Monster HP, multiply by")
        tooltip: ""
        value_key: root.value_context.monsterHealth
    }

    WidgetSlider {
        caption: qsTr("Monster Damage, multiply by")
        tooltip: ""
        value_key: root.value_context.monsterDamage
    }

    WidgetSlider {
        caption: qsTr("Monster EXP gain, multiply by")
        tooltip: ""
        value_key: root.value_context.monsterExperience
    }

    LabelWithTooltip {
        caption: qsTr("Change monster resistances")

        tooltip: qsTr("Say monster have 120 Fire res on Hell, 25 Lightning and 50 Cold" //
        + "<br>Then Fire is considered 'first', cold 'second' and Lightning 'third'. " //
        + "<br>If you want all monsters to be immune, set minimum range as 100" //
        + "<br>To make 3 immunities, set all 3 sliders to 100 minimum" //
        + "<br>To make all monster have 'Sunder charm' effect, set 95 as max" //
        + "<br>This will not affext X Enchanted elites boosting res" + "<br><b>Unbreakable res:</b> D2R= 145%, PD2= 170%.")
    }

    WidgetRadioGroup {
        caption: qsTr("What difficulty to apply resistance change:")
        isCompact: false
        model: [qsTr("None"), qsTr("Hell"), qsTr("Nightmare and Hell"), qsTr("Normal, Nightmare and Hell")]
        value_key: root.value_context.resChangeMode
    }

    WidgetRes {
        caption: qsTr("Change highest resistance")
        tooltip: qsTr("This modifies monster's highest resisntance value.<br>Usually this is its immunity")
        enabled: root.value_context.resChangeMode.value > 0

        value_key1: root.value_context.resMin1
        value_key2: root.value_context.resMax1
    }

    WidgetRes {
        caption: qsTr("Change 2nd resistance")
        tooltip: qsTr("This modifies resistance that is second in value,<br> it can be immunity on hell or just high value.")
        enabled: root.value_context.resChangeMode.value > 0

        value_key1: root.value_context.resMin2
        value_key2: root.value_context.resMax2
    }

    WidgetRes {
        caption: qsTr("Change 3nd resistance")
        tooltip: qsTr("You probably never notice in gameplay what 3rd resistance of monster is,<br>if you put mim to 100 and above you will learn a lot<br>Also monster can have all 6 res set, I don't want to add 6 controls")
        enabled: root.value_context.resChangeMode.value > 0

        value_key1: root.value_context.resMin3
        value_key2: root.value_context.resMax3
    }

    WidgetCheckbox {
        caption: qsTr("Use Hell resistances on NM/Normal")
        tooltip: qsTr("Want to deal with immunities in Normal? Here you go.")
        value_key: root.value_context.hellResistances
    }
}
