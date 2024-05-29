file(GLOB_RECURSE SOURCES *.cpp *.h *.rc)

group_sources("${SOURCES}" "${CMAKE_CURRENT_SOURCE_DIR}")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

if(MSVC)
	precompiled_header(pch)
endif()

if(MSVC)
	# Disable sdl checks
	string(REGEX REPLACE "/sdl" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
	string(REGEX REPLACE "/sdl" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

	# Force static VCRT
	#foreach(flag_var
	#	CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
	#	CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
	#	CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
	#	CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
	#	if(${flag_var} MATCHES "/MD")
	#		string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
	#	endif()
	#endforeach()
endif()

new_library_executable(Launcher "${SOURCES}" "LAUNCHER_STATIC")

set_property(TARGET Launcher PROPERTY WIN32_EXECUTABLE true)
set_target_properties(Launcher PROPERTIES LINK_FLAGS "/MANIFEST:NO")

target_link_libraries(
	Launcher
	PRIVATE
	RmlCore_static
	RmlDebugger_static
	RmlGUI_static
	asmjit_static
	Audio_static
	LucasFont_static
	LucasGUI_static
	litehtml_LucasGUI_static
	litehtml
	gumbo
	Direct3D11Renderer_static
	Direct3D9Renderer_static
	OpenGLRenderer_static
	glew_static
	SDLApp_static
	enet
	pixman_static
	cairo_static
	ModLauncher_static
	HackSupport_static
	Injection_static
	asmjit_static
	crypto_static
	ssl_static
	curl_static
	FileIntegrity_static
	ogg_static
	vorbis_static
	vorbisfile_static
	RakNet_static
	mongoose6_static
	Multiplayer_static
	Network_static
	Galactic_static
	SDL2
	sqlite3
	zlib_static
	png_static
	freetype_static
	tinyxml_static
	Taskbar_static
	WindowsApp_static
	dxerr
	dxguid
	dsound
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
	macro(copy_file_all_configurations FILE)
		install(FILES "${FILE}" DESTINATION "${LIB_DIRECTORY}/Debug" CONFIGURATIONS Debug)
		install(FILES "${FILE}" DESTINATION "${LIB_DIRECTORY}/Release" CONFIGURATIONS Release)
		install(FILES "${FILE}" DESTINATION "${LIB_DIRECTORY}/RelWithDebInfo" CONFIGURATIONS RelWithDebInfo)
	endmacro()

	macro(copy_file_all_configurations_to FILE TARGET)
		install(FILES "${FILE}" DESTINATION "${LIB_DIRECTORY}/Debug/${TARGET}" CONFIGURATIONS Debug)
		install(FILES "${FILE}" DESTINATION "${LIB_DIRECTORY}/Release/${TARGET}" CONFIGURATIONS Release)
		install(FILES "${FILE}" DESTINATION "${LIB_DIRECTORY}/RelWithDebInfo/${TARGET}" CONFIGURATIONS RelWithDebInfo)
	endmacro()

	set(DLLS_FOLDER .)

	if(MAFIAC_ARCHIVE_EXTERNAL)
		copy_file_all_configurations("${CMAKE_SOURCE_DIR}/MafiaC.tar")
	endif()
	copy_file_all_configurations("${CMAKE_SOURCE_DIR}/Setup/cacert.pem")
	copy_file_all_configurations_to("${CMAKE_SOURCE_DIR}/Setup/dbghelp.dll" ${DLLS_FOLDER})

	copy_dependency_to("${DEPENDENCIES_LIBS}" SDL2 dll ${DLLS_FOLDER})
	copy_dependency_to("${DEPENDENCIES_LIBS}" SDL2 pdb ${DLLS_FOLDER})
	copy_dependency_to("${DEPENDENCIES_LIBS}" sqlite3 dll ${DLLS_FOLDER})
	copy_dependency_to("${DEPENDENCIES_LIBS}" sqlite3 pdb ${DLLS_FOLDER})
	copy_dependency_to("${DEPENDENCIES_LIBS}" enet dll ${DLLS_FOLDER})
	copy_dependency_to("${DEPENDENCIES_LIBS}" enet pdb ${DLLS_FOLDER})

	install(DIRECTORY "${DEPENDENCIES_LIBS}/Debug/" DESTINATION "${LIB_DIRECTORY}/Debug/${DLLS_FOLDER}" CONFIGURATIONS Debug FILES_MATCHING PATTERN "bass.dll")
	install(DIRECTORY "${DEPENDENCIES_LIBS}/Release/" DESTINATION "${LIB_DIRECTORY}/Release/${DLLS_FOLDER}" CONFIGURATIONS Release FILES_MATCHING PATTERN "bass.dll")
	install(DIRECTORY "${DEPENDENCIES_LIBS}/Release/" DESTINATION "${LIB_DIRECTORY}/RelWithDebInfo/${DLLS_FOLDER}" CONFIGURATIONS RelWithDebInfo FILES_MATCHING PATTERN "bass.dll")

	install_spidermonkey_to("${DLLS_FOLDER}")

	copy_dependency("${MODLAUNCHER_LIBS}" Updater exe)
	copy_dependency("${MODLAUNCHER_LIBS}" Updater pdb)
endif()
