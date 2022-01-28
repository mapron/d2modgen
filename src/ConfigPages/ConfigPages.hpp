/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "IConfigPage.hpp"

namespace D2ModGen {

QList<IConfigPage*> CreateConfigPages(QWidget* parent);

}
