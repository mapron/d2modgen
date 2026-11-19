#pragma once
// Do not edit, this file was generated from AppConfig.yaml using update.py

#include "AppConfig.hpp"
#include "StructBaseQt.hpp"

namespace D2ModGen {

class AppConfig_Qt : public CommonStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(PropertyInt_Qt* d2rLocaleId READ get_d2rLocaleId CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* inputMode READ get_inputMode CONSTANT)
    Q_PROPERTY(PropertyString_Qt* inputPath READ get_inputPath CONSTANT)
    Q_PROPERTY(PropertyString_Qt* modname READ get_modname CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* outputMode READ get_outputMode CONSTANT)
    Q_PROPERTY(PropertyString_Qt* outputPath READ get_outputPath CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* refreshSeed READ get_refreshSeed CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* versionD2R READ get_versionD2R CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* versionMajor READ get_versionMajor CONSTANT)
    Q_PROPERTY(PropertyString_Qt* extraTrace READ get_extraTrace CONSTANT)
    Q_PROPERTY(PropertyString_Qt* langId READ get_langId CONSTANT)
    Q_PROPERTY(PropertyString_Qt* themeId READ get_themeId CONSTANT)
    Q_PROPERTY(PropertyString_Qt* userHome READ get_userHome CONSTANT)
    Q_PROPERTY(PropertyString_Qt* runnerTemplate READ get_runnerTemplate CONSTANT)
    Q_PROPERTY(ConstantPropertyString_Qt* installLocationLegacy READ get_installLocationLegacy CONSTANT)
    Q_PROPERTY(ConstantPropertyString_Qt* installLocationD2R READ get_installLocationD2R CONSTANT)
    Q_PROPERTY(ConstantPropertyString_Qt* saveSubfolder READ get_saveSubfolder CONSTANT)
    Q_PROPERTY(ConstantPropertyString_Qt* bnetConfig READ get_bnetConfig CONSTANT)

public:
    explicit AppConfig_Qt(AppConfig& data)
        : CommonStruct_Qt(data)
        , m_data(data)
        , m_d2rLocaleId(data.getPropertyInt(0))
        , m_inputMode(data.getPropertyInt(1))
        , m_inputPath(data.getPropertyString(0))
        , m_modname(data.getPropertyString(1))
        , m_outputMode(data.getPropertyInt(2))
        , m_outputPath(data.getPropertyString(2))
        , m_refreshSeed(data.getPropertyBool(0))
        , m_versionD2R(data.getPropertyInt(3))
        , m_versionMajor(data.getPropertyInt(4))
        , m_extraTrace(data.getPropertyString(3))
        , m_langId(data.getPropertyString(4))
        , m_themeId(data.getPropertyString(5))
        , m_userHome(data.getPropertyString(6))
        , m_runnerTemplate(data.getPropertyString(7))
        , m_installLocationLegacy(data.getPropertyString(8))
        , m_installLocationD2R(data.getPropertyString(9))
        , m_saveSubfolder(data.getPropertyString(10))
        , m_bnetConfig(data.getPropertyString(11))
    {
    }

    PropertyInt_Qt* get_d2rLocaleId() { return &m_d2rLocaleId; }

    PropertyInt_Qt* get_inputMode() { return &m_inputMode; }

    PropertyString_Qt* get_inputPath() { return &m_inputPath; }

    PropertyString_Qt* get_modname() { return &m_modname; }

    PropertyInt_Qt* get_outputMode() { return &m_outputMode; }

    PropertyString_Qt* get_outputPath() { return &m_outputPath; }

    PropertyBool_Qt* get_refreshSeed() { return &m_refreshSeed; }

    PropertyInt_Qt* get_versionD2R() { return &m_versionD2R; }

    PropertyInt_Qt* get_versionMajor() { return &m_versionMajor; }

    PropertyString_Qt* get_extraTrace() { return &m_extraTrace; }

    PropertyString_Qt* get_langId() { return &m_langId; }

    PropertyString_Qt* get_themeId() { return &m_themeId; }

    PropertyString_Qt* get_userHome() { return &m_userHome; }

    PropertyString_Qt* get_runnerTemplate() { return &m_runnerTemplate; }

    ConstantPropertyString_Qt* get_installLocationLegacy() { return &m_installLocationLegacy; }

    ConstantPropertyString_Qt* get_installLocationD2R() { return &m_installLocationD2R; }

    ConstantPropertyString_Qt* get_saveSubfolder() { return &m_saveSubfolder; }

    ConstantPropertyString_Qt* get_bnetConfig() { return &m_bnetConfig; }

    Q_INVOKABLE void assign(AppConfig_Qt* another) { m_data.assign(another->m_data, true); }

private:
    AppConfig& m_data;

    PropertyInt_Qt m_d2rLocaleId;

    PropertyInt_Qt m_inputMode;

    PropertyString_Qt m_inputPath;

    PropertyString_Qt m_modname;

    PropertyInt_Qt m_outputMode;

    PropertyString_Qt m_outputPath;

    PropertyBool_Qt m_refreshSeed;

    PropertyInt_Qt m_versionD2R;

    PropertyInt_Qt m_versionMajor;

    PropertyString_Qt m_extraTrace;

    PropertyString_Qt m_langId;

    PropertyString_Qt m_themeId;

    PropertyString_Qt m_userHome;

    PropertyString_Qt m_runnerTemplate;

    ConstantPropertyString_Qt m_installLocationLegacy;

    ConstantPropertyString_Qt m_installLocationD2R;

    ConstantPropertyString_Qt m_saveSubfolder;

    ConstantPropertyString_Qt m_bnetConfig;
};

}
