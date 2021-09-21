file(GLOB_RECURSE SOURCES *.cpp *.h)

group_sources("${SOURCES}" "${CMAKE_CURRENT_SOURCE_DIR}/source;${CMAKE_CURRENT_SOURCE_DIR}/res")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

add_library(d3d8to9_static STATIC ${SOURCES})

if(MSVC AND EMBEDDED_PDB)
	# embedd pdb
	set_target_properties(d3d8to9_static PROPERTIES COMPILE_OPTIONS "/Z7")
endif()
