project(stb_image VERSION 2.29)

add_library(stb_image STATIC stb_image.cpp)
target_include_directories(stb_image PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> )

# CustomizeOutputName(stb_image ${PROJECT_VERSION} OutputName)
# set_target_properties(
#     stb_image
#     PROPERTIES 
#         OUTPUT_NAME ${OutputName}
#         VERSION ${PROJECT_VERSION}
# )

set_target_properties(stb_image PROPERTIES FOLDER "Engine/Deps/stb_image")

install(TARGETS 
    stb_image 
    EXPORT TheaterEngineTargets 
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)