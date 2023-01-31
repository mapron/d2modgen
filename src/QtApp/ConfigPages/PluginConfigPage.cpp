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
Mernel::PropertyTree qvariantToProperty(const QVariant& value);
Mernel::PropertyTree qvariantToProperty(const QVariantMap& value)
{
    Mernel::PropertyTree result;
    result.convertToMap();
    for (auto it = value.constBegin(); it != value.constEnd(); ++it)
        result.insert(it.key().toStdString(), qvariantToProperty(it.value()));

    return result;
}

Mernel::PropertyTree qvariantToProperty(const QVariantList& value)
{
    Mernel::PropertyTree result;
    result.convertToList();
    for (const QVariant& iter : value)
        result.append(qvariantToProperty(iter));

    return result;
}

Mernel::PropertyTree qvariantToProperty(const QVariant& value)
{
    if (value.isNull())
        return Mernel::PropertyTree{};
    if (value.type() == QVariant::Bool)
        return Mernel::PropertyTreeScalar(value.toBool());
    if (value.type() == QVariant::Int)
        return Mernel::PropertyTreeScalar(value.toInt());
    if (value.type() == QVariant::Double)
        return Mernel::PropertyTreeScalar(value.toDouble());
    if (value.type() == QVariant::String)
        return Mernel::PropertyTreeScalar(value.toString().toStdString());
    if (value.type() == QVariant::List)
        return qvariantToProperty(value.toList());
    if (value.type() == QVariant::Map)
        return qvariantToProperty(value.toMap());
    qWarning() << "unhandled type:" << value;
    return {};
}

QVariant propertyToQVariant(const Mernel::PropertyTree& value)
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

PluginConfigPage::PluginConfigPage(const std::string& localeId, const IModule::Ptr& module, QWidget* parent)
    : ConfigPageAbstract(localeId, module, parent)
{
    auto info = getModule().pluginInfo();
    if (info.value("hasQML", Mernel::PropertyTreeScalar(true)).toBool()) {
        auto    root    = info.value("root", Mernel::PropertyTreeScalar("")).toString();
        auto    id      = info.value("id", Mernel::PropertyTreeScalar("")).toString();
        QString qmlPath = QString::fromStdString(root + "/" + id + ".qml");
        m_quick         = new QQuickWidget(this);
        m_quick->setSource(QUrl::fromLocalFile(qmlPath));
        m_quick->setClearColor("#31363b");
        m_quick->setResizeMode(QQuickWidget::ResizeMode::SizeRootObjectToView);
        m_quick->engine()->rootContext()->setContextProperty("parentWidget", this);
        addWidget(m_quick);
        m_quick->show();
    } else {
        makeAllEditors();
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

void PluginConfigPage::updateUIFromSettings(const Mernel::PropertyTree& data)
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

void PluginConfigPage::writeSettingsFromUI(Mernel::PropertyTree& data) const
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
