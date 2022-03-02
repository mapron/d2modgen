/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "CommonTypes.hpp"

#include "CoreExport.hpp"

#include <QJsonDocument>

namespace D2ModGen {

CORE_EXPORT bool readJsonFile(const QString& file, QJsonDocument& data);
CORE_EXPORT bool writeJsonFile(const QString& file, const QJsonDocument& data, bool escape = false);

inline QString ensureTrailingSlash(QString path)
{
    if (path.isEmpty())
        return {};

    const bool hasSlash = path.endsWith("\\") || path.endsWith("/");
    if (!hasSlash)
        path += "/";
    return path;
}
inline std::string ensureTrailingSlash(const std::string& path)
{
    return QString::fromStdString(path).toStdString();
}

}
