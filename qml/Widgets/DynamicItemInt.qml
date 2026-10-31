import QtQuick

Item {
    id: root
    visible: false

    property QtObject value_context: null
    property string value_key: ""

    property int dynamicValue: (value_context && value_key) ? value_context[value_key] : 0

    onDynamicValueChanged: {
        if (value_context && value_key) {
            value_context[value_key] = dynamicValue;
        }
    }
}
