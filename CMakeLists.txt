# Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
# SPDX-License-Identifier: MIT
# See LICENSE file for details.

# Common declarations
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/install CACHE STRING "" FORCE)

cmake_minimum_required(VERSION 3.12...3.19)

project(modgen)

set(MERNEL_ROOT ${CMAKE_CURRENT_LIST_DIR}/ext/mernel)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake ${MERNEL_ROOT}/cmake)


option(ENABLE_TEST_PLUGINS "Enable plugins for testing" OFF)

if (WIN32)
    add_definitions(-DNOMINMAX -D_UNICODE -DUNICODE -D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -D_USE_MATH_DEFINES)
endif()


# Qt Setup
set(QT_MODULE_LIST Core Gui Widgets Svg Qml Network QmlModels QmlWorkerScript Quick QuickWidgets QuickTemplates2 QuickControls2)
find_package(Qt5 COMPONENTS ${QT_MODULE_LIST} LinguistTools)

set(QT_CMAKE_EXPORT_NAMESPACE Qt5)
set(${QT_CMAKE_EXPORT_NAMESPACE}::uic ${Qt5Widgets_UIC_EXECUTABLE})
set(${QT_CMAKE_EXPORT_NAMESPACE}::rcc ${Qt5Core_RCC_EXECUTABLE})
set(${QT_CMAKE_EXPORT_NAMESPACE}::lrelease ${Qt5_LRELEASE_EXECUTABLE})

include(targets)
include(qt_helpers)

add_definitions(-DCASCLIB_NO_AUTO_LINK_LIBRARY -DSTORMLIB_NO_AUTO_LINK)

include(CascLib)
include(StormLib)

include(ext/mernel/mernel_thirdparty.cmake)
include(ext/mernel/mernel.cmake)

