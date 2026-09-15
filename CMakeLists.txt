# Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
# SPDX-License-Identifier: BSL-1.0
# See LICENSE file for details.

# Common declarations
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/install CACHE STRING "" FORCE)

set(Qt6Quick3D_FOUND TRUE CACHE BOOL "" FORCE)
set(Qt6ShaderTools_FOUND TRUE CACHE BOOL "" FORCE)
set(Qt6TaskTree_FOUND TRUE CACHE BOOL "" FORCE)

cmake_minimum_required(VERSION 3.31...4.0)

project(modgen)

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

if (WIN32)
include(CascLib)
include(StormLib)
endif()

include(ext/mernel/mernel_thirdparty.cmake)
include(ext/mernel/mernel.cmake)

# source
set(sourceRoot ${CMAKE_CURRENT_SOURCE_DIR}/src)

AddTarget(TYPE shared NAME ModgenCore
    SOURCE_DIR ${sourceRoot}/CoreLib
	INCLUDES   ${sourceRoot}/CoreLib
    EXPORT_INCLUDES
    LINK_LIBRARIES [ WIN33 ? casclib stormlib ] rapidjson MernelPlatform
    )

function(InstallTextFiles folder)
    set(presetsRootBin ${CMAKE_CURRENT_BINARY_DIR}/${folder})
    file(GLOB_RECURSE presetFiles RELATIVE
        ${CMAKE_CURRENT_SOURCE_DIR}/${folder}
        ${CMAKE_CURRENT_SOURCE_DIR}/${folder}/*.json
        ${CMAKE_CURRENT_SOURCE_DIR}/${folder}/*.txt)
    foreach(filename ${presetFiles})
        add_custom_command(OUTPUT ${presetsRootBin}/${filename}
            COMMAND ${CMAKE_COMMAND} ARGS -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${folder}/${filename} ${presetsRootBin}/${filename}
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${folder}/${filename})
        target_sources(ModgenCore PRIVATE ${presetsRootBin}/${filename})
    endforeach()
    
    install(DIRECTORY ${presetsRootBin} DESTINATION bin)
endfunction()

InstallTextFiles(presets)


# ConsoleApp
AddTarget( TYPE app_console NAME modgen_console
    SOURCE_DIR ${sourceRoot}/ConsoleApp
    LINK_LIBRARIES ModgenCore MernelPlatform
    )

if (NOT Qt6_FOUND)
	return()
endif()


set(CMAKE_AUTOMOC ON)

# QtApp
qt_standard_project_setup()

AddTarget( TYPE app_qt NAME modgen_ui
    SOURCE_DIR ${sourceRoot}/QtApp
    INCLUDES   ${sourceRoot}/QtApp
    LINK_LIBRARIES ModgenCore MernelPlatform
    QT_MODULES Widgets Qml Quick 
    )

file(GLOB qmlFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/qml/*.qml)
file(GLOB cppFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src/QtApp/*.cpp)

qt6_add_qml_module(modgen_ui
    NO_RESOURCE_TARGET_PATH
    URI MainModule
    VERSION 1.0
    QML_FILES
        ${qmlFiles}
        
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
AddTarget( TYPE app_ui NAME modgen
    SOURCE_DIR ${sourceRoot}/Shortcut
    STATIC_RUNTIME
    SKIP_INSTALL
    )
install(TARGETS modgen RUNTIME DESTINATION .)


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
