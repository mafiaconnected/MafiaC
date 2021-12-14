file(GLOB_RECURSE SOURCES *.cpp *.h *.rc)

group_sources("${SOURCES}" "${CMAKE_CURRENT_SOURCE_DIR}")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/../MafiaSDK")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/../Direct3D9")

if(MSVC)
	precompiled_header(stdafx)
endif()

new_hack_static(MultiplayerModOne "${SOURCES}" "")

target_link_libraries(
	MultiplayerModOne
	PRIVATE
	d3d8to9_static
	asmjit_static
	Direct3D9Renderer_static
	Audio_static
	LucasFont_static
	LucasGUI_static
	litehtml_LucasGUI_static
	litehtml
	gumbo
	JSScripting_static
	SquirrelScripting_static
	LuaScripting_static
	Multiplayer_static
	Network_static
	Scripting_static
	mozjs-52
	dxerr
	dxguid
	dsound
	ogg_static
	vorbis_static
	vorbisfile_static
	mongoose6_static
	curl_static
	png_static
	freetype_static
	lua53_static
	sqstdlib_static
	squirrel_static
	tinyxml_static
	crypto_static
	ssl_static)
