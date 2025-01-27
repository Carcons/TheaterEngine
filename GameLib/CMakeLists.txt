project(GameLibrary)

set(Sources src/Launcher.cpp)

#### GameApplicationEditor ####
add_library(GameLibEditor STATIC ${Sources})
add_library(Theater::GameLibEditor ALIAS GameLibEditor)
target_link_libraries(GameLibEditor PUBLIC Editor)
target_compile_definitions(GameLibEditor PRIVATE WITH_EDITOR)
set_target_properties(GameLibEditor PROPERTIES FOLDER "GameLibrary")

#### GameApplication ####
add_library(GameLib STATIC ${Sources})
add_library(Theater::GameLib ALIAS GameLib)
target_link_libraries(GameLib PUBLIC Engine)
set_target_properties(GameLib PROPERTIES FOLDER "GameLibrary")

#### INSTALLATION ####
install(TARGETS GameLib GameLibEditor EXPORT TheaterEngineTargets ARCHIVE DESTINATION lib)