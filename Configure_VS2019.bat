@echo off
set CMAKE_GENERATOR="Visual Studio 16 2019"
set CMAKE_GENERATOR_PLATFORM=Win32
set CMAKE_GENERATOR_TOOLSET=v142
set CMAKE_EXTRA_ARGS=
call Configure.bat
if not defined AUTOMATION pause