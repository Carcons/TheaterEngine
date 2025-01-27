project(tinyfiledialogs VERSION 3.18.2)

add_library(TinyFileDialogs STATIC tinyfiledialogs.cpp)
target_include_directories(TinyFileDialogs PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

# CustomizeOutputName(TinyFileDialogs ${PROJECT_VERSION} OutputName)
# set_target_properties(
#     TinyFileDialogs
#     PROPERTIES 
#         OUTPUT_NAME ${OutputName}
#         VERSION ${PROJECT_VERSION}
# )


install(TARGETS TinyFileDialogs EXPORT TheaterEngineTargets ARCHIVE DESTINATION lib)

set_target_properties(TinyFileDialogs PROPERTIES FOLDER "Editor/Deps/TinyFileDialogs")
set(EDITOR_DEPS ${EDITOR_DEPS} TinyFileDialogs PARENT_SCOPE)