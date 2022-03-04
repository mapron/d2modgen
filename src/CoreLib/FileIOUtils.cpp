/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "FileIOUtils.hpp"

#include <fstream>
#include <sstream>

#include <string>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/filestream.h>

#include "Logger.hpp"

namespace D2ModGen {

class JsonStreamOut {
public:
    JsonStreamOut(std::string& output)
        : m_output(output)
    {}

    char   Peek() const { return 0; }
    char   Take() { return 0; }
    size_t Tell() const { return 0; }

    void Put(char c) { m_output += c; }

    char*  PutBegin() { return 0; }
    size_t PutEnd(char*) { return 0; }

private:
    std::string& m_output;
};

void jsonToPropery(PropertyTree& data, rapidjson::Value& input)
{
    if (input.IsObject()) {
        data.convertToMap();
        for (auto keyIt = input.MemberBegin(); keyIt != input.MemberEnd(); ++keyIt) {
            std::string key(keyIt->name.GetString());
            jsonToPropery(data[key], keyIt->value);
        }
    } else if (input.IsArray()) {
        data.convertToList();
        for (auto nodeIt = input.Begin(); nodeIt != input.End(); ++nodeIt) {
            PropertyTree child;
            jsonToPropery(child, *nodeIt);
            data.append(std::move(child));
        }
    } else if (input.IsBool())
        data = PropertyTreeScalar(input.GetBool());
    else if (input.IsDouble())
        data = PropertyTreeScalar(input.GetDouble());
    else if (input.IsInt())
        data = PropertyTreeScalar(std::int64_t(input.GetInt()));
    else if (input.IsInt64())
        data = PropertyTreeScalar(std::int64_t(input.GetInt64()));
    else if (input.IsUint())
        data = PropertyTreeScalar(std::int64_t(input.GetUint()));
    else if (input.IsUint64())
        data = PropertyTreeScalar(std::int64_t(input.GetUint64()));
    else if (input.IsString()) {
        std::string inputString(input.GetString(), input.GetStringLength());
        data = PropertyTreeScalar(std::move(inputString));
    }
}

void propertyToJson(const PropertyTree& data, rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator)
{
    if (data.isNull()) {
    } else if (data.isList()) {
        json.SetArray();
        for (const PropertyTree& child : data.getList()) {
            rapidjson::Value tempValue;
            propertyToJson(child, tempValue, allocator);
            json.PushBack(tempValue, allocator);
        }
    } else if (data.isMap()) {
        json.SetObject();
        for (const auto& p : data.getMap()) {
            const PropertyTree& child = p.second;
            rapidjson::Value    tempValue;
            propertyToJson(child, tempValue, allocator);
            json.AddMember(p.first.c_str(), allocator, tempValue, allocator);
        }
    } else if (data.isScalar()) {
        const auto& scalar = data.getScalar();
        if (const auto* bval = std::get_if<bool>(&scalar); bval)
            json.SetBool(*bval);
        if (const auto* ival = std::get_if<int64_t>(&scalar); ival)
            json.SetInt64(*ival);
        if (const auto* dval = std::get_if<double>(&scalar); dval)
            json.SetDouble(*dval);
        if (const auto* sval = std::get_if<std::string>(&scalar); sval)
            json.SetString(sval->data(), sval->size(), allocator);
    }
}

bool readJsonFromBuffer(const std::string& buffer, PropertyTree& data)
{
    rapidjson::Document input;
    const char*         dataPtr = buffer.data();
    if (buffer.starts_with(std::string_view("\xef\xbb\xbf", 3)))
        dataPtr += 3;
    auto& res = input.Parse<0>(dataPtr);
    if (res.HasParseError()) {
        Logger(Logger::Err) << res.GetParseError();
        return false;
    }

    if (!input.IsObject() && !input.IsArray())
        return false;

    jsonToPropery(data, input);

    return true;
}

bool writeJsonToBuffer(std::string& buffer, const PropertyTree& data)
{
    rapidjson::Document json;
    propertyToJson(data, json, json.GetAllocator());

    JsonStreamOut                    outStream(buffer);
    rapidjson::Writer<JsonStreamOut> writer(outStream);
    json.Accept(writer);

    return true;
}

bool readFileIntoBuffer(const std::string& filename, std::string& buffer)
{
    std::ifstream ifs(string2path(filename), std::ios_base::in | std::ios_base::binary);
    if (!ifs)
        return false;

    ifs.seekg(0, std::ios::end);

    buffer.resize(ifs.tellg());
    ifs.seekg(0, std::ios::beg);
    ifs.read(buffer.data(), buffer.size());
    return true;
}

bool writeFileFromBuffer(const std::string& filename, const std::string& buffer)
{
    std::ofstream ofs(string2path(filename), std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
    if (!ofs)
        return false;

    if (buffer.empty())
        return true;

    ofs.write(buffer.data(), buffer.size());
    if (!ofs)
        return false;
    return true;
}

bool createDirectoriesForFile(const std::string& filename)
{
    auto            path = string2path(filename);
    std::error_code ec;
    if (!std_fs::exists(path.parent_path(), ec))
        std_fs::create_directories(path.parent_path(), ec);
    else 
        return true;
    return !ec;
}

bool createDirectories(const std::string& folder)
{
    auto            path = string2path(folder);
    std::error_code ec;
    if (!std_fs::exists(path, ec))
        std_fs::create_directories(path, ec);
    else 
        return true;
    return !ec;
}

}