# source
set(sourceRoot ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(pluginRootBin ${CMAKE_CURRENT_BINARY_DIR}/plugins)
set(presetsRootBin ${CMAKE_CURRENT_BINARY_DIR}/presets)

AddTarget(TYPE shared NAME ModgenCore
    SOURCE_DIR ${sourceRoot}/CoreLib
	INCLUDES   ${sourceRoot}/CoreLib
    EXPORT_INCLUDES
    LINK_LIBRARIES rapidjson casclib stormlib MernelPlatform
    )


file(GLOB_RECURSE resourceFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/modules
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/*.json)
foreach(filename ${resourceFiles})
	add_custom_command(OUTPUT ${pluginRootBin}/${filename}
		COMMAND ${CMAKE_COMMAND} ARGS -E copy ${CMAKE_CURRENT_SOURCE_DIR}/modules/${filename} ${pluginRootBin}/${filename}
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/modules/${filename})
	target_sources(ModgenCore PRIVATE ${pluginRootBin}/${filename})
endforeach()

file(GLOB_RECURSE presetFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/presets
    ${CMAKE_CURRENT_SOURCE_DIR}/presets/*.json ${CMAKE_CURRENT_SOURCE_DIR}/presets/*.txt)
foreach(filename ${presetFiles})
	add_custom_command(OUTPUT ${presetsRootBin}/${filename}
		COMMAND ${CMAKE_COMMAND} ARGS -E copy ${CMAKE_CURRENT_SOURCE_DIR}/presets/${filename} ${presetsRootBin}/${filename}
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/presets/${filename})
	target_sources(ModgenCore PRIVATE ${presetsRootBin}/${filename})
endforeach()

# ConsoleApp
AddTarget( TYPE app_console NAME modgen_console
    SOURCE_DIR ${sourceRoot}/ConsoleApp
    LINK_LIBRARIES ModgenCore MernelPlatform
    )

# TestPlugin
if (ENABLE_TEST_PLUGINS)
	set(pluginRootSrc ${sourceRoot}/Plugins/TestPlugin)
	file(GLOB src_Plugin
		${pluginRootSrc}/*.hpp
		${pluginRootSrc}/*.cpp
		)
	add_library(simpleTest SHARED ${src_Plugin})
	target_link_libraries(simpleTest PRIVATE modgenCore)
	generate_export_header(simpleTest BASE_NAME Plugin EXPORT_FILE_NAME PluginExport.hpp)
	
	set_target_properties(simpleTest PROPERTIES 
		RUNTIME_OUTPUT_DIRECTORY ${pluginRootBin}/simpleTest
		)
	file(GLOB_RECURSE resourceFiles RELATIVE ${pluginRootSrc}/dist
		${pluginRootSrc}/dist/*)
	foreach(filename ${resourceFiles})
		add_custom_command(OUTPUT ${pluginRootBin}/${filename}
			COMMAND ${CMAKE_COMMAND} ARGS -E copy ${pluginRootSrc}/dist/${filename} ${pluginRootBin}/${filename}
			DEPENDS ${pluginRootSrc}/dist/${filename})
		target_sources(simpleTest PRIVATE ${pluginRootBin}/${filename})
	endforeach()
endif()

if (NOT Qt5Core_DIR)
	return()
endif()

# QtApp

AddTarget( TYPE app_ui NAME modgen
    SOURCE_DIR ${sourceRoot}/QtApp
    INCLUDES   ${sourceRoot}/QtApp
    LINK_LIBRARIES ModgenCore MernelPlatform
    QT_MODULES Widgets Quick QuickWidgets QuickControls2
    )

GenerateQrcWithTranslations(Translations ${CMAKE_CURRENT_SOURCE_DIR}/translations qrcName)
AddQrcTarget(${CMAKE_CURRENT_SOURCE_DIR}/theme/breeze.qrc)
AddResourceCustomTarget("all_resources" Translations breeze)

install(TARGETS modgen ModgenCore RUNTIME DESTINATION bin)
install(DIRECTORY ${pluginRootBin} DESTINATION bin PATTERN *.ilk EXCLUDE PATTERN *.pdb EXCLUDE)
install(DIRECTORY ${presetsRootBin} DESTINATION bin PATTERN *.ilk EXCLUDE PATTERN *.pdb EXCLUDE)
foreach(th IN ITEMS dark light)
	install(FILES theme/${th}.conf DESTINATION bin/theme)
	configure_file(theme/${th}.conf ${CMAKE_CURRENT_BINARY_DIR}/theme/${th}.conf COPYONLY)
endforeach()

InstallQmlFiles(
	QtQuick.2
	QtQuick/Controls.2
	QtQuick/Window.2
	QtQuick/Templates.2
	)

#[[
########## Installation (Win32 only at the moment) ##########
#]]

if(WIN32)
# Qt install - plugins
set(QtPluginsList Qt5::QSvgPlugin)
if(APPLE)
    list(APPEND QtPluginsList Qt5::QCocoaIntegrationPlugin)
elseif(WIN32)
    list(APPEND QtPluginsList Qt5::QWindowsIntegrationPlugin Qt5::QWindowsVistaStylePlugin)
endif()
foreach(plugin ${QtPluginsList})
    get_target_property(imploc_RELEASE ${plugin} IMPORTED_LOCATION_RELEASE)
    get_target_property(imploc_DEBUG   ${plugin} IMPORTED_LOCATION_DEBUG)

    get_filename_component(pluginPath ${imploc_RELEASE} DIRECTORY)
    get_filename_component(pluginDir ${pluginPath} NAME)
    if (EXISTS "${imploc_DEBUG}")
        install(FILES ${imploc_DEBUG}   DESTINATION bin/${pluginDir} CONFIGURATIONS Debug )
    endif()
    install(FILES ${imploc_RELEASE} DESTINATION bin/${pluginDir} CONFIGURATIONS Release )
endforeach()

# Qt install - shared libs
foreach(qt5Module ${QT_MODULE_LIST})
    get_target_property(imploc_RELEASE Qt5::${qt5Module} IMPORTED_LOCATION_RELEASE)
    get_target_property(imploc_DEBUG   Qt5::${qt5Module} IMPORTED_LOCATION_DEBUG)
    if (EXISTS "${imploc_DEBUG}")
        install(FILES ${imploc_DEBUG}   DESTINATION bin CONFIGURATIONS Debug    )
    endif()
    install(FILES ${imploc_RELEASE} DESTINATION bin CONFIGURATIONS Release  )
endforeach()

# Compiler runtime
set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
if (NOT(CMAKE_BUILD_TYPE STREQUAL Release))
    set(CMAKE_INSTALL_DEBUG_LIBRARIES TRUE)
endif()
include (InstallRequiredSystemLibraries)

endif(WIN32)
