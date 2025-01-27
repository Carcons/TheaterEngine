cmake_minimum_required(VERSION 3.28)
project(GameProjectDemo)

if (WIN32)
set(TARGET_PLATFORM WIN32)
endif()

set(TARGET GameProjectDemo)
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

RecurseSourceScan("Sources/" "Sources/" Sources Headers)

add_library(${TARGET}-Lib SHARED GameInterface/GameLibInterface.cpp ${Sources} ${Headers})
target_link_libraries(${TARGET}-Lib PRIVATE Theater::Engine)
target_compile_definitions(${TARGET}-Lib PRIVATE EXPORT_GAMEPLAY)

add_executable(${TARGET}-Editor GameProject/Main.cpp)
target_link_libraries(${TARGET}-Editor PRIVATE Theater::Editor)
add_dependencies(${TARGET}-Editor ${TARGET}-Lib)

add_executable(${TARGET} ${TARGET_PLATFORM} GameProject/Main.cpp)
target_link_libraries(${TARGET} PRIVATE Theater::Engine)
add_dependencies(${TARGET} ${TARGET}-Lib)


set_target_properties(${TARGET}-Lib PROPERTIES
    OUTPUT_NAME TheaterGame
    DEBUG_POSTFIX ""
)
