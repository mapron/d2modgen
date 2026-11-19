#pragma once
// Do not edit, this file was generated from AppConfig.yaml using update.py

#include "StructBaseCpp.hpp"

namespace D2ModGen {

struct AppConfig : public CommonStruct {
    int64_t m_d2rLocaleId{ 0 };

    int64_t m_inputMode{ 0 };

    std::string m_inputPath{};

    std::string m_modname{ "rando" };

    int64_t m_outputMode{ 0 };

    std::string m_outputPath{};

    bool m_refreshSeed{ false };

    int64_t m_versionD2R{ 1 };

    int64_t m_versionMajor{ 1 };

    std::string m_extraTrace{ "" };

    std::string m_langId{ "en_US" };

    std::string m_themeId{ "dark" };

    std::string m_userHome{ "" };

    std::string m_runnerTemplate{ "" };

    std::string m_installLocationLegacy{ "" };

    std::string m_installLocationD2R{ "" };

    std::string m_saveSubfolder{ "Saved Games/Diablo II Resurrected/" };

    std::string m_bnetConfig{ "AppData/Roaming/Battle.net/Battle.net.config" };

    AppConfig()
    {
        addScalar("d2rLocaleId", m_d2rLocaleId).setRange(0, 14).setZero(0);
        addScalar("inputMode", m_inputMode).setRange(0, 2).setZero(0);
        addScalar("inputPath", m_inputPath);
        addScalar("modname", m_modname).setZero("rando");
        addScalar("outputMode", m_outputMode).setRange(0, 3).setZero(0);
        addScalar("outputPath", m_outputPath);
        addScalar("refreshSeed", m_refreshSeed).setZero(false);
        addScalar("versionD2R", m_versionD2R).setRange(0, 1).setZero(1);
        addScalar("versionMajor", m_versionMajor).setRange(0, 1).setZero(1);
        addScalar("extraTrace", m_extraTrace).setZero("");
        addScalar("langId", m_langId).setZero("en_US");
        addScalar("themeId", m_themeId).setZero("dark");
        addScalar("userHome", m_userHome).setZero("");
        addScalar("runnerTemplate", m_runnerTemplate).setZero("");
        addScalar("installLocationLegacy", m_installLocationLegacy).setZero("").setConstant();
        addScalar("installLocationD2R", m_installLocationD2R).setZero("").setConstant();
        addScalar("saveSubfolder", m_saveSubfolder).setZero("Saved Games/Diablo II Resurrected/").setConstant();
        addScalar("bnetConfig", m_bnetConfig).setZero("AppData/Roaming/Battle.net/Battle.net.config").setConstant();
    }
};
}
