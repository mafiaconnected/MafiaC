@echo off
set CMAKE_GENERATOR="Visual Studio 15 2017"
set CMAKE_GENERATOR_PLATFORM=Win32
set CMAKE_GENERATOR_TOOLSET=v141_xp
set CMAKE_EXTRA_ARGS=-DNO_ENHANCED_INSTRUCTIONS=ON
call Configure.bat
if not defined AUTOMATION pause