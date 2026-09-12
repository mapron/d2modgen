import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

WidgetCommonControl {
    id: root

    // --- PUBLIC API ---
    property int denom: 5
    property int mult: 10
    property int from: s_spinboxAverage / denom
    property int to: s_spinboxAverage * mult

    // --- NON-LINEAR MATH CONVERSION LOGIC ---
    readonly property int s_sliderAverage: 500
    readonly property int s_spinboxAverage: 100

    onAppDataChanged: {
        value = getValue();
        internalSlider.value = valueToSlider(value);
        textInput.text = value.toString();
    }

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
        if (sliderVal === s_sliderAverage) {
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
        if (appVal === s_spinboxAverage) {
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
            let ratioLog = expGrowthReverse(ratio);
            return s_sliderAverage + (ratioLog * s_sliderAverage);
        }
    }

    // Bottom Row: Slider track + Interactive value display box
    RowLayout {
        spacing: 10
        Layout.fillWidth: true

        Slider {
            id: internalSlider
            Layout.fillWidth: true
            from: 0
            to: 1000
            live: true
            handle.implicitWidth: 16
            handle.implicitHeight: 16

            // Explicitly sync visual handle from the real application value
            value: root.valueToSlider(root.value)

            // Triggers smoothly only when the user is actively dragging the handle
            onMoved: {
                root.value = root.sliderToValue(internalSlider.value);
                root.setValue();
                textInput.text = root.value.toString();
            }
        }

        RowLayout {
            spacing: 4

            TextField {
                id: textInput

                color: textInput.palette.text

                background: Rectangle {
                    implicitWidth: 45
                    implicitHeight: 22
                    color: textInput.palette.base
                    border.color: textInput.acceptableInput ? palette.mid : "red"
                    border.width: textInput.acceptableInput ? 1 : 2
                }

                text: root.value.toString()
                validator: IntValidator {
                    bottom: root.from
                    top: root.to
                }
                selectByMouse: true

                // Allow input box typing to update the slider position
                onAccepted: {
                    root.value = parseInt(text);
                    root.setValue();
                }
            }

            Label {
                text: root.suffix
                visible: root.suffix !== ""
            }
        }
    }
}
