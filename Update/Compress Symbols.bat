mkdir Release
cd ..\Lib\x86\v141_xp_static\Release
del Symbols.7z
cls
"..\..\..\..\Tools\7z.exe" a Symbols.7z -mx9 *.pdb Hacks\*.pdb Engine\*.pdb
move Symbols.7z ..\..\..\..\Update\Release\
pause