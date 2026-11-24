/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */

#include "DataContextOther.hpp"

#include <cassert>

namespace D2ModGen {

namespace {
const std::vector<std::string> s_modeNames{
    "DT",
    "NU",
    "WL",
    "GH",
    "A1",
    "A2",
    "BL",
    "SC",
    "S1",
    "S2",
    "S3",
    "S4",
    "DD",
    "KB",
    "SQ",
    "RN",
};

}

AnimationMode animationModeFromString(const std::string& mode)
{
    for (size_t i = 0; i < s_modeNames.size(); i++) {
        if (s_modeNames[i] == mode) {
            return AnimationMode(i);
        }
    }
    return AnimationMode::Undefined;
}

std::string animationModeToString(AnimationMode mode)
{
    if (mode == AnimationMode::Undefined)
        return "";
    return s_modeNames[static_cast<int>(mode)];
}

bool DataRow_monstatsEx::hasMode(AnimationMode mode) const
{
    if (mode == AnimationMode::Undefined)
        return false;
    return modes[static_cast<int>(mode)].value_or(false);
}

AnimationMode DataRow_monstatsEx::getSafeSupportedMode() const
{
    for (AnimationMode safeMode : { AnimationMode::A1, AnimationMode::A2, AnimationMode::NU }) {
        if (hasMode(safeMode)) {
            return safeMode;
        }
    }
    assert(0);
    return AnimationMode::Undefined;
}

Character DataRow_skills::getClass() const
{
    return getClassId(classSpecific);
}

void DataRow_skills::setNewClass(Character clas)
{
    auto prev = getClass();
    if (clas != prev) {
        if (prev == Character::Ass) {
            std::set<std::string> codeChange{ "KK", "S2" };
            for (auto* col : { &anim, &seqtrans }) {
                if (codeChange.contains(*col))
                    *col = "A1";
            }
        }
    }

    classSpecific = enumToString(clas);
}

int DataRow_treasureclassex::getDropSomethingProb() const
{
    int totalOther = 0;
    for (const auto& item : items)
        totalOther += item.prob.value_or(0);
    return totalOther;
}

}
