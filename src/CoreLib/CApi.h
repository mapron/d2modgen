/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "CoreExport.hpp"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void* opaque;
} CDataContext;

typedef struct {
    void* opaque;
} CTable;

typedef struct {
    void* opaque;
} CRow;

typedef struct {
    void* opaque;
} CCell;

CORE_EXPORT CTable dataContext_getTable(CDataContext cdataContext, const char* tableName, int needWrite, int needIndexes);

CORE_EXPORT CRow table_getRow(CTable ctable, int row);
CORE_EXPORT int  table_getRowCount(CTable ctable);

CORE_EXPORT CCell row_getCellByIndex(CRow crow, int column);
CORE_EXPORT CCell row_getCell(CRow crow, const char* columnName);

CORE_EXPORT void cell_setDataStr(CCell ccell, const char* newValue);
CORE_EXPORT void cell_setDataInt(CCell ccell, int newValue);

CORE_EXPORT const char* cell_getDataStr(CCell ccell);
CORE_EXPORT int         cell_getDataInt(CCell ccell);

typedef void (*GenerateCFuncPtr)(CDataContext cdataContext);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include <type_traits>
#define CHECK_GENERATE(gen) static_assert(std::is_same<decltype(&gen), GenerateCFuncPtr>::value, "generate() signature is invalid");
#endif
