import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick
import "../Widgets"

WidgetCommonTab {
    id: root
    value_context: config.modulePerfectRoll
    enabled_key: config.enabledModules.modulePerfectRoll
    caption: qsTr("Perfect rolls")
    tooltip: qsTr("Remove random effect when rolling between min and max value for something.")

    RowLayout {
        ColumnLayout {
            WidgetCheckbox {
                caption: qsTr("Perfect rolls for crafting in the Cube")
                tooltip: ""
                        value_key: root.value_context.craft
            }

            WidgetCheckbox {
                caption: qsTr("Perfect Uniques")
                tooltip: ""
                        value_key: root.value_context.uniques
            }

            WidgetCheckbox {
                caption: qsTr("Perfect Rune Word rolls")
                tooltip: ""
                        value_key: root.value_context.runeWords
            }

            WidgetCheckbox {
                caption: qsTr("Perfect Set items")
                tooltip: ""
                        value_key: root.value_context.setItems
            }
        }
        ColumnLayout {
            WidgetCheckbox {
                caption: qsTr("Perfect Magic/Rare affixes")
                tooltip: ""
                        value_key: root.value_context.affixes
            }

            WidgetCheckbox {
                caption: qsTr("Perfect predetermined crafted affixes")
                tooltip: ""
                        value_key: root.value_context.crafted
            }

            WidgetCheckbox {
                caption: qsTr("Always max defense Armor")
                tooltip: ""
                        value_key: root.value_context.armor
            }
        }
    }

    WidgetExponentialSlider {
        caption: qsTr("Unique items level 1 power multiply")
        tooltip: ""
        value_key: root.value_context.uniques_power_l1
        isCompact: true
        stretchCaption: true
    }

    WidgetExponentialSlider {
        caption: qsTr("Unique items level 50 power multiply")
        tooltip: ""
        value_key: root.value_context.uniques_power_l50
        isCompact: true
        stretchCaption: true
    }

    WidgetExponentialSlider {
        caption: qsTr("Set items level 1 power multiply")
        tooltip: ""
        value_key: root.value_context.set_power_l1
        isCompact: true
        stretchCaption: true
    }

    WidgetExponentialSlider {
        caption: qsTr("Set items level 50 power multiply")
        tooltip: ""
        value_key: root.value_context.set_power_l50
        isCompact: true
        stretchCaption: true
    }

    WidgetExponentialSlider {
        caption: qsTr("Magic affix level 1 power multiply")
        tooltip: ""
        value_key: root.value_context.affix_power_l1
        isCompact: true
        stretchCaption: true
    }

    WidgetExponentialSlider {
        caption: qsTr("Magic affix level 50 power multiply")
        tooltip: ""
        value_key: root.value_context.affix_power_l50
        isCompact: true
        stretchCaption: true
    }

    WidgetExponentialSlider {
        caption: qsTr("Rune Words power multiply")
        tooltip: ""
        value_key: root.value_context.rw_power
        isCompact: true
        stretchCaption: true
    }

    WidgetExponentialSlider {
        caption: qsTr("Crafted items power multiply")
        tooltip: ""
        value_key: root.value_context.crafted_power
        isCompact: true
        stretchCaption: true
    }

    WidgetExponentialSlider {
        caption: qsTr("Rune/Gems power multiply")
        tooltip: ""
        value_key: root.value_context.gem_power
        isCompact: true
        stretchCaption: true
    }
}
