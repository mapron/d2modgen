/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */

import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Window 2.2

Page {
    id: root
    objectName: "root"
    Column {
        spacing: 8
        anchors.top: parent.top
       
        
        Row {
            Label {
                text: "Reduce armor and weapon requirements:"
                
            }
        
            Slider {
                id: reduceRequirements
                from: 1
                value: 25
                to: 100
                /*
                handle: Rectangle {
                        x: reduceRequirements.leftPadding + (reduceRequirements.horizontal ? reduceRequirements.visualPosition * (reduceRequirements.availableWidth - width) : (reduceRequirements.availableWidth - width) / 2)
                        y: reduceRequirements.topPadding + (reduceRequirements.horizontal ? (reduceRequirements.availableHeight - height) / 2 : reduceRequirements.visualPosition * (reduceRequirements.availableHeight - height))
                        implicitWidth: 14
                        implicitHeight: 14
                        radius: width / 2
                        color: reduceRequirements.pressed ? reduceRequirements.palette.light : reduceRequirements.palette.window
                        border.width: reduceRequirements.visualFocus ? 2 : 1
                        border.color: reduceRequirements.visualFocus ? reduceRequirements.palette.highlight : reduceRequirements.enabled ? reduceRequirements.palette.mid : reduceRequirements.palette.midlight
                    }
                background: Rectangle {
                       x: reduceRequirements.leftPadding + (reduceRequirements.horizontal ? 0 : (reduceRequirements.availableWidth - width) / 2)
                       y: reduceRequirements.topPadding + (reduceRequirements.horizontal ? (reduceRequirements.availableHeight - height) / 2 : 0)
                       implicitWidth: reduceRequirements.horizontal ? 200 : 8
                       implicitHeight: reduceRequirements.horizontal ? 8 : 200
                       width: reduceRequirements.horizontal ? reduceRequirements.availableWidth : implicitWidth
                       height: reduceRequirements.horizontal ? implicitHeight : reduceRequirements.availableHeight
                       radius: 4
                       color: reduceRequirements.palette.midlight
                       scale: reduceRequirements.horizontal && reduceRequirements.mirrored ? -1 : 1
               
                       Rectangle {
                           y: reduceRequirements.horizontal ? 0 : reduceRequirements.visualPosition * parent.height
                           width: reduceRequirements.horizontal ? reduceRequirements.position * parent.width : 8
                           height: reduceRequirements.horizontal ? 8 : reduceRequirements.position * parent.height
               
                           radius: 4
                           color: reduceRequirements.palette.dark
                       }
                   }*/
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
