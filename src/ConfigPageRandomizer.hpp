/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "ConfigPageAbstract.hpp"
#include "AttributeHelper.hpp"

class ConfigPageRandomizer : public ConfigPageAbstract {
public:
    ConfigPageRandomizer(QWidget* parent);

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

        void postProcess(bool replaceSkills, bool replaceCharges, bool skipKnock);

        void sortByLevel();

        std::pair<int, int> getBounds(int level, int balance, int minRange) const;

        const MagicPropBundle& getRandomBundle(QRandomGenerator& rng, int level, int balance) const;

        void addParsedBundle(MagicPropBundle inBundle);
    };

    struct MagicPropSet {
        QMap<AttributeFlag, MagicPropBucket> bucketByType;

        void addParsedBundle(MagicPropBundle inBundle);
        void postProcess(bool replaceSkills, bool replaceCharges, bool skipKnock);

        QList<const MagicPropBundle*> getRandomBundles(const AttributeFlagSet& allowedTypes,
                                                       QRandomGenerator&       rng,
                                                       int                     count,
                                                       int                     level,
                                                       int                     balance) const;
    };

    struct ItemTypeInfo {
        QSet<AttributeFlag> flags;
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
    KeySet generate(GenOutput& output, QRandomGenerator& rng, const GenerationEnvironment& env) const override;
};
