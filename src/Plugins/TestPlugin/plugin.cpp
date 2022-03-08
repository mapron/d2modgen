/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */

#include "Bindings/Cpp/ModgenCppWrapper.hpp"

#include "PluginExport.hpp"

extern "C" PLUGIN_EXPORT void generate(CDataContext cdataContext, CPropertyTree cmoduleData)
{
    Wrapper::PropertyTree ptree(cmoduleData);
    const auto&           formValues = ptree.getMap();
    const bool            addKeys    = formValues.scalarValue("addKeys").toBool();
    if (!addKeys)
        return;

    Wrapper::DataContext ctx(cdataContext);

    Wrapper::Table charTable = ctx.getTable("charstats", true, false);
    const int      size      = charTable.getRowCount();
    for (int i = 0; i < size; ++i) {
        Wrapper::Row row = charTable.getRow(i);

        if (row["class"].getStr() == "Expansion")
            continue;
        for (int i = 1; i <= 10; ++i) {
            const std::string codeColumn  = "item" + std::to_string(i);
            const std::string countColumn = "item" + std::to_string(i) + "count";

            Wrapper::Cell code  = row[codeColumn];
            Wrapper::Cell count = row[countColumn];
            if (count.getInt() == 0) {
                count.setInt(1);
                code.setStr("key");
                break;
            }
        }
    }
}

CHECK_GENERATE(generate)
