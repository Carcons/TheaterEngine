project(freetype VERSION 2.13.3)

set(SKIP_INSTALL_HEADERS 1)
set(SKIP_INSTALL_ALL 1)
add_subdirectory(freetype-2.13.3)

# CustomizeOutputName(freetype ${PROJECT_VERSION} OutputName)
# set_target_properties(
#     freetype
#     PROPERTIES 
#         OUTPUT_NAME ${OutputName}
#         VERSION ${PROJECT_VERSION}
# )


set_target_properties(freetype PROPERTIES FOLDER "Engine/Deps/freetype")
install(TARGETS freetype EXPORT TheaterEngineTargets ARCHIVE DESTINATION lib)