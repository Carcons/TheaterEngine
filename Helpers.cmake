# Organize source into filters
function(ApplySourceFilters sources_var headers_var)
    # Handle source files
    foreach(source IN LISTS ${sources_var})
        get_filename_component(source_path "${source}" PATH)
        string(REPLACE "/" "\\" source_path_filter "${source_path}")
        source_group("${source_path_filter}" FILES "${source}")
    endforeach()

    # Handle header files
    foreach(header IN LISTS ${headers_var})
        get_filename_component(header_path "${header}" PATH)
        string(REPLACE "/" "\\" header_path_filter "${header_path}")
        source_group("${header_path_filter}" FILES "${header}")
    endforeach()
endfunction()

# # Get Compiler ID and Version
# function(GetCompilerInfo OutputName OutputVersion)
#     if (MSVC)
#         set(CompilerVersion ${MSVC_VERSION})
#     else()
#         set(CompilerVersion ${CMAKE_CXX_COMPILER_VERSION})
#     endif()

#     string(REPLACE "." "-" CompilerVersion ${CompilerVersion})
#     string(TOLOWER ${CMAKE_CXX_COMPILER_ID} CompilerID)

#     set(${OutputName} ${CompilerID} PARENT_SCOPE)
#     set(${OutputVersion} ${CompilerVersion} PARENT_SCOPE)
# endfunction()

# # Customize Target output name 
# function(CustomizeOutputName Target Version OutputName)
#     GetCompilerInfo(CompilerID CompilerVersion)
#     string(TOLOWER ${Target} LowerTarget)
#     set(${OutputName} "${LowerTarget}${Version}-${CompilerID}${CompilerVersion}" PARENT_SCOPE)
# endfunction()

# Recursive source search
function(RecurseSourceScan sDir hDir OutputSources OutputHeaders)
    set(SourcesList 
        ${sDir}/*.cpp 
        ${sDir}/*.c 
        ${sDir}/*.hpp 
        ${sDir}/*.h
    )

    set(HeadersList
        ${hDir}/*.hpp 
        ${hDir}/*.h
    )

    set(srcs)
    set(headers)
    
    foreach(pattern ${SourcesList})
        file(GLOB_RECURSE tempSrcs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${pattern})
        list(APPEND srcs ${tempSrcs})
    endforeach()
        
    foreach(pattern ${HeadersList})
        file(GLOB_RECURSE tempHeaders RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${pattern})
        list(APPEND headers ${tempHeaders})
    endforeach()
    
    set(${OutputSources} ${srcs} PARENT_SCOPE)
    set(${OutputHeaders} ${headers} PARENT_SCOPE)
endfunction()