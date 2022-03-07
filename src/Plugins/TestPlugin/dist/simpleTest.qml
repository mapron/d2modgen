/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */

import QtQuick 2.0
import QtQuick.Controls 2.15

Page {
    id: root
    objectName: "root"
    
    Column {
        spacing: 20
        anchors.horizontalCenter: parent.horizontalCenter
        
        Row {
            Label {
                text: "Reduce armor and weapon requirements:"
            }
        
            Slider {
                id: reduceRequirements
                from: 1
                value: 25
                to: 100
            }
        
        }
        
        
        CheckBox {
            id: addKeys
            text: "Add keys to character's inventory"
            onClicked: parentWidget.dataChanged();
        }
        
    }
    
    function setFormValues(data) {
        addKeys.checked = data["addKeys"]
        reduceRequirements.value = data["reduceRequirements"]
    }
    
    function getFormValues() {
        return {
            "addKeys" : addKeys.checked,
            "reduceRequirements" : Math.floor(reduceRequirements.value),
            "randomDouble": 42.1,
            "randomString": "foo",
            "randomArray" : [1, 2]
        }
    }
}
