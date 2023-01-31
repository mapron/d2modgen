# Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
# SPDX-License-Identifier: MIT
# See LICENSE file for details.

function(InstallQmlFiles)
	set(modules ${ARGN})
	set(qmlRoot ${_qt5Core_install_prefix})
	foreach(module ${modules})
		set(exc)
		if(CMAKE_BUILD_TYPE STREQUAL "Release")
			set(exc PATTERN *plugind.dll EXCLUDE)
		elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
			set(exc PATTERN  *plugin.dll EXCLUDE)
		endif()
		get_filename_component(subdir ${module} DIRECTORY)
		if (subdir)
			set(subdir /${subdir})
		endif()
		install(DIRECTORY ${qmlRoot}/qml/${module} DESTINATION bin/qml${subdir} PATTERN *.ilk EXCLUDE PATTERN *.pdb EXCLUDE ${exc})
	endforeach()
endfunction()
