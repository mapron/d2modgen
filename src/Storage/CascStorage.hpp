/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "IStorage.hpp"

namespace D2ModGen {

class CascStorage : public IInputStorage {
public:
    Result readData(const QString& storageRoot, const RequestFileList& filenames) const noexcept override;

    Result listContents(const QString& storageRoot) const noexcept override { return {}; }
};

}
