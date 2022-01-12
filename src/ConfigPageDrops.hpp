/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "ConfigPageAbstract.hpp"

class DropsPage : public AbstractPage {
public:
    DropsPage(QWidget* parent);

    // IConfigPage interface
public:
    QString caption() const override
    {
        return "Drops";
    }
    QString settingKey() const override
    {
        return "drops";
    }
    KeySet generate(TableSet& tableSet, QRandomGenerator& rng) const override;
};

