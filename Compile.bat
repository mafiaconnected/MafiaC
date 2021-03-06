@if [%CMAKE_GENERATOR%] == [] echo Insufficient parameters! && pause && exit

mkdir CMake.tmp\%CMAKE_GENERATOR%\%CMAKE_GENERATOR_PLATFORM%\%CMAKE_GENERATOR_TOOLSET%%CMAKE_EXTRA_PATH%
cd CMake.tmp\%CMAKE_GENERATOR%\%CMAKE_GENERATOR_PLATFORM%\%CMAKE_GENERATOR_TOOLSET%%CMAKE_EXTRA_PATH%

cmake ../../../.. -G %CMAKE_GENERATOR% -A %CMAKE_GENERATOR_PLATFORM% -T %CMAKE_GENERATOR_TOOLSET% -DCMAKE_INSTALL_PREFIX="../../../.." %CMAKE_EXTRA_ARGS%
if %ERRORLEVEL% neq 0 pause && goto eof
cmake --build . --config Debug --target install
if %ERRORLEVEL% neq 0 pause && goto eof
cmake --build . --config Release --target install
if %ERRORLEVEL% neq 0 pause && goto eof
cmake --build . --config RelWithDebInfo --target install
if %ERRORLEVEL% neq 0 pause && goto eof

:eof
cd ../../../..
