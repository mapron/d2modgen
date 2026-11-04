.pragma library

/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */

const s_sliderAverage = 1.0;
const s_spinboxAverage = 100;

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
function sliderToValue(isLinear, from, to, sliderVal) {
    if (isLinear) {
        return sliderVal;
    } else if (sliderVal === s_sliderAverage) {
        return s_spinboxAverage;
    } else if (sliderVal < s_sliderAverage) {
        let lowRange = s_spinboxAverage - from;
        return Math.round(from + (sliderVal * lowRange / s_sliderAverage));
    } else {
        let highRatio = sliderVal - s_sliderAverage;
        let highRange = to - s_spinboxAverage;
        let ratio = highRatio / s_sliderAverage;
        let ratioExp = expGrowthDirect(ratio);
        return Math.round(s_spinboxAverage + (ratioExp * highRange));
    }
}

// Converts actual application value back to visual slider track position (0-1000)
function valueToSlider(isLinear, from, to, appVal) {
    if (isLinear) {
        return appVal;
    } else if (appVal === s_spinboxAverage) {
        return s_sliderAverage;
    } else if (appVal < s_spinboxAverage) {
        let lowRange = s_spinboxAverage - from;
        if (lowRange === 0)
            return 0;
        return (appVal - from) * s_sliderAverage / lowRange;
    } else {
        let highRatio = appVal - s_spinboxAverage;
        let highRange = to - s_spinboxAverage;
        if (highRange === 0)
            return s_sliderAverage;
        let ratio = highRatio / highRange;
        let ratioLog = isLinear ? ratio : expGrowthReverse(ratio);
        return s_sliderAverage + (ratioLog * s_sliderAverage);
    }
}
