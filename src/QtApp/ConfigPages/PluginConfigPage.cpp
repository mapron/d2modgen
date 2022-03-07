/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */

#include "PluginConfigPage.hpp"

#include "Formats/FileFormatJson.hpp"

#include <QQuickWidget>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QJSValue>
#include <QUrl>

namespace D2ModGen {

namespace {
//PropertyTree qjsonToProperty(const QJsonDocument& doc)
//{
//    if (doc.isNull())
//        return {};
//    return doc.isArray() ? qjsonToProperty(doc.array()) : qjsonToProperty(doc.object());
//}
PropertyTree qjsonToProperty(const QVariant& value);
PropertyTree qjsonToProperty(const QVariantMap& value)
{
    PropertyTree result;
    result.convertToMap();
    for (auto it = value.constBegin(); it != value.constEnd(); ++it)
        result.insert(it.key().toStdString(), qjsonToProperty(it.value()));

    return result;
}

PropertyTree qjsonToProperty(const QVariantList& value)
{
    PropertyTree result;
    result.convertToList();
    for (const QVariant& iter : value)
        result.append(qjsonToProperty(iter));

    return result;
}

PropertyTree qjsonToProperty(const QVariant& value)
{
    if (value.isNull())
        return PropertyTree{};
    if (value.type() == QVariant::Bool)
        return PropertyTreeScalar(value.toBool());
    if (value.type() == QVariant::Int)
        return PropertyTreeScalar(value.toInt());
    if (value.type() == QVariant::Double)
        return PropertyTreeScalar(value.toDouble());
    if (value.type() == QVariant::String)
        return PropertyTreeScalar(value.toString().toStdString());
    if (value.type() == QVariant::List)
        return qjsonToProperty(value.toList());
    if (value.type() == QVariant::Map)
        return qjsonToProperty(value.toMap());
    qWarning() << "unhandled type:" << value;
    return {};
}

QVariant propertyToQjson(const PropertyTree& value)
{
    if (value.isNull())
        return QVariant();
    if (value.isScalar()) {
        const auto& sc = value.getScalar();
        if (sc.isBool())
            return QVariant(sc.toBool());
        if (sc.isInt())
            return sc.toInt();
        if (sc.isDouble())
            return sc.toDouble();
        if (sc.isString())
            return QString::fromStdString(sc.toString());
    }
    if (value.isList()) {
        QList<QVariant> arr;
        for (const auto& child : value.getList())
            arr << propertyToQjson(child);
        return arr;
    }
    if (value.isMap()) {
        QMap<QString, QVariant> obj;
        for (const auto& child : value.getMap())
            obj[child.first.c_str()] = propertyToQjson(child.second);
        return obj;
    }
    return {};
}
}

PluginConfigPage::PluginConfigPage(const IModule::Ptr& module, QWidget* parent)
    : ConfigPageAbstract(module, parent)
{
    auto info = getModule().pluginInfo();
    if (info.value("hasQML", true).toBool()) {
        auto    root    = info.value("root", "").toString();
        auto    id      = info.value("id", "").toString();
        QString qmlPath = QString::fromStdString(root + "/" + id + ".qml");
        m_quick         = new QQuickWidget(this);
        m_quick->setSource(QUrl::fromLocalFile(qmlPath));
        m_quick->setClearColor("#31363b");
        m_quick->engine()->rootContext()->setContextProperty("parentWidget", this);
        addWidget(m_quick);
        m_quick->show();
    }
    //closeLayout();
}

PluginConfigPage::~PluginConfigPage()
{
}

void PluginConfigPage::qmlDataChanged()
{
    emit dataChanged();
}

QString PluginConfigPage::caption() const
{
    return QString::fromStdString(getModule().settingKey()).mid(7);
}

void PluginConfigPage::updateUIFromSettings(const PropertyTree& data)
{
    {
        //std::string buffer;
        //writeJsonToBuffer(buffer, data);
        //qDebug() << "READ JSON data:" << buffer.c_str();
    }
    
    QVariant value = propertyToQjson(data);
    if (data.isNull())
        value = QVariantMap{};
    //qDebug() << "READ QML variant:" << value;
    QObject* rootItem = m_quick->rootObject();
    QMetaObject::invokeMethod(rootItem, "setFormValues", Q_ARG(QVariant, value));
}

void PluginConfigPage::writeSettingsFromUI(PropertyTree& data) const
{
    QVariant  returnedValue;
    QObject* rootItem = m_quick->rootObject();
    QMetaObject::invokeMethod(rootItem, "getFormValues", Q_RETURN_ARG(QVariant, returnedValue));

    //qDebug() << "WRITE QML variant:" << returnedValue;
    auto jsv = returnedValue.value<QJSValue>();
    auto var = jsv.toVariant();
    data = qjsonToProperty(var);
    {
        //std::string buffer;
        //writeJsonToBuffer(buffer, data);
       // qDebug() << "WRITE JSON data:" << buffer.c_str();
    }
}

}
