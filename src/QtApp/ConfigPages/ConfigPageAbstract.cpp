/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageAbstract.hpp"

#include <QVBoxLayout>
#include <QLabel>

namespace D2ModGen {

ConfigPageAbstract::ConfigPageAbstract(const std::string& localeId, const IModule::Ptr& module, QWidget* parent)
    : IConfigPage(parent)
    , m_module(module)
    , m_localeId(localeId)
{
    assert(m_module);
    m_layout = new QVBoxLayout(this);
    m_layout->setMargin(0);

    const auto& info = module->pluginInfo();

    const PropertyTree  emptyMap       = []() { PropertyTree val; val.convertToMap(); return val; }();
    const PropertyTree  emptyList      = []() { PropertyTree val; val.convertToList(); return val; }();
    const PropertyTree& allLocales     = info.contains("locales") ? info["locales"] : emptyMap;
    const PropertyTree& mainLocaleData = allLocales.contains(localeId) ? allLocales[localeId] : emptyMap;
    const PropertyTree& enLocaleData   = allLocales.contains("en_US") ? allLocales["en_US"] : emptyMap;

    const PropertyTree& mainLocaleDataTitles   = mainLocaleData.contains("controlsTitles") ? mainLocaleData["controlsTitles"] : emptyMap;
    const PropertyTree& mainLocaleDataSuffixes = mainLocaleData.contains("controlsSuffixes") ? mainLocaleData["controlsSuffixes"] : emptyMap;
    const PropertyTree& mainLocaleDataHelps    = mainLocaleData.contains("controlsHelps") ? mainLocaleData["controlsHelps"] : emptyMap;

    const PropertyTree& enLocaleDataTitles   = enLocaleData.contains("controlsTitles") ? enLocaleData["controlsTitles"] : emptyMap;
    const PropertyTree& enLocaleDataSuffixes = enLocaleData.contains("controlsSuffixes") ? enLocaleData["controlsSuffixes"] : emptyMap;
    const PropertyTree& enLocaleDataHelps    = enLocaleData.contains("controlsHelps") ? enLocaleData["controlsHelps"] : emptyMap;

    auto readLocList = [](const PropertyTree& mainData, const PropertyTree& enData, const std::string& key) -> QStringList {
        auto readOneValue = [](const PropertyTree& data, const std::string& key) -> QStringList {
            if (!data.contains(key))
                return {};
            const PropertyTree& child = data[key];
            if (child.isList()) {
                QStringList result;
                for (const PropertyTree& grandchild : child.getList()) {
                    auto v = grandchild.getScalar().toString();
                    if (!v.empty())
                        result << QString::fromStdString(v);
                }
                return result;
            }
            if (child.isScalar() && !child.getScalar().toString().empty())
                return { QString::fromStdString(child.getScalar().toString()) };
            return {};
        };
        const auto mainValue = readOneValue(mainData, key);
        if (!mainValue.isEmpty())
            return mainValue;
        const auto enValue = readOneValue(enData, key);
        if (!enValue.isEmpty())
            return enValue;

        return {};
    };
    auto readLoc = [&readLocList](const PropertyTree& mainData, const PropertyTree& enData, const std::string& key, const QString& defaultValue) -> QString {
        auto l = readLocList(mainData, enData, key);
        return l.isEmpty() ? defaultValue : l.join("\n");
    };
    auto readStringList = [&readLocList](const PropertyTree& mainData, const PropertyTree& enData, size_t expectedSize) {
        const QStringList main = readLocList(mainData, enData, "presets");
        QStringList       result;
        for (size_t i = 0; i < expectedSize; ++i)
            result << main.value(i, QString("Preset #%1").arg(i));

        return result;
    };
    auto qid = QString::fromStdString(module->settingKey());

    m_title = readLoc(mainLocaleData, enLocaleData, "title", qid);
    m_help  = readLoc(mainLocaleData, enLocaleData, "help", "");

    if (info.contains("presets")) {
        const QStringList names = readStringList(mainLocaleData, enLocaleData, info["presets"].getList().size());
        int               i     = 0;
        for (const auto& presetData : info["presets"].getList()) {
            Preset preset;
            preset.m_title = names.value(i);
            preset.m_data  = presetData;
            m_presets.push_back(preset);
            i++;
        }
    }

    if (info.contains("controls")) {
        auto controlsList = info["controls"].getList();
        for (const PropertyTree& controlData : controlsList) {
            std::string          type = controlData.value("type", "").toString();
            std::string          id   = controlData.value("id", "").toString();
            IValueWidget::Params hint;
            hint.m_compact = controlData.value("compact", false).toBool();
            if (type == "sliderMinMax") {
                hint.m_control = IValueWidget::Control::SliderMinMax;
                hint.m_min     = controlData.value("min", 0).toInt();
                hint.m_max     = controlData.value("max", 100).toInt();
            } else if (type == "slider") {
                hint.m_control = IValueWidget::Control::Slider;
                hint.m_mult    = controlData.value("mult", 10.).toDouble();
                hint.m_denom   = controlData.value("denom", 10.).toDouble();
            } else if (type == "checkbox") {
                hint.m_control = IValueWidget::Control::CheckBox;
            } else if (type == "lineedit") {
                hint.m_control = IValueWidget::Control::LineEdit;
            } else if (type == "label") {
                hint.m_control = IValueWidget::Control::Label;
            }
            hint.m_title = readLoc(mainLocaleDataTitles, enLocaleDataTitles, id, QString::fromStdString(id));
            auto sub     = readLocList(mainLocaleDataSuffixes, enLocaleDataSuffixes, id);
            if (!sub.isEmpty())
                hint.m_title += "\n" + sub.join("\n");
            hint.m_help = readLoc(mainLocaleDataHelps, enLocaleDataHelps, id, "");
            m_uiHints.push_back({ id, hint });
        }
    }
}

IConfigPage::PresetList ConfigPageAbstract::pagePresets() const
{
    return m_presets;
}

QString ConfigPageAbstract::caption() const
{
    return m_title;
}

QString ConfigPageAbstract::pageHelp() const
{
    return m_help;
}

void ConfigPageAbstract::updateUIFromSettings(const PropertyTree& data)
{
    for (const auto& p : m_editors) {
        const auto& key = p.first;
        auto*       w   = p.second;
        assert(data.contains(key));
        if (data.contains(key))
            w->setValue(data[key]);
    }
}

void ConfigPageAbstract::writeSettingsFromUI(PropertyTree& data) const
{
    for (const auto& p : m_editors) {
        const auto& key = p.first;
        auto*       w   = p.second;
        data.insert(key, w->getValue());
    }
}

void ConfigPageAbstract::addWidget(QWidget* w)
{
    m_layout->addWidget(w);
}

IValueWidget* ConfigPageAbstract::makeEditor(const IValueWidget::Params& params)
{
    using Control = IValueWidget::Control;
    // const IModule::UiControlHint& hint = params.m_hint;

    if (params.m_control == Control::SliderMinMax)
        return new SliderWidgetMinMax(params, this);
    if (params.m_control == Control::Slider)
        return new SliderWidget(params, this);
    if (params.m_control == Control::LineEdit)
        return new LineWidget(params, this);
    if (params.m_control == Control::CheckBox)
        return new CheckboxWidget(params, this);

    assert(false);
    return nullptr;
}

void ConfigPageAbstract::makeWidget(const std::string& id, const IValueWidget::Params& params)
{
    if (params.m_control == IValueWidget::Control::Label) {
        addWidget(new QLabel(params.m_title, this));
        return;
    }

    auto* w = makeEditor(params);
    addWidget(w);
    m_editors[id] = w;
    connect(w, &IValueWidget::dataChanged, this, &IConfigPage::dataChanged);
}

void ConfigPageAbstract::makeAllEditors()
{
    for (const auto& p : m_uiHints)
        makeWidget(p.first, p.second);
}

void ConfigPageAbstract::closeLayout()
{
    m_layout->addStretch();
}

}
