cmake_minimum_required(VERSION 3.13)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

project(MafiaC C CXX)

# Embedded pdb in static libraries
set(EMBEDDED_PDB OFF)

# Warning level 3
set(CUSTOM_MSVC_WARNING_LEVEL 3)

include(j-common)
include(j-macros)
include(j-dependencies)
include(j-galactic)
include(j-spidermonkey)
include(j-signing)
include(j-warnings)

include_directories("${DEPENDENCIES_PATH}/include/mongoose6")
include_directories("${DEPENDENCIES_PATH}/include/lua5.3")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(Projects)
