# detect the OS
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(SFML_OS_WINDOWS ON)

    # don't use the OpenGL ES implementation on Windows
    set(OPENGL_ES OFF)

    # detect the architecture
    if("${CMAKE_GENERATOR_PLATFORM}" MATCHES "ARM64" OR "${MSVC_CXX_ARCHITECTURE_ID}" MATCHES "ARM64" OR "${CMAKE_SYSTEM_PROCESSOR}" MATCHES "ARM64")
        set(ARCH_ARM64 ON)
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(ARCH_X86 ON)
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(ARCH_X64 ON)
    else()
        message(FATAL_ERROR "Unsupported architecture")
        return()
    endif()
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(SFML_OS_UNIX ON)

    if(ANDROID)
        set(SFML_OS_ANDROID ON)

        # use the OpenGL ES implementation on Android
        set(OPENGL_ES ON)
    else()
        set(SFML_OS_LINUX ON)

        # don't use the OpenGL ES implementation on Linux
        set(OPENGL_ES OFF)
    endif()
elseif(CMAKE_SYSTEM_NAME MATCHES "^k?FreeBSD$")
    set(SFML_OS_FREEBSD ON)

    # don't use the OpenGL ES implementation on FreeBSD
    set(OPENGL_ES OFF)
elseif(CMAKE_SYSTEM_NAME MATCHES "^OpenBSD$")
    set(SFML_OS_OPENBSD ON)

    # don't use the OpenGL ES implementation on OpenBSD
    set(OPENGL_ES OFF)
elseif(CMAKE_SYSTEM_NAME MATCHES "^NetBSD$")
    set(SFML_OS_NETBSD ON)

    # don't use the OpenGL ES implementation on NetBSD
    set(OPENGL_ES OFF)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "iOS")
    set(SFML_OS_IOS ON)

    # use the OpenGL ES implementation on iOS
    set(OPENGL_ES ON)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(SFML_OS_MACOS ON)

    # don't use the OpenGL ES implementation on macOS
    set(OPENGL_ES OFF)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    set(SFML_OS_ANDROID ON)

    # use the OpenGL ES implementation on Android
    set(OPENGL_ES ON)

# comparing CMAKE_SYSTEM_NAME with "CYGWIN" generates a false warning depending on the CMake version
# let's avoid it so the actual error is more visible
elseif(${CYGWIN})
    message(FATAL_ERROR "Unfortunately SFML doesn't support Cygwin's 'hybrid' status between both Windows and Linux derivatives.\nIf you insist on using the GCC, please use a standalone build of MinGW without the Cygwin environment instead.")
else()
    message(FATAL_ERROR "Unsupported operating system or environment")
    return()
endif()

# detect the compiler
# Note: The detection is order is important because:
# - Visual Studio can both use MSVC and Clang
# - GNUCXX can still be set on macOS when using Clang
if(MSVC)
    set(SFML_COMPILER_MSVC ON)

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(SFML_COMPILER_CLANG_CL ON)
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(SFML_COMPILER_CLANG ON)

    execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "-v" OUTPUT_VARIABLE CLANG_COMPILER_VERSION ERROR_VARIABLE CLANG_COMPILER_VERSION)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(SFML_COMPILER_GCC ON)

    execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "-v" OUTPUT_VARIABLE GCC_COMPILER_VERSION ERROR_VARIABLE GCC_COMPILER_VERSION)
    string(REGEX MATCHALL ".*(tdm[64]*-[1-9]).*" SFML_COMPILER_GCC_TDM "${GCC_COMPILER_VERSION}")
else()
    message(WARNING "Unrecognized compiler: ${CMAKE_CXX_COMPILER_ID}. Use at your own risk.")
endif()
