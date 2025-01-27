set(TARGET Engine)
set(CMAKE_INSTALL_CONFIG_DIR ${CMAKE_INSTALL_PREFIX}/cmake)
set(ENGINE_DEPS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps)
set(ENGINE_PRIVATE_DEPS)
set(ENGINE_PUBLIC_DEPS)

RecurseSourceScan("src/" "include/" Sources PublicHeaders)
ApplySourceFilters("${Sources}" "${PublicHeaders}")

add_library(${TARGET} SHARED ${Sources} ${PublicHeaders})
add_library(Theater::Engine ALIAS ${TARGET})
set_target_properties(${TARGET} PROPERTIES FOLDER "Engine")

add_subdirectory(${ENGINE_DEPS_DIR})
include(${ENGINE_DEPS_DIR}/Packages.cmake)

target_compile_features(${TARGET} PUBLIC cxx_std_17)
target_compile_definitions(${TARGET} PRIVATE ENGINE_EXPORT)
target_include_directories(
    ${TARGET} 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
        $<INSTALL_INTERFACE:include>                                
)

message(STATUS "Engine Deps: ${ENGINE_PRIVATE_DEPS} ${ENGINE_PUBLIC_DEPS}")
target_link_libraries(${TARGET} PRIVATE ${ENGINE_PRIVATE_DEPS} PUBLIC ${ENGINE_PUBLIC_DEPS})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Sources" FILES ${Sources})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include" PREFIX "Public Headers" FILES ${PublicHeaders})


##### INSTALLATION #####
install(TARGETS ${TARGET} EXPORT TheaterEngineTargets RUNTIME DESTINATION lib ARCHIVE DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include DESTINATION ${CMAKE_INSTALL_PREFIX})

install(EXPORT TheaterEngineTargets
    FILE TheaterEngineTargets.cmake
    NAMESPACE Theater::
    DESTINATION ${CMAKE_INSTALL_CONFIG_DIR}
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/TheaterEngineConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY ExactVersion #AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/TheaterEngineConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/TheaterEngineConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_CONFIG_DIR}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/TheaterEngineConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/TheaterEngineConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_CONFIG_DIR}
)

message(STATUS "Engine Output Name: "  ${EngineOutputName})