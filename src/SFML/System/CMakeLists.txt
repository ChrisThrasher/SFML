# all source files
set(SRC
    Clock.cpp
    Err.cpp
    Exception.cpp
    Sleep.cpp
    String.cpp
    Utils.cpp
    Vector2.cpp
    Vector3.cpp
    FileInputStream.cpp
    MemoryInputStream.cpp
)

# add platform specific sources
if(SFML_OS_WINDOWS)
    set(PLATFORM_SRC
        Win32/SleepImpl.cpp
    )
else()
    set(PLATFORM_SRC
        Unix/SleepImpl.cpp
    )

    if(SFML_OS_ANDROID)
        list(APPEND PLATFORM_SRC
            Android/Activity.cpp
            Android/NativeActivity.cpp
            Android/ResourceStream.cpp
            Android/ResourceStream.cpp
            Android/SuspendAwareClock.cpp
        )
    endif()
endif()

find_package(Threads REQUIRED)

# define the sfml-system target
sfml_add_library(System
                 SOURCES ${SRC} ${PLATFORM_SRC})

# enable precompiled headers
if (SFML_ENABLE_PCH)
    message(VERBOSE "enabling PCH for SFML library 'sfml-system' (reused as the PCH for other SFML libraries)")
    target_precompile_headers(sfml-system PRIVATE ${PROJECT_SOURCE_DIR}/src/SFML/PCH.hpp)
endif()

if(SFML_OS_ANDROID)
    # glad sources
    target_include_directories(sfml-system SYSTEM PRIVATE "${PROJECT_SOURCE_DIR}/extlibs/headers/glad/include")
endif()

target_link_libraries(sfml-system PRIVATE Threads::Threads)

if(SFML_OS_LINUX)
    target_link_libraries(sfml-system PRIVATE rt)
elseif(SFML_OS_WINDOWS)
    target_link_libraries(sfml-system PRIVATE winmm)
elseif(SFML_OS_ANDROID)
    target_link_libraries(sfml-system PRIVATE android log)
endif()
