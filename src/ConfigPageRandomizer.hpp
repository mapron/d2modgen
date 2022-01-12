/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "ConfigPageAbstract.hpp"

class RandomizerPage : public AbstractPage {
public:
    RandomizerPage(QWidget* parent);

    struct MagicProp {
        QString code;
        QString par;
        QString min;
        QString max;
        int     level = 0;
    };

    struct MagicPropBundle {
        QList<MagicProp> props;
        int              level = 0;
    };

    struct MagicPropBucket {
        QList<MagicPropBundle> bundles;
        QMap<int, int>         lowerLevelBounds;

        void postProcess(bool replaceSkills, bool replaceCharges);

        void sortByLevel();

        std::pair<int, int> getBounds(int level, int balance, int minRange) const;

        const MagicPropBundle& getRandomBundle(QRandomGenerator& rng, int level, int balance) const;

        void addParsedBundle(MagicPropBundle inBundle);
    };

    struct MagicPropSet {
        // @todo: do we really need to handle item-specific properties (knockback, replinish quantity)?
        static const QString           s_all;
        QMap<QString, MagicPropBucket> bucketByType;
    };

    // IConfigPage interface
public:
    QString caption() const override
    {
        return "Item Randomizer";
    }
    QString settingKey() const override
    {
        return "randomizer";
    }
    KeySet generate(TableSet& tableSet, QRandomGenerator& rng) const override;
};

