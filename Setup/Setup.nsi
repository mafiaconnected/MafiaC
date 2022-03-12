;NSIS Modern User Interface
;Basic Example Script
;Written by Joost Verburg

!define MULTIUSER_EXECUTIONLEVEL Highest
!define MULTIUSER_MUI
!define MULTIUSER_INSTALLMODE_COMMANDLINE

;--------------------------------
;Include Modern UI

Unicode true

ManifestDPIAware true
ManifestLongPathAware true
ManifestSupportedOS all

;!include MultiUser.nsh
!include "MUI2.nsh"
!include "FileFunc.nsh"

;--------------------------------
;General

;!define PRODUCT_BETA

!define UNINSTALL_NAME "MafiaC"

!define PRODUCT_NAME "Mafia Connected"
!define PRODUCT_NAME_SHORT "MafiaC"
!define PRODUCT_VERSION "1.0.0"
!define PRODUCT_VERSION_BUILD "0"
!define PRODUCT_VERSION_FULL "${PRODUCT_VERSION}.${PRODUCT_VERSION_BUILD}"
!define PRODUCT_ICON "..\Projects\Launcher\MafiaC.ico"
;!define DELTA_UPDATE
;!define INSTALL_ALL_USERS

;Name and file
Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
!ifdef DELTA_UPDATE
OutFile "Installer\${PRODUCT_NAME_SHORT} ${PRODUCT_VERSION}_delta.exe"
AutoCloseWindow true
!else
OutFile "Installer\${PRODUCT_NAME_SHORT}-${PRODUCT_VERSION}.exe"
!endif

;Default installation folder
!ifdef INSTALL_ALL_USERS
InstallDir "$PROGRAMFILES\Jack's Mini Network\${PRODUCT_NAME}"
!else
InstallDir "$LOCALAPPDATA\Jack's Mini Network\${PRODUCT_NAME}"
!endif

!ifdef INSTALL_ALL_USERS
!define INSTALL_REGISTRY_ROOT "HKLM"
!else
!define INSTALL_REGISTRY_ROOT "HKCU"
!endif

;Get installation folder from registry if available
InstallDirRegKey ${INSTALL_REGISTRY_ROOT} "Software\Jack's Mini Network\${PRODUCT_NAME}" "Installation Directory"

;Request application privileges for Windows Vista
!ifdef INSTALL_ALL_USERS
RequestExecutionLevel admin
!else
RequestExecutionLevel highest
!endif

SetCompressor /SOLID lzma

Icon "${PRODUCT_ICON}"
UninstallIcon "${PRODUCT_ICON}"

!insertmacro MUI_DEFAULT MUI_ICON "${PRODUCT_ICON}"
!insertmacro MUI_DEFAULT MUI_UNICON "${PRODUCT_ICON}"

XPStyle on
ManifestDPIAware true
ShowInstDetails show

;--------------------------------
;Interface Settings

!define MUI_ABORTWARNING

!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "nsis.bmp" ; optional
!define MUI_WELCOMEFINISHPAGE_BITMAP "win.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "win.bmp"

!define MUI_FINISHPAGE_RUN
!define MUI_FINISHPAGE_RUN_TEXT "Start ${PRODUCT_NAME}"
!define MUI_FINISHPAGE_RUN_FUNCTION "LaunchLink"

;--------------------------------
;Language Selection Dialog Settings

;Remember the installer language
!define MUI_LANGDLL_REGISTRY_ROOT ${INSTALL_REGISTRY_ROOT}
!define MUI_LANGDLL_REGISTRY_KEY "Software\Jack's Mini Network\${PRODUCT_NAME}"
!define MUI_LANGDLL_REGISTRY_VALUENAME "Language"

;--------------------------------
;Pages

!ifndef DELTA_UPDATE
  !define MUI_PAGE_CUSTOMFUNCTION_PRE PrePage
  !insertmacro MUI_PAGE_WELCOME
  !define MUI_PAGE_CUSTOMFUNCTION_PRE PrePage
  !insertmacro MUI_PAGE_LICENSE "License.rtf"
  ;!insertmacro MULTIUSER_PAGE_INSTALLMODE
  ;!define MUI_PAGE_CUSTOMFUNCTION_PRE PrePage
  ;!insertmacro MUI_PAGE_COMPONENTS
  !define MUI_PAGE_CUSTOMFUNCTION_PRE PrePage
  !insertmacro MUI_PAGE_DIRECTORY
