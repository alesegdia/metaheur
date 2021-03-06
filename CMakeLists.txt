cmake_minimum_required(VERSION 2.8)
project( metaheuristicas )

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	SET (CMAKE_C_FLAGS_INIT                "-Wall -std=c99")
	SET (CMAKE_C_FLAGS_DEBUG_INIT          "-g")
	SET (CMAKE_C_FLAGS_MINSIZEREL_INIT     "-Os -DNDEBUG")
	SET (CMAKE_C_FLAGS_RELEASE_INIT        "-O4 -DNDEBUG")
	SET (CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")

	SET (CMAKE_CXX_FLAGS_INIT                "-Wall")
	SET (CMAKE_CXX_FLAGS_DEBUG_INIT          "-g")
	SET (CMAKE_CXX_FLAGS_MINSIZEREL_INIT     "-Os -DNDEBUG")
	SET (CMAKE_CXX_FLAGS_RELEASE_INIT        "-O4 -DNDEBUG")
	SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")
	list( APPEND CMAKE_CXX_FLAGS "-std=c++11" )
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")
endif()

add_subdirectory(src)
set(CMAKE_BUILD_TYPE RELEASE)
