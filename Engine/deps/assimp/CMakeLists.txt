project(Assimp VERSION 3.4)

set(ASSIMP_BUILD_TESTS OFF)                 
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)          
set(ASSIMP_NO_EXPORT ON)                    
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF) 

set(ASSIMP_BUILD_FBX_IMPORTER ON)           
set(ASSIMP_BUILD_GLTF_IMPORTER ON)          
set(ASSIMP_BUILD_OBJ_IMPORTER ON)          


set(ASSIMP_BUILD_COLLADA_IMPORTER OFF)      
set(ASSIMP_BUILD_3DS_IMPORTER OFF)          
set(ASSIMP_BUILD_DAE_IMPORTER OFF)          
set(ASSIMP_BUILD_BLEND_IMPORTER OFF)        
set(ASSIMP_BUILD_X_IMPORTER OFF)   
add_subdirectory(assimp-5.4.3 EXCLUDE_FROM_ALL)
         
set_target_properties(assimp PROPERTIES BUILD_SHARED_LIBS ON FOLDER "Engine/Deps/assimp")

install(TARGETS assimp EXPORT TheaterEngineTargets LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)