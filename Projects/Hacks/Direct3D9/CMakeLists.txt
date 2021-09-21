file(GLOB SOURCES *.cpp *.h *.rc)

group_sources("${SOURCES}" "${CMAKE_CURRENT_SOURCE_DIR}")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

if(MSVC)
	precompiled_header(stdafx)
endif()

new_hack_static(Direct3D9 "${SOURCES}" "")

target_link_libraries(
	Direct3D9
	PRIVATE
	d3d8to9_static)
