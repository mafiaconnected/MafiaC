
if(WIN32)
	set(MODLAUNCHER_PATH "$ENV{jmodlauncher_home}" CACHE PATH "ModLauncher location")

	if(NOT EXISTS "${MODLAUNCHER_PATH}")
		message(FATAL_ERROR "Invalid ModLauncher path")
	endif()

	include_directories("${MODLAUNCHER_PATH}/include")
	set(MODLAUNCHER_LIBS "${MODLAUNCHER_PATH}/${LIB_DIRECTORY}")
endif()

if(MSVC)
	set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /LIBPATH:\"${MODLAUNCHER_LIBS}/Debug\"")
	set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /LIBPATH:\"${MODLAUNCHER_LIBS}/Release\"")
	set(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO} /LIBPATH:\"${MODLAUNCHER_LIBS}/Release\"")

	set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /LIBPATH:\"${MODLAUNCHER_LIBS}/Debug\"")
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /LIBPATH:\"${MODLAUNCHER_LIBS}/Release\"")
	set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO} /LIBPATH:\"${MODLAUNCHER_LIBS}/Release\"")
endif()

# Add directx sdk to include/linker path
if(WIN32)
	if(DEFINED ENV{DXSDK_DIR})
		include_directories($ENV{DXSDK_DIR}/Include)
		link_directories("$ENV{DXSDK_DIR}/Lib/${ENGINE_PLATFORM}")
	else()
		message(FATAL_ERROR "The DirectX SDK needs to be installed.\n\nhttps://www.microsoft.com/en-gb/download/details.aspx?id=6812")
	endif()
endif()

if(MSVC)
	# declaration of 'identifier' hides previous local declaration
	disable_warning(4456)

	# 'conversion' conversion from 'type1' to 'type2', possible loss of data
	disable_warning(4244)

	# 'equality-operator' : signed/unsigned mismatch
	disable_warning(4389)

	# export 'exportname' specified multiple times; using first specification
	disable_linker_warning(4197)
endif()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../Shared")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../d3d8")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/MafiaCNetworkShared/src")

if(WIN32)
	add_definitions(-DUNICODE)
	add_definitions(-D_UNICODE)
endif()

# So we can omit things from public release
define(RELEASE PUBLIC_RELEASE=1)

add_definitions(-DGALACTIC_OPENSSL=1)
#add_definitions(-DSTATIC_JS_API)

add_definitions(-DCURL_STATICLIB)
add_definitions(-DSDL2_STATIC)

add_definitions(-DENGINE_STATIC)
#add_definitions(-DENGINE_DLL)

add_definitions(-DASMJIT_STATIC)

add_definitions(-DRMLUI_STATIC_LIB)

set(MAFIAC_ARCHIVE_EXTERNAL true)
if(MAFIAC_ARCHIVE_EXTERNAL)
	add_definitions(-DMAFIAC_ARCHIVE_EXTERNAL)
endif()

# options for MAFIAC
add_definitions(-DMAFIAC_SCRIPTING_ALWAYS_INITIALISED=1)

get_git_info()

configure_file(${CMAKE_SOURCE_DIR}/Shared/LauncherGitVersion.h.in ${CMAKE_BINARY_DIR}/generated/LauncherGitVersion.h)

include_directories(${CMAKE_BINARY_DIR}/generated)

set(MODLAUNCHER_HACKS_STATIC true)
set(MODLAUNCHER_HACKS_FOLDER "")

function(new_hack_shared NAME SOURCES DEFINES)
	set(LIBRARY_SOURCES "")
	foreach(SOURCE ${SOURCES})
		list(APPEND LIBRARY_SOURCES ${SOURCE})
	endforeach()

	add_library("${NAME}" SHARED ${LIBRARY_SOURCES})

	set_target_properties("${NAME}" PROPERTIES RUNTIME_OUTPUT_NAME "${NAME}${RUNTIME_RELEASE_POSTFIX}")
	set_target_properties("${NAME}" PROPERTIES RUNTIME_OUTPUT_NAME_DEBUG "${NAME}${RUNTIME_DEBUG_POSTFIX}")

	target_compile_definitions("${NAME}" PRIVATE "HACKSUPPORT_HACK=${NAME}")

	foreach(DEFINE ${DEFINES})
		target_compile_definitions("${NAME}" PRIVATE ${DEFINE})
	endforeach()

	install(TARGETS "${NAME}" RUNTIME DESTINATION "${LIB_DIRECTORY}/Debug/${MODLAUNCHER_HACKS_FOLDER}" CONFIGURATIONS Debug)
	install(TARGETS "${NAME}" RUNTIME DESTINATION "${LIB_DIRECTORY}/Release/${MODLAUNCHER_HACKS_FOLDER}" CONFIGURATIONS Release)
	install(TARGETS "${NAME}" RUNTIME DESTINATION "${LIB_DIRECTORY}/RelWithDebInfo/${MODLAUNCHER_HACKS_FOLDER}" CONFIGURATIONS RelWithDebInfo)

	install_target_debug_info_to(${NAME} "${MODLAUNCHER_HACKS_FOLDER}")

	set_target_properties("${NAME}" PROPERTIES FOLDER Hacks)
endfunction()

function(new_hack_static NAME SOURCES DEFINES)
	set(LIBRARY_SOURCES "")
	foreach(SOURCE ${SOURCES})
		list(APPEND LIBRARY_SOURCES ${SOURCE})
	endforeach()

	add_library(${NAME} STATIC ${LIBRARY_SOURCES})

	target_compile_definitions(${NAME} PRIVATE "HACKSUPPORT_STATIC")
	target_compile_definitions(${NAME} PRIVATE "HACKSUPPORT_HACK=${NAME}")

	foreach(DEFINE ${DEFINES})
		target_compile_definitions(${NAME} PRIVATE ${DEFINE})
	endforeach()

	set_target_properties(${NAME} PROPERTIES FOLDER Hacks)
endfunction()

macro(import_hack NAME)
	copy_dependency_to("${MODLAUNCHER_LIBS}" ${NAME} dll Hacks)
	copy_dependency_to("${MODLAUNCHER_LIBS}" ${NAME} pdb Hacks)
endmacro()

add_subdirectory(Launcher)

add_subdirectory(d3d8to9)

add_subdirectory(Hacks)

add_subdirectory(MafiaC)

add_subdirectory(MafiaCNetworkShared)