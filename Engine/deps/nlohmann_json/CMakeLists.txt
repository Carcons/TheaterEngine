project(nlohmann_json VERSION 3.11.3)

add_library(nlohmann_json INTERFACE )
target_include_directories(nlohmann_json INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>)

install(TARGETS nlohmann_json EXPORT TheaterEngineTargets)

set_target_properties(nlohmann_json PROPERTIES FOLDER "Engine/Deps/nlohmann_json")