@echo off
setlocal
set CMAKE_GENERATOR="Visual Studio 15 2017"
set CMAKE_GENERATOR_PLATFORM=x64
set CMAKE_GENERATOR_TOOLSET=v141
rem call Compile.bat
set CMAKE_EXTRA_PATH=_static
set CMAKE_EXTRA_ARGS=-DFORCE_STATIC_VCRT=ON
call Compile.bat
endlocal
if not defined AUTOMATION pause