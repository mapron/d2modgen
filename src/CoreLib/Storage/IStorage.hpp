/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "TableId.hpp"
#include "JsonId.hpp"

#include <map>

namespace D2ModGen {

class IStorage {
public:
    static std::string makeTableRelativePath(TableId id, bool baseSub);

    static constexpr std::string_view s_dataVersionPath = "data\\global\\dataversionbuild.txt";

    struct StoredData {
        bool                           valid = false;
        std::map<TableId, std::string> tables;
        std::map<JsonId, std::string>  jsonFiles;

        int64_t dataVersion = 0;

        bool operator==(const StoredData&) const = default;
    };

public:
    virtual ~IStorage() = default;

    virtual StoredData readData() const noexcept = 0;
};

class IWriteableStorage : public IStorage {
public:
    virtual bool prepareForWrite() const noexcept                 = 0;
    virtual bool writeData(const StoredData& data) const noexcept = 0;
};

}
