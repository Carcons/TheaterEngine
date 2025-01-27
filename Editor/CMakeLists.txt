project(Editor)

set(EDITOR_DEPS )
set(Sources src/Editor.cpp src/Montserrat-Regular.hpp include/TheaterEditor/Editor.hpp)

add_subdirectory(deps)
add_library(Editor STATIC ${Sources})
add_library(Theater::Editor ALIAS Editor)

target_compile_definitions(Editor PUBLIC WITH_EDITOR)
target_link_libraries(Editor PUBLIC Engine)
target_include_directories(
    Editor
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
        $<INSTALL_INTERFACE:include>                                
)
target_link_libraries(Editor PRIVATE ${EDITOR_DEPS})

set_target_properties(Editor PROPERTIES FOLDER "Editor")

#### INSTALLATION ####

install(TARGETS Editor EXPORT TheaterEngineTargets ARCHIVE DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include DESTINATION ${CMAKE_INSTALL_PREFIX})