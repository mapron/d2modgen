/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "DataContext.hpp"
#include "TableUtils.hpp"

#include <QFile>

namespace D2ModGen {

namespace {

bool writeCSV(QString& csvData, const Table& table)
{
    csvData += table.columns.join('\t') + "\r\n";
    for (const auto& row : table.rows)
        csvData += row.data.join('\t') + "\r\n";
    return true;
}

bool readCSV(const QString& csvData, Table& table)
{
    QStringList rows = csvData.split("\r\n", Qt::SkipEmptyParts);
    if (rows.isEmpty())
        return false;
    const QString header = rows.takeFirst();
    table.columns        = header.split('\t', Qt::KeepEmptyParts);
    for (const QString& row : rows) {
        const QStringList cells = row.split('\t', Qt::KeepEmptyParts);
        table.rows << TableRow{ cells };
    }
#ifndef NDEBUG
    {
        QString check;
        writeCSV(check, table);
        assert(check == csvData);
    }
#endif
    return true;
}

}

bool DataContext::readData(const IStorage::StoredData& data)
{
    for (const auto& fileData : data.tables) {
        if (fileData.id.isEmpty()) {
            qWarning() << "Empty csv id found!";
            return false;
        }

        QString data = QString::fromUtf8(fileData.data);
        Table   table;
        table.id = fileData.id;
        if (!readCSV(data, table)) {
            qWarning() << "failed to parse csv:" << fileData.id;
            return false;
        }
        tableSet.tables[fileData.id] = std::move(table);
        tableSet.relativeNames.insert(IStorage::makeTableRelativePath(fileData.id, false));
    }
    for (const auto& fileData : data.inMemoryFiles) {
        if (fileData.data.isEmpty()) {
            qWarning() << "Json data is empty:" << fileData.relFilepath;
            return false;
        }

        QJsonParseError err;
        auto            loadDoc(QJsonDocument::fromJson(fileData.data, &err));
        if (loadDoc.isNull()) {
            qWarning() << "failed to parse json:" << fileData.relFilepath << ", " << err.errorString();
            return false;
        }
        if (jsonFiles.contains(fileData.relFilepath)) {
            qWarning() << "duplicate json file found:" << fileData.relFilepath;
            return false;
        }

        jsonFiles[fileData.relFilepath] = qjsonToProperty(loadDoc);
    }
    for (const auto& fileData : data.refFiles) {
        if (!QFile::exists(fileData.absSrcFilepath)) {
            qWarning() << "Non-existent file:" << fileData.absSrcFilepath;
            return false;
        }
        if (jsonFiles.contains(fileData.relFilepath)) {
            qWarning() << "File for plain copy already exists in json list:" << fileData.relFilepath;
            return false;
        }
        if (tableSet.relativeNames.contains(fileData.relFilepath)) {
            qWarning() << "File for plain copy already exists in CSV list:" << fileData.relFilepath;
            return false;
        }
        if (copyFiles.contains(fileData.relFilepath)) {
            qWarning() << "duplicate copy file found:" << fileData.relFilepath;
            return false;
        }
        copyFiles[fileData.relFilepath] = fileData;
    }

    return true;
}

bool DataContext::writeData(IStorage::StoredData& data) const
{
    for (const auto& p : tableSet.tables) {
        const Table& table = p.second;
        if (!table.modified && !table.forceOutput)
            continue;

        QString tableStr;
        if (!writeCSV(tableStr, table))
            return false;
        data.tables.push_back(IStorage::StoredFileTable{ tableStr.toUtf8(), table.id });
    }
    for (const auto& p : jsonFiles) {
        QJsonDocument jdoc    = propertyToDoc(p.second);
        QByteArray    datastr = jdoc.toJson(QJsonDocument::Indented);
        datastr.replace(QByteArray("\xC3\x83\xC2\xBF\x63"), QByteArray("\xC3\xBF\x63")); // hack: replace color codes converter to UTF-8 from latin-1.
        data.inMemoryFiles.push_back(IStorage::StoredFileMemory{ datastr, p.first });
    }
    for (const auto& p : copyFiles) {
        data.refFiles.push_back(p.second);
    }
    return true;
}

bool DataContext::mergeWith(const DataContext& source, ConflictPolicy policy)
{
    for (const auto& p : source.tableSet.tables) {
        const Table& sourceTable = p.second;
        if (!tableSet.tables.contains(sourceTable.id)) {
            qWarning() << "unknown table id:" << sourceTable.id;
            continue;
        }
        Table& destTable = tableSet.tables[sourceTable.id];
        if (destTable.modified) {
            if (policy == ConflictPolicy::RaiseError)
                return false;
            if (policy == ConflictPolicy::Skip)
                continue;
        }

        const TableView sourceView(sourceTable);
        TableView       destView(destTable);
        destTable.modified = true;

        // already modified, need to resolve conflicts:
        switch (policy) {
            case ConflictPolicy::Skip:
            case ConflictPolicy::RaiseError:
            case ConflictPolicy::Replace: // clear previous data
            {
                destView.clear();
                destView.concat(sourceView);
                break;
            }
            case ConflictPolicy::Append: // place all data at the end
            {
                destView.concat(sourceView);
                break;
            }
            case ConflictPolicy::Update: // overwrite records with same key
            {
                if (!destView.createRowIndex())
                    return false;

                destView.merge(sourceView, false, true);
                break;
            }
            case ConflictPolicy::AppendNew: // place new data at the end
            {
                if (!destView.createRowIndex())
                    return false;

                destView.merge(sourceView, true, false);
                break;
            }
            case ConflictPolicy::Merge: // try Update, then AppendNew
            {
                if (!destView.createRowIndex())
                    return false;

                destView.merge(sourceView, true, true);
                break;
            }
        };
    }

    for (const auto& p : source.jsonFiles) {
        if (jsonFiles.contains(p.first)) {
            if (policy == ConflictPolicy::RaiseError)
                return false;
            if (policy == ConflictPolicy::Skip)
                continue;
        }
        if (copyFiles.contains(p.first)) {
            if (policy == ConflictPolicy::Skip)
                continue;
            return false;
        }
        jsonFiles[p.first] = p.second;
    }

    for (const auto& p : source.copyFiles) {
        if (copyFiles.contains(p.first)) {
            if (policy == ConflictPolicy::RaiseError)
                return false;
            if (policy == ConflictPolicy::Skip)
                continue;
        }
        if (jsonFiles.contains(p.first)) {
            if (policy == ConflictPolicy::Skip)
                continue;
            return false;
        }
        if (tableSet.relativeNames.contains(p.first.toLower())) {
            if (policy == ConflictPolicy::Skip)
                continue;
            return false;
        }
        copyFiles[p.first] = p.second;
    }

    return true;
}

PropertyTree DataContext::qjsonToProperty(const QJsonDocument& doc)
{
    if (doc.isNull())
        return {};
    return doc.isArray() ? qjsonToProperty(doc.array()) : qjsonToProperty(doc.object());
}

PropertyTree DataContext::qjsonToProperty(const QJsonObject& value)
{
    PropertyTree result;
    result.convertToMap();
    for (auto it = value.constBegin(); it != value.constEnd(); ++it)
        result.insert(it.key().toStdString(), qjsonToProperty(it.value()));

    return result;
}

PropertyTree DataContext::qjsonToProperty(const QJsonArray& value)
{
    PropertyTree result;
    result.convertToList();
    for (const QJsonValue& iter : value)
        result.append(qjsonToProperty(iter));

    return result;
}

PropertyTree DataContext::qjsonToProperty(const QJsonValue& value)
{
    if (value.isNull())
        return PropertyTree{};
    if (value.isBool())
        return PropertyTreeScalar(value.toBool());
    if (value.isDouble())
        return PropertyTreeScalar(value.toDouble());
    if (value.isString())
        return PropertyTreeScalar(value.toString().toStdString());
    if (value.isArray())
        return qjsonToProperty(value.toArray());
    if (value.isObject())
        return qjsonToProperty(value.toObject());
    return {};
}

QJsonValue DataContext::propertyToQjson(const PropertyTree& value)
{
    if (value.isNull())
        return QJsonValue();
    if (value.isScalar()) {
        const auto& sc = value.getScalar();
        if (const auto* bval = std::get_if<bool>(&sc); bval)
            return *bval;
        if (const auto* ival = std::get_if<int64_t>(&sc); ival)
            return *ival;
        if (const auto* dval = std::get_if<double>(&sc); dval)
            return *dval;
        if (const auto* sval = std::get_if<std::string>(&sc); sval)
            return QString::fromStdString(*sval);
    }
    if (value.isList()) {
        QJsonArray arr;
        for (const auto& child : value.getList())
            arr << propertyToQjson(child);
        return arr;
    }
    if (value.isMap()) {
        QJsonObject obj;
        for (const auto& child : value.getMap())
            obj[child.first.c_str()] = propertyToQjson(child.second);
        return obj;
    }
    return {};
}

QJsonDocument DataContext::propertyToDoc(const PropertyTree& value)
{
    QJsonValue jdata = propertyToQjson(value);
    if (jdata.isArray())
        return QJsonDocument(jdata.toArray());
    else if (jdata.isObject())
        return QJsonDocument(jdata.toObject());
    return {};
}

}
