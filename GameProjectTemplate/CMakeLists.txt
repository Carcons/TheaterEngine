cmake_minimum_required(VERSION 3.28)
project(GameProjectDemo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

set(TARGET ${CMAKE_PROJECT_NAME})
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(BIN_DIR ${PROJECT_ROOT}/Bin)

set(Sources Sources/DemoActor.cpp Sources/DemoActor.hpp)

find_package(TheaterEngine 1.0 REQUIRED)

add_library(${TARGET}-Lib SHARED EditorInterface/EditorInterface.cpp ${Sources})
target_link_libraries(${TARGET}-Lib PRIVATE Theater::Engine)
target_compile_definitions(${TARGET}-Lib PRIVATE EXPORT_GAMEPLAY)

add_executable(${TARGET} GameProject/dummy.cpp)
target_link_libraries(${TARGET} PRIVATE Theater::GameLibEditor)

set_target_properties(${TARGET}-Lib PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${BIN_DIR}
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${BIN_DIR}
    OUTPUT_NAME Game
)

set_target_properties(${TARGET} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${BIN_DIR}
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${BIN_DIR}
)

message(STATUS "TheaterEngine Dir: ${TheaterEngine_DIR}")
add_custom_command(
    TARGET ${TARGET}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Theater::Engine> ${BIN_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Theater::Editor> ${BIN_DIR}/
    COMMENT "Copying Engine libraries to ${BIN_DIR}"
)