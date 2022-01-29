# Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
# SPDX-License-Identifier: MIT
# See LICENSE file for details.

cmake_minimum_required(VERSION 3.12.0)

set(CMAKE_CURRENT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../")

set(CMAKE_MODULE_PATH	${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if (NOT LUPDATE_TOOL)
    message(SEND_ERROR "Please define LUPDATE_TOOL cmake variable.")
    return()
endif()

function(LupdateAppendSources sourcesListName directory)
    file( GLOB_RECURSE __sources
        ${directory}/*.cpp
        ${directory}/*.c
        ${directory}/*.h
        ${directory}/*.hpp
        ${directory}/*.ui
        ${directory}/*.qml
        ${directory}/*.js
        )
    set(__sources ${${sourcesListName}} ${__sources})
    list(REMOVE_DUPLICATES __sources)
    set(${sourcesListName} ${__sources} PARENT_SCOPE)
endfunction()

function(LupdateFiles tsFiles qmFiles sourceFiles)
    file(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/cpptmp.lst)
    file(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/tstmp.lst)
    foreach (ts ${tsFiles})
        file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/tstmp.lst "${ts}\n")
    endforeach()
    foreach (cpp ${sourceFiles})
        file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/cpptmp.lst "${cpp}\n")
    endforeach()

    execute_process(COMMAND ${LUPDATE_TOOL} @${CMAKE_CURRENT_BINARY_DIR}/cpptmp.lst -no-obsolete -ts @${CMAKE_CURRENT_BINARY_DIR}/tstmp.lst)
endfunction()

function(UpdateLocales)
    set(__options)
    set(__one_val TS_DIR SOURCE_BASE)
    set(__multi_val LANGUAGES MODULES)
    cmake_parse_arguments(ARGS  "${__options}" "${__one_val}" "${__multi_val}" ${ARGN})

    set(__options)
    set(__one_val)
    set(__multi_val DIRS SOURCES)

    foreach(moduleArgs ${ARGS_MODULES})
        string(REPLACE "[;" "" moduleArgs "${moduleArgs}")
        string(REPLACE ";]" "" moduleArgs "${moduleArgs}")
        list(GET moduleArgs 0 module)
        list(REMOVE_AT moduleArgs 0)

        cmake_parse_arguments(MODULE "${__options}" "${__one_val}" "${__multi_val}" ${moduleArgs})
        set(tsFiles)
        set(qmFiles)
        foreach(language ${ARGS_LANGUAGES})
            set(src ${ARGS_TS_DIR}/${module}_${language}.ts)
            list(APPEND tsFiles ${src})

            set(qm ${ARGS_QM_DIR}/${module}_${language}.qm)
            list(APPEND qmFiles ${qm})
        endforeach()

        set(sources ${MODULE_SOURCES})
        foreach (sourceDir ${MODULE_DIRS})
            set(sourceDir ${ARGS_SOURCE_BASE}/${sourceDir})
            message(STATUS "Updating module '${module}' with sourceDir='${sourceDir}'")
            LupdateAppendSources(sources "${sourceDir}")
        endforeach()
        if (NOT sources)
            message(FATAL_ERROR "No sources specified for ${module} translation module.")
        endif()
        LupdateFiles("${tsFiles}" "${qmFiles}" "${sources}")

    endforeach()
endfunction()

UpdateLocales(
    LANGUAGES
        en_US ru_RU
    MODULES
        [ modgen DIRS src ]
    TS_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/translations
    SOURCE_BASE
        ${CMAKE_CURRENT_LIST_DIR}/..
    )

file(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/cpptmp.lst)
file(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/tstmp.lst)
