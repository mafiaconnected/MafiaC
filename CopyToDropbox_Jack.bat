copy GTAC.tar "%dropbox%\MafiaC\Client\"
copy GTAC.tar "%dropbox%\MafiaC (Staff)\Builds\Jack\Client\"
copy GTAC.tar "%dropbox%\MafiaC (Testers)\Builds\Jack\Client\"

cd Lib\x86\v142

del "%dropbox%\MafiaC\Client\*.exe"
del "%dropbox%\MafiaC\Client\*.dll"

REM Staff Folder
del "%dropbox%\MafiaC (Staff)\Builds\Jack\Client\*.exe"
del "%dropbox%\MafiaC (Staff)\Builds\Jack\Client\*.dll"
copy Debug\*.exe "%dropbox%\MafiaC (Staff)\Builds\Jack\Client\"
copy Debug\*.dll "%dropbox%\MafiaC (Staff)\Builds\Jack\Client\"
copy RelWithDebInfo\*.exe "%dropbox%\MafiaC (Staff)\Builds\Jack\Client\"
copy RelWithDebInfo\*.dll "%dropbox%\MafiaC (Staff)\Builds\Jack\Client\"

REM Tester Folder
del "%dropbox%\MafiaC (Testers)\Builds\Jack\Client\*.exe"
del "%dropbox%\MafiaC (Testers)\Builds\Jack\Client\*.dll"
copy RelWithDebInfo\*.exe "%dropbox%\MafiaC (Testers)\Builds\Jack\Client\"
copy RelWithDebInfo\*.dll "%dropbox%\MafiaC (Testers)\Builds\Jack\Client\"

pause
