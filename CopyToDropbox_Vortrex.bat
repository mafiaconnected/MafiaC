copy GTAC.tar "%dropbox%\MafiaC (Staff)\Builds\Vortrex\Client\"
copy GTAC.tar "%dropbox%\MafiaC (Testers)\Builds\Vortrex\Client\"

cd Lib\x86\v141_xp

REM Staff Folder
del "%dropbox%\MafiaC (Staff)\Builds\Vortrex\Client\*.exe"
del "%dropbox%\MafiaC (Staff)\Builds\Vortrex\Client\*.dll"
copy Debug\*.exe "%dropbox%\MafiaC (Staff)\Builds\Vortrex\Client\"
copy Debug\*.dll "%dropbox%\MafiaC (Staff)\Builds\Vortrex\Client\"
copy RelWithDebInfo\*.exe "%dropbox%\MafiaC (Staff)\Builds\Vortrex\Client\"
copy RelWithDebInfo\*.dll "%dropbox%\MafiaC (Staff)\Builds\Vortrex\Client\"

REM Tester Folder
del "%dropbox%\MafiaC (Testers)\Builds\Vortrex\Client\*.exe"
del "%dropbox%\MafiaC (Testers)\Builds\Vortrex\Client\*.dll"
copy RelWithDebInfo\*.exe "%dropbox%\MafiaC (Testers)\Builds\Vortrex\Client\"
copy RelWithDebInfo\*.dll "%dropbox%\MafiaC (Testers)\Builds\Vortrex\Client\"
pause
