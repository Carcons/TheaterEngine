project(DearImGui)

RecurseSourceScan("src" "include" Sources Headers)

add_library(DearImGui STATIC ${Sources})
target_include_directories(DearImGui PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)


install(TARGETS DearImGui EXPORT TheaterEngineTargets ARCHIVE DESTINATION lib)

set_target_properties(DearImGui PROPERTIES FOLDER "Editor/Deps/DearImGui")
set(EDITOR_DEPS ${EDITOR_DEPS} DearImGui PARENT_SCOPE)