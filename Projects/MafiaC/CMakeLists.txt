file(GLOB_RECURSE SOURCES *.cpp *.h *.rc)

group_sources("${SOURCES}" "${CMAKE_CURRENT_SOURCE_DIR}")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

if(MSVC)
	precompiled_header(pch)
endif()

new_hack_shared(MafiaC "${SOURCES}" "HACKSUPPORT_STATIC")

set(LINK_CMD_DEBUG "/DEF:\"${CMAKE_CURRENT_SOURCE_DIR}/HackSupportDebug.def\"")
set(LINK_CMD_RELEASE "/DEF:\"${CMAKE_CURRENT_SOURCE_DIR}/HackSupport.def\"")

set_target_properties(MafiaC PROPERTIES LINK_FLAGS_DEBUG ${LINK_CMD_DEBUG})
set_target_properties(MafiaC PROPERTIES LINK_FLAGS_RELEASE ${LINK_CMD_RELEASE})
set_target_properties(MafiaC PROPERTIES LINK_FLAGS_MINSIZEREL ${LINK_CMD_RELEASE})
set_target_properties(MafiaC PROPERTIES LINK_FLAGS_RELWITHDEBINFO ${LINK_CMD_DEBUG})

# Hacks
target_link_libraries(
    MafiaC
    PRIVATE
	DirectInputHook
	Direct3D9
	WindowHacks
    DebugTest
	ModLoader
    DiscordRichPresence_static
    IPCServer_static
    CustomWindow_static
    CrashHandler_static
    MultiplayerModOne)

target_link_libraries(
	MafiaC
	PRIVATE
	MafiaCNetworkShared
	lua53_static
	Direct3D11Renderer_static
	RmlCore_static
	RmlDebugger_static
	RmlGUI_static
	enet
	pixman_static
	cairo_static
	ModLauncher_static
	discord-rpc_static
	HackSupport_static
	MinHook_static
	Injection_static
	asmjit_static
	crypto_static
	ssl_static
	curl_static
	FileIntegrity_static
	mongoose6_static
	Network_static
	Galactic_static
	SDL2
	sqlite3
	zlib_static
	png_static
	tinyxml_static
	Taskbar_static
	WindowsApp_static
	Gdiplus
	wldap32
	crypt32
	ws2_32
	comctl32
	uxtheme
	winmm
	imm32
	version
	shlwapi
	psapi
	dbghelp
	iphlpapi
	legacy_stdio_definitions)

if(MSVC)
	sign_me(MafiaC)
endif()
