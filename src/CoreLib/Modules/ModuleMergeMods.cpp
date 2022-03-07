/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ModuleMergeMods.hpp"

#include "FileIOUtils.hpp"
#include "TableUtils.hpp"

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
        const PropertyTree& itemData = input.m_settings[argCompat("item_%1", i)];

        auto type       = static_cast<StorageType>(itemData["type"].getScalar().toInt());
        auto policy     = static_cast<ConflictPolicy>(itemData["policy"].getScalar().toInt());
        auto modname    = (itemData["mod"].getScalar().toString());
        auto folderRoot = (itemData["folder"].getScalar().toString());

        output.m_sources.push_back(ExtraDependencies::Source{
            type,
            policy,
            ensureTrailingSlash(folderRoot),
            modname,
        });
    }
}

}
