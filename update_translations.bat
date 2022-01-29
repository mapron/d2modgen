@echo on
set originalDir=%CD%
mkdir %~dp0build
cd %~dp0build
cmake.exe  -DLUPDATE_TOOL=lupdate.exe %* -P ../cmake/translationsRelease.cmake
cd %originalDir%
