cmake_minimum_required(VERSION 3.13)
project(pokengine CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_FLAGS "-Wall -g")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES_BASE ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.c ${PROJECT_SOURCE_DIR}/src/*.hpp ${PROJECT_SOURCE_DIR}/src/*.h)
file(GLOB_RECURSE SOURCES_TEST ${PROJECT_SOURCE_DIR}/test/*.cpp ${PROJECT_SOURCE_DIR}/test/*.c ${PROJECT_SOURCE_DIR}/test/*.h ${PROJECT_SOURCE_DIR}/test/*.hpp)

add_executable(pokengine ${SOURCES_BASE} ${SOURCES_TEST})

# Writing sources to a file so build.py script knows the sources
#set(SOURCES_OUTPUT_FILE "${CMAKE_BINARY_DIR}/.build_sources")
#file(WRITE ${SOURCES_OUTPUT_FILE} "")
#
#get_target_property(SOURCES_COMBINED pokengine SOURCES)
#
#foreach (SOURCE ${SOURCES_COMBINED})
#  file(APPEND ${SOURCES_OUTPUT_FILE} "${SOURCE}\n")
#endforeach()

target_include_directories(pokengine PRIVATE src)

#target_compile_definitions(pokengine PRIVATE NON_MATCHING)
target_compile_options(pokengine PRIVATE -fno-rtti -fno-exceptions)
target_compile_options(pokengine PRIVATE -Wall -Wextra -Wdeprecated)
target_compile_options(pokengine PRIVATE -Wno-unused-parameter -Wno-unused-private-field)
target_compile_options(pokengine PRIVATE -fno-strict-aliasing)
target_compile_options(pokengine PRIVATE -Wno-invalid-offsetof)

include_directories(${PROJECT_SOURCE_DIR}/src)
