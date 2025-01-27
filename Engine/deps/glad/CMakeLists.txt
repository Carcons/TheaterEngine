project(GLAD VERSION 0.1.36)

set(SRC 
    src/glad.c
)

add_library(glad STATIC ${SRC})
target_include_directories(glad PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/opengl>)
# if (MSVC)
#     target_compile_options(glad PUBLIC "/MDd" "$<$<CONFIG:Release>:/MD>")
# endif()

# CustomizeOutputName(glad ${PROJECT_VERSION} OutputName)
# set_target_properties(
#     glad
#     PROPERTIES 
#         OUTPUT_NAME ${OutputName}
#         VERSION ${PROJECT_VERSION}
# )


set_target_properties(glad PROPERTIES FOLDER "Engine/Deps/glad")

install(TARGETS glad EXPORT TheaterEngineTargets ARCHIVE DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/opengl DESTINATION ${CMAKE_INSTALL_PREFIX}/include)