!endif
  !insertmacro MUI_PAGE_INSTFILES
!ifndef DELTA_UPDATE
  !define MUI_PAGE_CUSTOMFUNCTION_PRE PrePage
  !insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH
!endif

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English" ;first language is the default language
  !insertmacro MUI_LANGUAGE "French"
  !insertmacro MUI_LANGUAGE "German"
  !insertmacro MUI_LANGUAGE "Spanish"
  !insertmacro MUI_LANGUAGE "SpanishInternational"
  !insertmacro MUI_LANGUAGE "SimpChinese"
  !insertmacro MUI_LANGUAGE "TradChinese"
  !insertmacro MUI_LANGUAGE "Japanese"
  !insertmacro MUI_LANGUAGE "Korean"
  !insertmacro MUI_LANGUAGE "Italian"
  !insertmacro MUI_LANGUAGE "Dutch"
  !insertmacro MUI_LANGUAGE "Danish"
  !insertmacro MUI_LANGUAGE "Swedish"
  !insertmacro MUI_LANGUAGE "Norwegian"
  !insertmacro MUI_LANGUAGE "NorwegianNynorsk"
  !insertmacro MUI_LANGUAGE "Finnish"
  !insertmacro MUI_LANGUAGE "Greek"
  !insertmacro MUI_LANGUAGE "Russian"
  !insertmacro MUI_LANGUAGE "Portuguese"
  !insertmacro MUI_LANGUAGE "PortugueseBR"
  !insertmacro MUI_LANGUAGE "Polish"
  !insertmacro MUI_LANGUAGE "Ukrainian"
  !insertmacro MUI_LANGUAGE "Czech"
  !insertmacro MUI_LANGUAGE "Slovak"
  !insertmacro MUI_LANGUAGE "Croatian"
  !insertmacro MUI_LANGUAGE "Bulgarian"
  !insertmacro MUI_LANGUAGE "Hungarian"
  !insertmacro MUI_LANGUAGE "Thai"
  !insertmacro MUI_LANGUAGE "Romanian"
  !insertmacro MUI_LANGUAGE "Latvian"
  !insertmacro MUI_LANGUAGE "Macedonian"
  !insertmacro MUI_LANGUAGE "Estonian"
  !insertmacro MUI_LANGUAGE "Turkish"
  !insertmacro MUI_LANGUAGE "Lithuanian"
  !insertmacro MUI_LANGUAGE "Slovenian"
  !insertmacro MUI_LANGUAGE "Serbian"
  !insertmacro MUI_LANGUAGE "SerbianLatin"
  !insertmacro MUI_LANGUAGE "Arabic"
  !insertmacro MUI_LANGUAGE "Farsi"
  !insertmacro MUI_LANGUAGE "Hebrew"
  !insertmacro MUI_LANGUAGE "Indonesian"
  !insertmacro MUI_LANGUAGE "Mongolian"
  !insertmacro MUI_LANGUAGE "Luxembourgish"
  !insertmacro MUI_LANGUAGE "Albanian"
  !insertmacro MUI_LANGUAGE "Breton"
  !insertmacro MUI_LANGUAGE "Belarusian"
  !insertmacro MUI_LANGUAGE "Icelandic"
  !insertmacro MUI_LANGUAGE "Malay"
  !insertmacro MUI_LANGUAGE "Bosnian"
  !insertmacro MUI_LANGUAGE "Kurdish"
  !insertmacro MUI_LANGUAGE "Irish"
  !insertmacro MUI_LANGUAGE "Uzbek"
  !insertmacro MUI_LANGUAGE "Galician"
  !insertmacro MUI_LANGUAGE "Afrikaans"
  !insertmacro MUI_LANGUAGE "Catalan"
  !insertmacro MUI_LANGUAGE "Esperanto"

