import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

RowLayout {
    id: root
    spacing: 10
    Layout.fillWidth: true

    // --- PUBLIC API ---
    property string suffix: ""
    property int denom: 5
    property int mult: 10
    property int from: s_spinboxAverage / denom
    property int to: s_spinboxAverage * mult
    property int visualWidth: 50
    property bool isLinear: false

    property alias value_context: item.value_context
    property alias value_key: item.value_key

    // --- NON-LINEAR MATH CONVERSION LOGIC ---
    readonly property real s_sliderAverage: 1.0
    readonly property int s_spinboxAverage: 100

    function expGrowthDirect(val) {
        let ex = Math.exp(val * 2);
        ex -= 1.0;
        ex /= (Math.E * Math.E - 1.0);
        return ex;
    }

    function expGrowthReverse(val) {
        val *= (Math.E * Math.E - 1.0);
        val += 1.0;
        let lg = Math.log(val);
        return lg / 2;
    }

    // Converts visual slider track position (0-1000) to actual application value
    function sliderToValue(sliderVal) {
        if (isLinear) {
            return sliderVal;
        } else if (sliderVal === s_sliderAverage) {
            return s_spinboxAverage;
        } else if (sliderVal < s_sliderAverage) {
            let lowRange = s_spinboxAverage - root.from;
            return Math.round(root.from + (sliderVal * lowRange / s_sliderAverage));
        } else {
            let highRatio = sliderVal - s_sliderAverage;
            let highRange = root.to - s_spinboxAverage;
            let ratio = highRatio / s_sliderAverage;
            let ratioExp = expGrowthDirect(ratio);
            return Math.round(s_spinboxAverage + (ratioExp * highRange));
        }
    }

    // Converts actual application value back to visual slider track position (0-1000)
    function valueToSlider(appVal) {
        if (isLinear) {
            return appVal;
        } else if (appVal === s_spinboxAverage) {
            return s_sliderAverage;
        } else if (appVal < s_spinboxAverage) {
            let lowRange = s_spinboxAverage - root.from;
            if (lowRange === 0)
                return 0;
            return (appVal - root.from) * s_sliderAverage / lowRange;
        } else {
            let highRatio = appVal - s_spinboxAverage;
            let highRange = root.to - s_spinboxAverage;
            if (highRange === 0)
                return s_sliderAverage;
            let ratio = highRatio / highRange;
            let ratioLog = isLinear ? ratio : expGrowthReverse(ratio);
            return s_sliderAverage + (ratioLog * s_sliderAverage);
        }
    }

    DynamicItemInt {
        id: item
    }

    Slider {
        id: internalSlider
        Layout.fillWidth: true
        from: isLinear ? root.from : 0.0
        to: isLinear ? root.to : 2.0
        value: valueToSlider(item.dynamicValue)
        handle.implicitWidth: 16
        handle.implicitHeight: 16

        onMoved: item.dynamicValue = sliderToValue(value)
        live: true
    }

    TextField {
        id: textInput

        color: textInput.palette.text
        implicitWidth: root.visualWidth

        background: Rectangle {
            color: textInput.palette.base
            border.color: textInput.acceptableInput ? palette.accent : "red"
            border.width: textInput.acceptableInput ? 1 : 2
        }

        text: item.dynamicValue.toString()

        validator: IntValidator {
            bottom: root.from
            top: root.to
        }
        selectByMouse: true

        // Allow input box typing to update the slider position
        onEditingFinished: item.dynamicValue = parseInt(text)

        rightPadding: root.suffix !== "" ? percentSign.width + 12 : 8

        Text {
            id: percentSign
            visible: text !== ""
            text: root.suffix
            font: textInput.font

            // Match the text color to the input text (or make it slightly lighter/gray)
            color: textInput.color
            opacity: 0.8

            // Position it at the right edge, matching the vertical center of the input text
            anchors.right: parent.right
            anchors.rightMargin: 8
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
