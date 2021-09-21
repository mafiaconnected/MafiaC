file(GLOB_RECURSE SOURCES *.cpp *.h)

group_sources("${SOURCES}" "${CMAKE_CURRENT_SOURCE_DIR}/src;${CMAKE_CURRENT_SOURCE_DIR}")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

if(MSVC)
	precompiled_header(pch)
endif()

add_library(MafiaCNetworkShared STATIC ${SOURCES})
