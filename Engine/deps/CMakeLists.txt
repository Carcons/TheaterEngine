# GLAD
add_subdirectory(${ENGINE_DEPS_DIR}/glad)
set(ENGINE_PUBLIC_DEPS
    ${ENGINE_PUBLIC_DEPS}
    OpenGL::GL
    glad
)

#assimp
add_subdirectory(${ENGINE_DEPS_DIR}/assimp)
set(ENGINE_PRIVATE_DEPS ${ENGINE_PRIVATE_DEPS} assimp)

#nlohmann_json
add_subdirectory(${ENGINE_DEPS_DIR}/nlohmann_json)
set(ENGINE_PUBLIC_DEPS ${ENGINE_PUBLIC_DEPS} nlohmann_json)

# STB_IMAGE
add_subdirectory(${ENGINE_DEPS_DIR}/stb_image)
set(ENGINE_PRIVATE_DEPS ${ENGINE_PRIVATE_DEPS} stb_image)

# Windows platform
if (WIN32)   
    #XInput
    set(ENGINE_PRIVATE_DEPS ${ENGINE_PRIVATE_DEPS} XInput.lib)
endif(WIN32)

# Freetype
add_subdirectory(freetype)
set(ENGINE_PRIVATE_DEPS ${ENGINE_PRIVATE_DEPS} freetype)



set(ENGINE_PRIVATE_DEPS ${ENGINE_PRIVATE_DEPS} PARENT_SCOPE)
set(ENGINE_PUBLIC_DEPS ${ENGINE_PUBLIC_DEPS} PARENT_SCOPE)