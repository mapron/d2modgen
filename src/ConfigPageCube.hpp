/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "ConfigPageAbstract.hpp"

class CubePage : public AbstractPage {
public:
    CubePage(QWidget* parent);

    // IConfigPage interface
public:
    QString caption() const override
    {
        return "Horadric Cube";
    }
    QString settingKey() const override
    {
        return "cube";
    }
    KeySet generate(TableSet& tableSet, QRandomGenerator& rng) const override;
};