VIProductVersion "${PRODUCT_VERSION_FULL}"
VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductName" "${PRODUCT_NAME}"
VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductVersion" "${PRODUCT_VERSION_FULL}"
;VIAddVersionKey /LANG=${LANG_ENGLISH} "Comments" "A test comment"
VIAddVersionKey /LANG=${LANG_ENGLISH} "CompanyName" "Jack's Mini Network"
;VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalTrademarks" "Test Application is a trademark of Fake company"
VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalCopyright" "Copyright � 2021 Jack's Mini Network"
VIAddVersionKey /LANG=${LANG_ENGLISH} "FileDescription" "${PRODUCT_NAME}"
VIAddVersionKey /LANG=${LANG_ENGLISH} "FileVersion" "${PRODUCT_VERSION_FULL}"

;--------------------------------
;Reserve Files

  ;If you are using solid compression, files that are required before
  ;the actual installation should be stored first in the data block,
  ;because this will make your installer start faster.

  !insertmacro MUI_RESERVEFILE_LANGDLL

;--------------------------------
;Installer Sections

; https://nsis.sourceforge.io/Auto-uninstall_old_before_installing_new

!include LogicLib.nsh

!macro UninstallExisting exitcode uninstcommand
Push `${uninstcommand}`
Call UninstallExisting
Pop ${exitcode}
!macroend
Function UninstallExisting
Exch $1 ; uninstcommand
Push $2 ; Uninstaller
Push $3 ; Len
StrCpy $3 ""
StrCpy $2 $1 1
StrCmp $2 '"' qloop sloop
sloop:
	StrCpy $2 $1 1 $3
	IntOp $3 $3 + 1
	StrCmp $2 ' ' 0 sloop
	IntOp $3 $3 - 1
	Goto run
qloop:
	StrCmp $3 "" 0 +2
	StrCpy $1 $1 "" 1 ; Remove initial quote
	IntOp $3 $3 + 1
	StrCpy $2 $1 1 $3
	StrCmp $2 '"' 0 qloop
run:
	StrCpy $2 $1 $3 ; Path to uninstaller
	StrCpy $1 161 ; ERROR_BAD_PATHNAME
	GetFullPathName $3 "$2\.." ; $InstDir
	IfFileExists "$2" 0 +4
	ExecWait '"$2" /S _?=$3' $1 ; This assumes the existing uninstaller is a NSIS uninstaller, other uninstallers don't support /S nor _?=
	IntCmp $1 0 "" +2 +2 ; Don't delete the installer if it was aborted
	Delete "$2" ; Delete the uninstaller
	RMDir "$3" ; Try to delete $InstDir
	RMDir "$3\.." ; (Optional) Try to delete the parent of $InstDir
Pop $3
Pop $2
Exch $1 ; exitcode
FunctionEnd

!macro CheckInstalled REGISTRY_ROOT
	ReadRegStr $0 ${REGISTRY_ROOT} "Software\Microsoft\Windows\CurrentVersion\Uninstall\${UNINSTALL_NAME}" "UninstallString"
	${If} $0 != ""
	${AndIf} ${Cmd} `MessageBox MB_YESNO|MB_ICONQUESTION "Uninstall previous version?" /SD IDYES IDYES`
		!insertmacro UninstallExisting $0 $0
		${If} $0 <> 0
			MessageBox MB_YESNO|MB_ICONSTOP "Failed to uninstall, continue anyway?" /SD IDYES IDYES +2
				Abort
		${EndIf}
	${EndIf}
!macroend

; Check installed globally
Section ""
	SectionIn RO

	!insertmacro CheckInstalled "HKLM"
SectionEnd

; Check installed locally
Section ""
	SectionIn RO

	!insertmacro CheckInstalled "HKCU"
SectionEnd

Section "" SecLauncher
	SectionIn RO

!ifdef INSTALL_ALL_USERS
	SetShellVarContext all
!else
	SetShellVarContext current
