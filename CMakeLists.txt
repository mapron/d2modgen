# Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
# SPDX-License-Identifier: BSL-1.0
# See LICENSE file for details.

# Common declarations
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/install CACHE STRING "" FORCE)


set(Qt6Quick3D_FOUND TRUE CACHE BOOL "" FORCE)
set(Qt6ShaderTools_FOUND TRUE CACHE BOOL "" FORCE)
set(Qt6TaskTree_FOUND TRUE CACHE BOOL "" FORCE)

option(ENABLE_SCHEMA_GEN "Turn on code-generation for Config.hpp" FALSE)
option(D2MODGEN_BUILD_TESTS "Build unit tests" OFF)

cmake_minimum_required(VERSION 3.31...4.0)

project(modgen VERSION 1.1.0)

set(MERNEL_ROOT ${CMAKE_CURRENT_LIST_DIR}/ext/mernel)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake ${MERNEL_ROOT}/cmake)

if (WIN32)
    add_definitions(-DNOMINMAX -D_UNICODE -DUNICODE -D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -D_USE_MATH_DEFINES)
endif()


# Qt Setup
set(QT_MODULE_LIST Core Gui Widgets Svg Qml Quick
QuickWidgets QuickControls2 QuickLayouts
)
find_package(Qt6 COMPONENTS ${QT_MODULE_LIST} LinguistTools)

include(targets)
include(qt_helpers)

add_definitions(-DCASCLIB_NO_AUTO_LINK_LIBRARY -DSTORMLIB_NO_AUTO_LINK)

include(CascLib)
include(StormLib)

include(ext/mernel/mernel_thirdparty.cmake)
include(ext/mernel/mernel.cmake)

# source
set(sourceRoot ${CMAKE_CURRENT_SOURCE_DIR}/src)

AddTarget(TYPE static NAME Config
    SOURCE_DIR ${sourceRoot}/Config
	INCLUDES   ${sourceRoot}/Config
    [ Qt6_FOUND ? QT_MODULES Core ]
    LINK_LIBRARIES MernelPlatform
    EXPORT_INCLUDES
    COMPILE_DEFINITIONS [ Qt6_FOUND ? USE_QT ]
)
target_sources(Config PRIVATE 
    ${sourceRoot}/Config/Config.yaml
    ${sourceRoot}/Config/AppConfig.yaml
    ${sourceRoot}/Config/update.py
)

if (Python3_EXECUTABLE AND ENABLE_SCHEMA_GEN)
    add_custom_command(OUTPUT "${sourceRoot}/Config/Config.hpp" "${sourceRoot}/Config/ConfigQt.hpp"
                       COMMAND "${Python3_EXECUTABLE}" "${sourceRoot}/Config/update.py" "Config"
                       MAIN_DEPENDENCY "${sourceRoot}/Config/Config.yaml"
                       DEPENDS  "${sourceRoot}/Config/update.py"
                       WORKING_DIRECTORY "${sourceRoot}/Config"
                       COMMENT "Generating Config.hpp from Config.yaml"
                       VERBATIM
                       CODEGEN)
    add_custom_command(OUTPUT "${sourceRoot}/Config/AppConfig.hpp" "${sourceRoot}/Config/AppConfigQt.hpp"
                       COMMAND "${Python3_EXECUTABLE}" "${sourceRoot}/Config/update.py" "AppConfig"
                       MAIN_DEPENDENCY "${sourceRoot}/Config/AppConfig.yaml"
                       DEPENDS  "${sourceRoot}/Config/update.py"
                       WORKING_DIRECTORY "${sourceRoot}/Config"
                       COMMENT "Generating AppConfig.hpp from AppConfig.yaml"
                       VERBATIM
                       CODEGEN)
endif()

AddTarget(TYPE shared NAME ModgenCore
    SOURCE_DIR ${sourceRoot}/CoreLib
	INCLUDES   ${sourceRoot}/CoreLib
    EXPORT_INCLUDES
    LINK_LIBRARIES casclib stormlib rapidjson MernelPlatform Config
    )

# ConsoleApp
AddTarget( TYPE app_console NAME modgen_console
    SOURCE_DIR ${sourceRoot}/ConsoleApp
    LINK_LIBRARIES ModgenCore MernelPlatform Config
    )

# Tests
if (D2MODGEN_BUILD_TESTS)
    enable_testing()
    AddTarget(TYPE app_console NAME CoreTests
        SOURCE_DIR ${sourceRoot}/CoreTests
        LINK_LIBRARIES ModgenCore MernelPlatform gtest_main
        SKIP_INSTALL
        )
    file(COPY ${sourceRoot}/CoreTests/data/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/CoreTestsData)
    add_test(NAME CoreTests COMMAND CoreTests WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()

if (NOT Qt6_FOUND)
	return()
endif()


set(CMAKE_AUTOMOC ON)

# QtApp
qt_standard_project_setup()

AddTarget( TYPE app_qt NAME modgen_ui
    SOURCE_DIR ${sourceRoot}/QtApp
    INCLUDES   ${sourceRoot}/QtApp
    LINK_LIBRARIES ModgenCore MernelPlatform Config
    QT_MODULES Widgets Qml Quick Network
    COMPILE_DEFINITIONS APP_VERSION="${PROJECT_VERSION}"
    EXTRA_SOURCES assets/resource.rc
    )

file(GLOB_RECURSE qmlFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/qml/*.qml)
file(GLOB_RECURSE jsFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/qml/*.js)
file(GLOB cppFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src/QtApp/*.cpp)

qt6_add_qml_module(modgen_ui
    NO_RESOURCE_TARGET_PATH
    URI MainModule
    VERSION 1.0
    QML_FILES
        ${qmlFiles}
        ${jsFiles}
    RESOURCES
        assets/logo.png
)

qt_add_translations(modgen_ui
    TS_FILES
        translations/modgen_ru_RU.ts
        translations/modgen_ko_KR.ts
    SOURCES
        ${qmlFiles}
        ${cppFiles}
    RESOURCE_PREFIX 
        "/i18n"
)

# a "shortcut"
if (WIN32)
AddTarget( TYPE app_ui NAME modgen
    SOURCE_DIR ${sourceRoot}/Shortcut
    STATIC_RUNTIME
    SKIP_INSTALL
    EXTRA_SOURCES assets/resource.rc
    )
install(TARGETS modgen RUNTIME DESTINATION .)
endif()


#[[
########## Installation (Win32 only at the moment) ##########
#]]

if(WIN32)

set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
if (NOT(CMAKE_BUILD_TYPE STREQUAL Release))
    set(CMAKE_INSTALL_DEBUG_LIBRARIES TRUE)
endif()
include(InstallRequiredSystemLibraries)

include(CMakePackageConfigHelpers)

qt6_generate_deploy_qml_app_script(
    TARGET modgen_ui
    OUTPUT_SCRIPT deploy_script
   # INCLUDE_PLUGINS qsvg
    NO_TRANSLATIONS
    NO_COMPILER_RUNTIME

    DEPLOY_TOOL_OPTIONS
        --no-system-dxc-compiler
)

install(SCRIPT ${deploy_script})

endif(WIN32)
