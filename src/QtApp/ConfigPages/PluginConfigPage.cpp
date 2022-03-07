/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */

#include "PluginConfigPage.hpp"

#include <QQuickWidget>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QJSValue>
#include <QUrl>

namespace D2ModGen {

namespace {
PropertyTree qvariantToProperty(const QVariant& value);
PropertyTree qvariantToProperty(const QVariantMap& value)
{
    PropertyTree result;
    result.convertToMap();
    for (auto it = value.constBegin(); it != value.constEnd(); ++it)
        result.insert(it.key().toStdString(), qvariantToProperty(it.value()));

    return result;
}

PropertyTree qvariantToProperty(const QVariantList& value)
{
    PropertyTree result;
    result.convertToList();
    for (const QVariant& iter : value)
        result.append(qvariantToProperty(iter));

    return result;
}

PropertyTree qvariantToProperty(const QVariant& value)
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
        return qvariantToProperty(value.toList());
    if (value.type() == QVariant::Map)
        return qvariantToProperty(value.toMap());
    qWarning() << "unhandled type:" << value;
    return {};
}

QVariant propertyToQVariant(const PropertyTree& value)
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
            arr << propertyToQVariant(child);
        return arr;
    }
    if (value.isMap()) {
        QMap<QString, QVariant> obj;
        for (const auto& child : value.getMap())
            obj[child.first.c_str()] = propertyToQVariant(child.second);
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
        m_quick->setResizeMode(QQuickWidget::ResizeMode::SizeRootObjectToView);
        m_quick->engine()->rootContext()->setContextProperty("parentWidget", this);
        addWidget(m_quick);
        m_quick->show();
    } else {
        StringVector keys;
        auto         def = getModule().defaultValues();
        if (def.isMap()) {
            for (const auto& p : def.getMap())
                keys.push_back(p.first);
        }
        addEditors(makeEditors(keys));
    }
    closeLayout();
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
    if (m_quick) {
        QVariant value = propertyToQVariant(data);
        //qDebug() << "READ QML variant:" << value;
        QObject* rootItem = m_quick->rootObject();
        QMetaObject::invokeMethod(rootItem, "setFormValues", Q_ARG(QVariant, value));
    } else {
        ConfigPageAbstract::updateUIFromSettings(data);
    }
}

void PluginConfigPage::writeSettingsFromUI(PropertyTree& data) const
{
    if (m_quick) {
        QVariant returnedValue;
        QObject* rootItem = m_quick->rootObject();
        QMetaObject::invokeMethod(rootItem, "getFormValues", Q_RETURN_ARG(QVariant, returnedValue));

        //qDebug() << "WRITE QML variant:" << returnedValue;
        auto jsv = returnedValue.value<QJSValue>();
        auto var = jsv.toVariant();
        data     = qvariantToProperty(var);
    } else {
        ConfigPageAbstract::writeSettingsFromUI(data);
    }
}

}