!endif

	SetOutPath "$INSTDIR"

	Delete "$INSTDIR\Hacks\*.dll"

	; delete old crashes
	;Delete "$INSTDIR\Crashes\*.dmp"

	File "Redistributable\vc_redist.x86.exe"
	ExecWait '"$INSTDIR\vc_redist.x86.exe" /install /quiet /norestart'
	Delete "$INSTDIR\vc_redist.x86.exe"

	;File "Redistributable\dxwebsetup.exe"
	;ExecWait '"$INSTDIR\dxwebsetup.exe" /Q'
	;Delete "$INSTDIR\dxwebsetup.exe"

	RMDir "$INSTDIR\Redistributable"

	File "..\Lib\x86\v141_xp\Release\Launcher.exe"
	File "..\Lib\x86\v141_xp\Release\Updater.exe"
	File "..\Lib\x86\v141_xp\Release\*.dll"
	SetOutPath "$INSTDIR"
	File "cacert.pem"
	File "..\Lib\x86\v141_xp\Release\MafiaC.tar"
	File "resources.xml"

	;SetOutPath "$INSTDIR\resources"
	;File "resources\*.tar"

	SetOutPath "$INSTDIR"

	;Store installation folder
	WriteRegStr ${INSTALL_REGISTRY_ROOT} "Software\Jack's Mini Network\${PRODUCT_NAME}" "Installation Directory" $INSTDIR

	;Create uninstaller
	WriteUninstaller "$INSTDIR\Uninstall.exe"

	; Add shortcuts
	CreateDirectory "$SMPROGRAMS\Jack's Mini Network\${PRODUCT_NAME}"
	CreateShortCut "$SMPROGRAMS\Jack's Mini Network\${PRODUCT_NAME}\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
	CreateShortCut "$SMPROGRAMS\Jack's Mini Network\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk" "$INSTDIR\Launcher.exe" "" "$INSTDIR\Launcher.exe" 0

	; Write the uninstall keys for Windows
	WriteRegStr ${INSTALL_REGISTRY_ROOT} "Software\Microsoft\Windows\CurrentVersion\Uninstall\${UNINSTALL_NAME}" "DisplayName" "${PRODUCT_NAME}"
	WriteRegStr ${INSTALL_REGISTRY_ROOT} "Software\Microsoft\Windows\CurrentVersion\Uninstall\${UNINSTALL_NAME}" "DisplayIcon" '"$INSTDIR\Uninstall.exe"'
	WriteRegStr ${INSTALL_REGISTRY_ROOT} "Software\Microsoft\Windows\CurrentVersion\Uninstall\${UNINSTALL_NAME}" "UninstallString" '"$INSTDIR\Uninstall.exe"'
	WriteRegStr ${INSTALL_REGISTRY_ROOT} "Software\Microsoft\Windows\CurrentVersion\Uninstall\${UNINSTALL_NAME}" "QuietUninstallString" '"$INSTDIR\Uninstall.exe" /S'
	WriteRegStr ${INSTALL_REGISTRY_ROOT} "Software\Microsoft\Windows\CurrentVersion\Uninstall\${UNINSTALL_NAME}" "Publisher" "Jack's Mini Network"
	WriteRegStr ${INSTALL_REGISTRY_ROOT} "Software\Microsoft\Windows\CurrentVersion\Uninstall\${UNINSTALL_NAME}" "HelpLink" ""
	WriteRegStr ${INSTALL_REGISTRY_ROOT} "Software\Microsoft\Windows\CurrentVersion\Uninstall\${UNINSTALL_NAME}" "URLUpdateInfo" ""
	WriteRegStr ${INSTALL_REGISTRY_ROOT} "Software\Microsoft\Windows\CurrentVersion\Uninstall\${UNINSTALL_NAME}" "DisplayVersion" "${PRODUCT_VERSION}"
	WriteRegDWORD ${INSTALL_REGISTRY_ROOT} "Software\Microsoft\Windows\CurrentVersion\Uninstall\${UNINSTALL_NAME}" "VersionMajor" 0
	WriteRegDWORD ${INSTALL_REGISTRY_ROOT} "Software\Microsoft\Windows\CurrentVersion\Uninstall\${UNINSTALL_NAME}" "VersionMinor" 1
	WriteRegDWORD ${INSTALL_REGISTRY_ROOT} "Software\Microsoft\Windows\CurrentVersion\Uninstall\${UNINSTALL_NAME}" "NoModify" 1
	WriteRegDWORD ${INSTALL_REGISTRY_ROOT} "Software\Microsoft\Windows\CurrentVersion\Uninstall\${UNINSTALL_NAME}" "NoRepair" 1

	; Protocol
	;WriteRegStr HKCR "mafiac" "" "URL:Mafia Connected Protocol"
	;WriteRegStr HKCR "mafiac" "URL Protocol" ""
	;WriteRegStr HKCR "mafiac\DefaultIcon" "" "$INSTDIR\Launcher.exe"
	;WriteRegStr HKCR "mafiac\shell\open\command" "" '"$INSTDIR\Launcher.exe"%1'
	DeleteRegKey HKLM "Software\Classes\mafiac"
