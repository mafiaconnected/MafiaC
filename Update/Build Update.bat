mkdir Release
cd ..\Lib\x86\v141_xp\Release
del preupdate.tar
del postupdate.tar
cls
"..\..\..\..\Tools\7z.exe" a preupdate.tar Updater.exe ..\..\..\..\Update\removed.txt
"..\..\..\..\Tools\7z.exe" a postupdate.tar -xr!DebugTest.dll -xr!d3d8to9.dll Launcher.exe *.dll Hacks\*.dll Engine\*.exe Engine\*.dll ..\..\..\..\Setup\resources GTAC.tar cacert.pem
"..\..\..\..\Tools\7z.exe" a update.tar preupdate.tar postupdate.tar
"..\..\..\..\Tools\7z.exe" a update.tar.gz update.tar
del preupdate.tar
del postupdate.tar
del update.tar
move update.tar.gz ..\..\..\..\Update\Release\
pause