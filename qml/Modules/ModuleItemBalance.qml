import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick
import "../Widgets"

WidgetCommonTab {
    id: root
    value_context: config.itemBalance
    enabled_key: null
    caption: qsTr("Item balance")
    tooltip: qsTr("Remove random effect when rolling between min and max value for something.")

    ModuleItemBalanceCommon {
        caption: qsTr("Common settings")
        value_context: config.itemBalance.defaults
        isDefault: true
    }
    ModuleItemBalanceCommon {
        caption: qsTr("Armor and weapons bases")
        isEquipment: true
        value_context: config.itemBalance.equipment
    }
    ModuleItemBalanceCommon {
        caption: qsTr("Uniques")
        value_context: config.itemBalance.uniques
    }
    ModuleItemBalanceCommon {
        caption: qsTr("Set items")
        value_context: config.itemBalance.setitems
    }
    ModuleItemBalanceCommon {
        caption: qsTr("Sets")
        value_context: config.itemBalance.sets
    }
    ModuleItemBalanceCommon {
        caption: qsTr("Runewords")
        value_context: config.itemBalance.runewords
    }
    ModuleItemBalanceCommon {
        caption: qsTr("Magic/rare affixes")
        value_context: config.itemBalance.sets
    }
    ModuleItemBalanceCommon {
        caption: qsTr("Cube crafted affixes")
        value_context: config.itemBalance.crafted
    }
    ModuleItemBalanceCommon {
        caption: qsTr("Gems and runes")
        value_context: config.itemBalance.gems
    }

    Rectangle {
        Layout.fillWidth: true
        Layout.preferredHeight: 1
        color: palette.accent
    }

    WidgetSlider {
        caption: qsTr("Change Strength requirements on items")
        tooltip: qsTr("If you set value above 100%, Strength requirements <br> become higher compared to the original.")
        value_key: root.value_context.reqStr
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("Change Dexterity requirements on items")
        tooltip: ""
        value_key: root.value_context.reqDex
        isCompact: false
    }
}
