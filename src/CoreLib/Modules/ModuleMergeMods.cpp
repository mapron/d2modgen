/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ModuleMergeMods.hpp"

#include "FileIOUtils.hpp"

namespace D2ModGen {

namespace {
const bool s_init  = registerHelper<ModuleMergeModsPreload>();
const bool s_init2 = registerHelper<ModuleMergeModsPostGen>();
}

PropertyTreeScalarMap ModuleMergeMods::defaultValues() const
{
    return {
        { "sourceCount", 0 },
    };
}

void ModuleMergeMods::gatherInfoInternal(ExtraDependencies& output, const InputContext& input) const
{
    const int count = input.m_settings.value("sourceCount", 0).toInt();

    for (int i = 0; i < count; ++i) {
        const PropertyTree& itemData = input.m_settings[QString("item_%1").arg(i).toStdString()];

        auto type       = static_cast<StorageType>(itemData["type"].toInt());
        auto policy     = static_cast<ConflictPolicy>(itemData["policy"].toInt());
        auto modname    = QString::fromStdString(itemData["mod"].toString());
        auto folderRoot = QString::fromStdString(itemData["folder"].toString());

        output.m_sources << ExtraDependencies::Source{
            type,
            policy,
            ensureTrailingSlash(folderRoot),
            modname,
        };
    }
}

}
