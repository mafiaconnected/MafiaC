copy GTAC.tar "%dropbox%\MafiaC (Staff)\Builds\Mex\Client\"
copy GTAC.tar "%dropbox%\MafiaC (Testers)\Builds\Mex\Client\"

cd Lib\x86\v141_xp_static

REM Staff Folder
del "%dropbox%\MafiaC (Staff)\Builds\Vortrex\Mex\*.exe"
del "%dropbox%\MafiaC (Staff)\Builds\Vortrex\Mex\*.dll"
copy Debug\*.exe "%dropbox%\MafiaC (Staff)\Builds\Mex\Client\"
copy Debug\*.dll "%dropbox%\MafiaC (Staff)\Builds\Mex\Client\"
copy RelWithDebInfo\*.exe "%dropbox%\MafiaC (Staff)\Builds\Mex\Client\"
copy RelWithDebInfo\*.dll "%dropbox%\MafiaC (Staff)\Builds\Mex\Client\"

REM Tester Folder
del "%dropbox%\MafiaC (Testers)\Builds\Vortrex\Mex\*.exe"
del "%dropbox%\MafiaC (Testers)\Builds\Vortrex\Mex\*.dll"
copy RelWithDebInfo\*.exe "%dropbox%\MafiaC (Testers)\Builds\Mex\Client\"
copy RelWithDebInfo\*.dll "%dropbox%\MafiaC (Testers)\Builds\Mex\Client\"
pause
