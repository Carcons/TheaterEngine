cmake_minimum_required(VERSION 3.28)
project(TheaterEngine VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

if(CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configurations" FORCE)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

if(CMAKE_BUILD_TYPE AND NOT CMAKE_BUILD_TYPE MATCHES "Debug|Release")
    message(FATAL_ERROR "Invalid build type: ${CMAKE_BUILD_TYPE}. Choose Debug or Release.")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Binaries)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Binaries)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Binaries)

option(BUILD_SHARED_LIBS OFF)

set(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}${PROJECT_VERSION})

include(Helpers.cmake)

add_subdirectory(Engine)
add_subdirectory(Editor)
#add_subdirectory(GameLib)
add_subdirectory(GameProjectDemo)
#add_subdirectory(Editor)
#add_subdirectory(EditorProjectFiles)
#add_subdirectory(EngineTester)
#add_subdirectory(assimp-5.4.3 EXCLUDE_FROM_ALL)
#target_link_libraries(EngineTester PRIVATE assimp)

message(STATUS "Version: ${PROJECT_VERSION}")

#cmake -S . -B /build
#cmake --build /build --target install --config Release