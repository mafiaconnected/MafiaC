@echo off
setlocal
set CMAKE_GENERATOR="Visual Studio 17 2022"
set CMAKE_GENERATOR_PLATFORM=Win32
set CMAKE_GENERATOR_TOOLSET=v143
set CMAKE_EXTRA_PATH=_static
set CMAKE_EXTRA_ARGS=-DFORCE_STATIC_VCRT=ON
call Configure.bat
endlocal
if not defined AUTOMATION pause