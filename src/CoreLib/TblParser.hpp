/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 * 
 * Based on the Diablo II .tbl file structural definitions by Andrey Filipenkov.
 * Original project (https://github.com/kambala-decapitator/QTblEditor) licensed under the MIT License.
 * Borrowed code is minimal as should be considered as fair use.
 */
#pragma once

#include "MernelPlatform/ByteOrderStream.hpp"

namespace D2ModGen {

struct TblHeader // header of string *.tbl file
{
    uint16_t CRC;             // +0x00 - CRC value for string table
    uint16_t NodesNumber;     // +0x02 - size of Indices array
    uint32_t HashTableSize;   // +0x04 - size of TblHashNode array
    uint8_t  Version;         // +0x08 - file version, either 0 or 1, doesn't matter
    uint32_t DataStartOffset; // +0x09 - string table start offset
    uint32_t HashMaxTries;    // +0x0D - max number of collisions for string key search based on its hash value
    uint32_t FileSize;        // +0x11 - size of the file

    static const int size = 0x15;

    void readBinary(Mernel::ByteOrderDataStreamReader& stream);
    void writeBinary(Mernel::ByteOrderDataStreamWriter& stream) const;
};

struct TblHashNode // node of the hash table in string *.tbl file
{
    uint8_t  Active;          // +0x00 - shows if the entry is used. if 0, then it has been "deleted" from the table
    uint16_t Index;           // +0x01 - index in Indices array
    uint32_t HashValue;       // +0x03 - hash value of the current string key
    uint32_t StringKeyOffset; // +0x07 - offset of the current string key
    uint32_t StringValOffset; // +0x0B - offset of the current string value
    uint16_t StringValLength; // +0x0F - length of the current string value

    static const int size = 0x11;

    void readBinary(Mernel::ByteOrderDataStreamReader& stream);
    void writeBinary(Mernel::ByteOrderDataStreamWriter& stream) const;
};

struct DataNode // node of the string table in string *.tbl file
{
    std::string Key;
    std::string Val;
    size_t      Offset = 0;
};

struct TblStructure {
    std::vector<DataNode> m_rows;

    void readBinary(Mernel::ByteOrderDataStreamReader& stream);
    void writeBinary(Mernel::ByteOrderDataStreamWriter& stream) const;

    void readFromBuffer(const std::string& buffer);
    void writeToBuffer(std::string& buffer) const;

    static uint32_t hashValue(std::string_view str, int hashTableSize);
    static uint16_t getCRC(std::string_view str);
};

}
