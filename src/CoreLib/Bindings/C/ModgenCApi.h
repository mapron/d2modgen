/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "ModgenCoreExport.hpp"

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

MODGENCORE_EXPORT CTable dataContext_getTable(CDataContext cdataContext, const char* tableName, int32_t needWrite, int32_t needIndexes);

MODGENCORE_EXPORT CRow table_getRow(CTable ctable, int32_t row);
MODGENCORE_EXPORT int  table_getRowCount(CTable ctable);

MODGENCORE_EXPORT CCell row_getCellByIndex(CRow crow, int32_t column);
MODGENCORE_EXPORT CCell row_getCell(CRow crow, const char* columnName);

MODGENCORE_EXPORT void cell_setDataStr(CCell ccell, const char* newValue);
MODGENCORE_EXPORT void cell_setDataInt(CCell ccell, int32_t newValue);

MODGENCORE_EXPORT const char* cell_getDataStr(CCell ccell);
MODGENCORE_EXPORT int32_t     cell_getDataInt(CCell ccell);

// Mernel::PropertyTree
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

MODGENCORE_EXPORT int8_t tree_isNull(CPropertyTree ctree);
MODGENCORE_EXPORT int8_t tree_isScalar(CPropertyTree ctree);
MODGENCORE_EXPORT int8_t tree_isList(CPropertyTree ctree);
MODGENCORE_EXPORT int8_t tree_isMap(CPropertyTree ctree);

MODGENCORE_EXPORT CPropertyTreeScalar tree_getScalar(CPropertyTree ctree);
MODGENCORE_EXPORT CPropertyTreeList   tree_getList(CPropertyTree ctree);
MODGENCORE_EXPORT CPropertyTreeMap    tree_getMap(CPropertyTree ctree);

MODGENCORE_EXPORT CPropertyTree treeMap_value(CPropertyTreeMap ctreemap, const char* key); // can be null

MODGENCORE_EXPORT int8_t treeScalar_isNull(CPropertyTreeScalar cscalar);
MODGENCORE_EXPORT int8_t treeScalar_isBool(CPropertyTreeScalar cscalar);
MODGENCORE_EXPORT int8_t treeScalar_isInt(CPropertyTreeScalar cscalar);
MODGENCORE_EXPORT int8_t treeScalar_isDouble(CPropertyTreeScalar cscalar);
MODGENCORE_EXPORT int8_t treeScalar_isString(CPropertyTreeScalar cscalar);

MODGENCORE_EXPORT int8_t      treeScalar_toBool(CPropertyTreeScalar cscalar);
MODGENCORE_EXPORT int64_t     treeScalar_toInt(CPropertyTreeScalar cscalar);
MODGENCORE_EXPORT const char* treeScalar_toString(CPropertyTreeScalar cscalar);
MODGENCORE_EXPORT double      treeScalar_toDouble(CPropertyTreeScalar cscalar);

// RandomGenerator

typedef void (*GenerateCFuncPtr)(CDataContext cdataContext, CPropertyTree cmoduleData);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include <type_traits>
#define CHECK_GENERATE(gen) static_assert(std::is_same<decltype(&gen), GenerateCFuncPtr>::value, "generate() signature is invalid");
#endif
