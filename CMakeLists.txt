# Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
# SPDX-License-Identifier: MIT
# See LICENSE file for details.

# Common declarations
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/install CACHE STRING "" FORCE)

cmake_minimum_required(VERSION 3.12...3.19)

project(modgen)

include(GenerateExportHeader)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/targets.cmake)


# Qt Setup
set(QT_MODULE_LIST Core Gui Widgets Svg)
find_package(Qt5 COMPONENTS ${QT_MODULE_LIST} LinguistTools)
add_definitions(-DCASCLIB_NO_AUTO_LINK_LIBRARY -DSTORMLIB_NO_AUTO_LINK)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CascLib.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/StormLib.cmake)

set(jsonRoot ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)

# source
set(sourceRoot ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Core
file(GLOB_RECURSE src_Core
	${sourceRoot}/CoreLib/*.hpp
	${sourceRoot}/CoreLib/*.cpp
	)
add_library(modgenCore SHARED ${src_Core})
target_link_libraries(modgenCore PRIVATE casclib stormlib)
target_include_directories(modgenCore PUBLIC ${sourceRoot}/CoreLib ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(modgenCore PRIVATE ${jsonRoot})

generate_export_header(modgenCore BASE_NAME Core EXPORT_FILE_NAME CoreExport.hpp )

# ConsoleApp
file(GLOB_RECURSE src_ConsoleApp
	${sourceRoot}/ConsoleApp/*.hpp
	${sourceRoot}/ConsoleApp/*.cpp
	)
add_executable(modgen_console ${src_ConsoleApp})
target_link_libraries(modgen_console PRIVATE modgenCore)
target_include_directories(modgen_console PRIVATE ${sourceRoot}/ConsoleApp)

if (NOT Qt5Core_DIR)
	return()
endif()

# QtApp
file(GLOB_RECURSE src_QtApp
	${sourceRoot}/QtApp/*.hpp
	${sourceRoot}/QtApp/*.cpp
	)
if (WIN32 AND NOT (CMAKE_BUILD_TYPE STREQUAL "Debug"))
	set(w32 "WIN32")
endif()
GenerateQrcWithTranslations(Translations ${CMAKE_CURRENT_SOURCE_DIR}/translations qrcName)
add_executable(modgen ${w32} ${src_QtApp} theme/breeze.qrc ${qrcName})
set_target_properties(modgen PROPERTIES AUTOMOC ON AUTOUIC OFF AUTORCC ON)
target_include_directories(modgen PRIVATE ${sourceRoot}/QtApp)
target_link_libraries(modgen PRIVATE Qt5::Widgets modgenCore)

install(TARGETS modgen modgenCore RUNTIME DESTINATION bin)


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
