/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "ConfigPageAbstract.hpp"

class DifficultyPage : public AbstractPage {
public:
    DifficultyPage(QWidget* parent);

    // IConfigPage interface
public:
    QString caption() const override
    {
        return "Difficulty";
    }
    QString settingKey() const override
    {
        return "difficulty";
    }
    KeySet generate(TableSet& tableSet, QRandomGenerator& rng) const override;
};
