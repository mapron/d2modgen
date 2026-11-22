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
    value_context: config.balanceAttributes
    caption: qsTr("Item attributes - item rando only")
    tooltip: qsTr("Currently this works only for randomizer.<br>Maybe later this will be possible to use power sliders without rando.")

    LabelWithTooltip {
        caption: qsTr("Adjust <b>frequencies</b> of different attribute groups to appear:")
        tooltip: qsTr("That affects how often will you see specific properties on an item.<br>This does not affect kept properties!")
    }

    WidgetSlider {
        caption: qsTr("Aura when equip:")
        tooltip: ""
        value_key: root.value_context.frequencyAuras
    }
    WidgetSlider {
        caption: qsTr("On hit/attack/struck procs:")
        tooltip: ""
        value_key: root.value_context.frequencyProcs
    }

    WidgetSlider {
        caption: qsTr("Stats - life/mana/attributes:")
        tooltip: qsTr("Life, mana, str, dex, vitality, energy, including (per character level)")
        value_key: root.value_context.frequencyCharAttributes
    }

    WidgetSlider {
        caption: qsTr("Resistances:")
        tooltip: qsTr("Fire, lightning, cold, poison, magic resist, +max resist, +all res. <br>Does not include physical DR.")
        value_key: root.value_context.frequencyResistances
    }

    WidgetSlider {
        caption: qsTr("+skills:")
        tooltip: qsTr("+ skills (class only), +all skills, +skill tab, +specific skill(class only)")
        value_key: root.value_context.frequencySkills
    }

    WidgetSlider {
        caption: qsTr("Damage (except Enh. Damage %):")
        tooltip: qsTr("+min, +max, +flat elemental and physical damage. <br> Does NOT include enhanced damage, only flat.")
        value_key: root.value_context.frequencyDamage
    }

    WidgetSlider {
        caption: qsTr("Life/mana restoration:")
        tooltip: qsTr("Life and mana steal, mana and life regeneration, mana and life per kill")
        value_key: root.value_context.frequencyRestoration
    }

    Separator {}
    LabelWithTooltip {
        caption: qsTr("Adjust <b>values</b> of different attribute groups to generate:")
        tooltip: qsTr("That affects value of new roll.<br> Say, you set it to 200%, then instead of fire resistance 30%, 60% will be generated.")
    }
    WidgetSlider {
        caption: qsTr("Stats - life/mana/attributes:")
        value_key: root.value_context.powerCharAttributes
    }
    WidgetSlider {
        caption: qsTr("Resistances:")
        value_key: root.value_context.powerResistances
    }
    WidgetSlider {
        caption: qsTr("+skills:")
        value_key: root.value_context.powerSkills
    }

    WidgetSlider {
        caption: qsTr("Damage (except Enh. Damage %):")
        value_key: root.value_context.powerDamage
    }
    WidgetSlider {
        caption: qsTr("Life/mana restoration:")
        value_key: root.value_context.powerRestoration
    }
}
