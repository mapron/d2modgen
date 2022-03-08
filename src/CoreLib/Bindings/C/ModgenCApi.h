/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "CoreExport.hpp"

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

// DataContext

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

CORE_EXPORT CTable dataContext_getTable(CDataContext cdataContext, const char* tableName, int32_t needWrite, int32_t needIndexes);

CORE_EXPORT CRow table_getRow(CTable ctable, int32_t row);
CORE_EXPORT int  table_getRowCount(CTable ctable);

CORE_EXPORT CCell row_getCellByIndex(CRow crow, int32_t column);
CORE_EXPORT CCell row_getCell(CRow crow, const char* columnName);

CORE_EXPORT void cell_setDataStr(CCell ccell, const char* newValue);
CORE_EXPORT void cell_setDataInt(CCell ccell, int32_t newValue);

CORE_EXPORT const char* cell_getDataStr(CCell ccell);
CORE_EXPORT int32_t     cell_getDataInt(CCell ccell);

// PropertyTree
typedef struct {
    const void* opaque;
} CPropertyTree;

typedef struct {
    const void* opaque;
} CPropertyTreeScalar;

typedef struct {
    const void* opaque;
} CPropertyTreeMap;

typedef struct {
    const void* opaque;
} CPropertyTreeList;

CORE_EXPORT int8_t tree_isNull(CPropertyTree ctree);
CORE_EXPORT int8_t tree_isScalar(CPropertyTree ctree);
CORE_EXPORT int8_t tree_isList(CPropertyTree ctree);
CORE_EXPORT int8_t tree_isMap(CPropertyTree ctree);

CORE_EXPORT CPropertyTreeScalar tree_getScalar(CPropertyTree ctree);
CORE_EXPORT CPropertyTreeList   tree_getList(CPropertyTree ctree);
CORE_EXPORT CPropertyTreeMap    tree_getMap(CPropertyTree ctree);

CORE_EXPORT CPropertyTree treeMap_value(CPropertyTreeMap ctreemap, const char* key); // can be null

CORE_EXPORT int8_t treeScalar_isNull(CPropertyTreeScalar cscalar);
CORE_EXPORT int8_t treeScalar_isBool(CPropertyTreeScalar cscalar);
CORE_EXPORT int8_t treeScalar_isInt(CPropertyTreeScalar cscalar);
CORE_EXPORT int8_t treeScalar_isDouble(CPropertyTreeScalar cscalar);
CORE_EXPORT int8_t treeScalar_isString(CPropertyTreeScalar cscalar);

CORE_EXPORT int8_t      treeScalar_toBool(CPropertyTreeScalar cscalar);
CORE_EXPORT int64_t     treeScalar_toInt(CPropertyTreeScalar cscalar);
CORE_EXPORT const char* treeScalar_toString(CPropertyTreeScalar cscalar);
CORE_EXPORT double      treeScalar_toDouble(CPropertyTreeScalar cscalar);

// RandomGenerator

typedef void (*GenerateCFuncPtr)(CDataContext cdataContext, CPropertyTree cmoduleData);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include <type_traits>
#define CHECK_GENERATE(gen) static_assert(std::is_same<decltype(&gen), GenerateCFuncPtr>::value, "generate() signature is invalid");
#endif
