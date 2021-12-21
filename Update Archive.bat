del GTAC.tar
set PATH=%PATH%;%CD%\Tools;%GALACTIC_HOME%\Lib\x86\v141_xp\Release
set DIR=%CD%
cls
hbfelements Projects\Launcher\Launcher.xml Launcher.xml
7z a -ttar "%DIR%\MafiaC.tar" -xr!*.bak "%CD%\Launcher.xml" "%CD%\Projects\Launcher\Watermark.hbf" "%CD%\Projects\Launcher\Logo.png" "%CD%\Data\master.css" "%CD%\Data\GUI" "%CD%\Data\Languages" "%CD%\Data\Fonts" "%CD%\Data\Shaders"
del Launcher.xml
pause