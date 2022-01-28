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

    using ItemCode    = uint32_t;
    using ItemCodeSet = QSet<uint32_t>;
    struct ItemCodeFilter {
        ItemCodeSet include;
        ItemCodeSet exclude;
    };

    struct MagicProp {
        QString        code;
        QString        par;
        QString        min;
        QString        max;
        ItemCodeFilter itemTypes;
        int            level = 0;
    };

    struct MagicPropBundle {
        std::vector<MagicProp> props;
        int                    level = 0;
        ItemCodeFilter         itemTypes;

        void removeAt(int i) { props.erase(std::next(props.begin(), i)); }
    };

    struct MagicPropBucket {
        std::vector<MagicPropBundle> bundles;
        QMap<int, int>               lowerLevelBounds;

        void postProcess(bool replaceSkills, bool replaceCharges, bool skipKnock);

        void sortByLevel();

        std::pair<int, int> getBounds(int level, int balance, int minRange) const;

        const MagicPropBundle& getRandomBundle(QRandomGenerator& rng, int level, int balance) const;

        void addParsedBundle(MagicPropBundle inBundle);
    };

    struct MagicPropSet {
        std::map<AttributeFlag, MagicPropBucket> bucketByType;

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
        ItemCodeSet         nested;
        ItemCodeSet         parents;
    };

    struct MagicPropUniverse {
        std::map<ItemCode, MagicPropSet> propSetByCode;
        MagicPropSet                     all;
        std::map<ItemCode, ItemTypeInfo> itemTypeInfo;

        QList<const MagicPropBundle*> getRandomBundles(const AttributeFlagSet& allowedTypes,
                                                       const ItemCodeFilter&   query,
                                                       int                     specificItemUsage,
                                                       QRandomGenerator&       rng,
                                                       int                     count,
                                                       int                     level,
                                                       int                     balance) const;

        ItemCodeSet processQuery(const ItemCodeFilter& query) const;

        void fillPropSets();
    };

    static ItemCode makeCode(const QString& str)
    {
        const auto s = str.size();
        if (str.isEmpty()) {
            return 0;
        }
        uint32_t res = 0;
        for (int i = 0; i < 4; ++i) {
            const uint32_t shift = (8 * (4 - i - 1));
            const auto     c     = i >= s ? uint8_t(0) : static_cast<uint8_t>(str[i].toLatin1());
            res |= (c << shift);
        }
        return res;
    }
    static QString printCode(ItemCode code)
    {
        if (!code)
            return "";
        QString result;
        result.reserve(4);
        for (int i = 0; i < 4; ++i) {
            const uint32_t shift  = (8 * (4 - i - 1));
            const uint32_t masked = code & (0xFFU << shift);
            const uint8_t  c      = masked >> shift;
            if (c)
                result += static_cast<char>(c);
        }
        return result;
    }
    static QStringList printCodes(const ItemCodeSet& codes)
    {
        QStringList result;
        for (const auto c : codes)
            result << printCode(c);
        return result;
    }

    struct BucketRange {
        AttributeFlag type;
        int           offset;
        int           size;
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