SectionEnd

;--------------------------------
;Installer Functions

Function .onInit

	;!insertmacro MULTIUSER_INIT

	!insertmacro MUI_LANGDLL_DISPLAY

FunctionEnd

;--------------------------------
;Descriptions

;Language strings
LangString DESC_SecLauncher ${LANG_ENGLISH} "The client."
LangString DESC_SecServer ${LANG_ENGLISH} "The multiplayer server."

;Assign language strings to sections
;!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
;  !insertmacro MUI_DESCRIPTION_TEXT ${SecLauncher} $(DESC_SecLauncher)
;  ;!insertmacro MUI_DESCRIPTION_TEXT ${SecServer} $(DESC_SecServer)
;!insertmacro MUI_FUNCTION_DESCRIPTION_END

!ifndef DELTA_UPDATE

;--------------------------------
;Uninstaller Section

Section "Uninstall"

	${GetParameters} $R0
	${GetOptions} $R0 "-upgrade=" $R1

!ifdef INSTALL_ALL_USERS
	SetShellVarContext all
!else
	SetShellVarContext current
!endif

	SetOutPath "$INSTDIR"

	; Remove shortcuts, if any
	Delete "$SMPROGRAMS\Jack's Mini Network\${PRODUCT_NAME}\*.lnk"
	RMDir "$SMPROGRAMS\Jack's Mini Network\${PRODUCT_NAME}"
	RMDir "$SMPROGRAMS\Jack's Mini Network"

	Delete "$DESKTOP\${PRODUCT_NAME}.lnk"

	StrCmp $R1 "TRUE" no_delete_caches delete_caches
delete_caches:
	RMDir /r "$LOCALAPPDATA\Jack's Mini Network\${PRODUCT_NAME}\Caches"
	RMDir /r "$LOCALAPPDATA\Jack's Mini Network\${PRODUCT_NAME}\Crashes"
	;RMDir /r "$INSTDIR\resources"
no_delete_caches:

	RMDir "$INSTDIR\Redistributable"

	Delete "$INSTDIR\Hacks\*.dll"
	RMDir "$INSTDIR\Hacks"
	Delete "$INSTDIR\Engine\*.exe"
	Delete "$INSTDIR\Engine\*.dll"
	RMDir "$INSTDIR\Engine"
	Delete "$INSTDIR\resources\*.tar"
	RMDir "$INSTDIR\resources"
	;Delete "$INSTDIR\Crashes\*.dmp"
	;RMDir "$INSTDIR\Crashes"

	Delete "$INSTDIR\cacert.pem"
	Delete "$INSTDIR\MafiaC.tar"
	Delete "$INSTDIR\Launcher.exe"
	Delete "$INSTDIR\Updater.exe"
	Delete "$INSTDIR\*.dll"
	Delete "$INSTDIR\resources.xml"
	Delete "$INSTDIR\dbghelp.dll"

	Delete "$INSTDIR\Uninstall.exe"

	RMDir "$INSTDIR"

	; Clean registry entries used
	;DeleteRegKey HKCR "mafiac"
	DeleteRegKey HKLM "Software\Classes\mafiac"
	DeleteRegKey HKCU "Software\Classes\mafiac"
	DeleteRegKey ${INSTALL_REGISTRY_ROOT} "Software\Microsoft\Windows\CurrentVersion\Uninstall\${UNINSTALL_NAME}"
	DeleteRegKey /ifempty ${INSTALL_REGISTRY_ROOT} "Software\Jack's Mini Network\${PRODUCT_NAME}"

SectionEnd

;--------------------------------
;Uninstaller Functions

Function un.onInit

	;!insertmacro MULTIUSER_UNINIT

	!insertmacro MUI_UNGETLANGUAGE

FunctionEnd

Function LaunchLink
	ExecShell "" "$INSTDIR\Launcher.exe"
FunctionEnd
!endif

Function PrePage
	${GetParameters} $R0
	${GetOptions} $R0 "-upgrade=" $R1

	StrCmp $R1 "TRUE" upgrade not_upgrade
upgrade:
	Abort
not_upgrade:
FunctionEnd
