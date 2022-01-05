# Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
# SPDX-License-Identifier: MIT
# See LICENSE file for details.

# Common declarations
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/install CACHE STRING "" FORCE)

cmake_minimum_required(VERSION 3.12...3.19)

project(modgen)


# Qt Setup
# QtNetwork is only for QtMultimedia
set(QT_MODULE_LIST Core Gui Widgets)
find_package(Qt5 COMPONENTS ${QT_MODULE_LIST} REQUIRED)
add_definitions(-DCASCLIB_NO_AUTO_LINK_LIBRARY)

# casclib
set(cascRoot ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/casclib/src)
set(cascInc ${cascRoot})
set(cascSrc 
	CascDecompress.cpp
	CascDecrypt.cpp
	CascDumpData.cpp
	CascFiles.cpp
	CascFindFile.cpp
	CascIndexFiles.cpp
	CascOpenFile.cpp
	CascOpenStorage.cpp
	CascReadFile.cpp
	CascRootFile_Diablo3.cpp
	CascRootFile_Install.cpp
	CascRootFile_MNDX.cpp
	CascRootFile_OW.cpp
	CascRootFile_Text.cpp
	CascRootFile_TVFS.cpp
	CascRootFile_WoW.cpp
	common/Common.cpp
	common/Directory.cpp
	common/Csv.cpp
	common/FileStream.cpp
	common/FileTree.cpp
	common/ListFile.cpp
	common/RootHandler.cpp
	common/Mime.cpp
	common/Sockets.cpp
	# DllMain.c
	jenkins/lookup3.c
	md5/md5.cpp
	zlib/adler32.c
	zlib/crc32.c
	zlib/inffast.c
	zlib/inflate.c
	zlib/inftrees.c
	zlib/trees.c
	zlib/zutil.c
	
	CascCommon.h
	CascLib.h
	CascPort.h
	CascStructs.h
	common/Common.h
	common/Directory.h
	common/Csv.h
	common/Array.h
	common/FileTree.h
	common/ListFile.h
	common/Map.h
	common/RootHandler.h
	common/Mime.h
	common/Sockets.h
	#FileStream.h
	md5/md5.h
	zlib/deflate.h
	)
set(cascSrcFull)
foreach(file ${cascSrc})
	list(APPEND cascSrcFull ${cascInc}/${file})
endforeach()

add_library(casclib STATIC ${cascSrcFull})
target_include_directories(casclib PRIVATE ${cascInc})

# source
set(sourceRoot ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB src ${sourceRoot}/*.hpp ${sourceRoot}/*.cpp)

add_executable(modgen ${src})
set_target_properties(modgen PROPERTIES AUTOMOC ON AUTOUIC OFF AUTORCC OFF)
target_include_directories(modgen PRIVATE ${sourceRoot} ${cascInc})
target_link_libraries(modgen PRIVATE Qt5::Widgets casclib)
install(TARGETS modgen RUNTIME DESTINATION bin)


#[[
########## Installation (Win32 only at the moment) ##########
#]]

if(WIN32)
# Qt install - plugins
set(QtPluginsList)
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
