import QtQuick

Item {
    id: root

    property var value_context: null
    property string value_key: "key"

    property int dynamicValue: 0

    Connections {
        target: root.value_context

        // This keeps performance optimal by only listening to the relevant object
        ignoreUnknownSignals: true

        function onDataChanged() {
            if (root.value_context) {
                let newValue = root.value_context.getInt(root.value_key);

                // Prevent recursive updates if the value is already the same
                if (root.dynamicValue !== newValue) {
                    root.dynamicValue = newValue;
                }
            }
        }
    }

    onDynamicValueChanged: {
        if (root.value_context) {
            // Guard clause to avoid rewriting the same value back to C++
            let currentRemoteValue = root.value_context.getInt(root.value_key);
            if (currentRemoteValue !== root.dynamicValue) {
                root.value_context.set(root.value_key, root.dynamicValue);
            }
        }
    }

    onValue_contextChanged: {
        if (root.value_context) {
            root.dynamicValue = root.value_context.getInt(root.value_key);
        }
    }
}
