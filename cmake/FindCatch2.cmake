include(FetchContent)

if(POLICY CMP0169)
    cmake_policy(SET CMP0169 OLD)
endif()

set(CATCH_CONFIG_FAST_COMPILE ON CACHE BOOL "")
set(CATCH_CONFIG_NO_EXPERIMENTAL_STATIC_ANALYSIS_SUPPORT ON CACHE BOOL "")
FetchContent_Declare(Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.8.0
    GIT_SHALLOW ON)
FetchContent_GetProperties(Catch2)
if(NOT catch2_POPULATED)
    FetchContent_Populate(Catch2)
    add_subdirectory(${catch2_SOURCE_DIR} ${catch2_BINARY_DIR} EXCLUDE_FROM_ALL)

    include(Catch)

    # Build Catch2 in C++17 mode to enable C++17 features
    target_compile_features(Catch2 PRIVATE cxx_std_17)
    
    # Ensure that Catch2 sources and headers are not analyzed by any tools
    set_target_properties(Catch2 PROPERTIES COMPILE_OPTIONS "" EXPORT_COMPILE_COMMANDS OFF)
    set_target_properties(Catch2WithMain PROPERTIES EXPORT_COMPILE_COMMANDS OFF)
    set_target_properties(Catch2 Catch2WithMain PROPERTIES FOLDER "Dependencies")
    get_target_property(CATCH2_INCLUDE_DIRS Catch2 INTERFACE_INCLUDE_DIRECTORIES)
    target_include_directories(Catch2 SYSTEM INTERFACE ${CATCH2_INCLUDE_DIRS})
endif()
