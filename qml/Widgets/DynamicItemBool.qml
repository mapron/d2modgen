import QtQuick

Item {
    id: root
    visible: false

    property QtObject value_context: null
    property string value_key: ""

    property bool dynamicValue: (value_context && value_key) ? value_context[value_key] : false

    onDynamicValueChanged: {
        if (value_context && value_key) {
            value_context[value_key] = dynamicValue;
        }
    }
}
