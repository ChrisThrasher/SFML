# all source files
set(SRC
    Clipboard.cpp
    Context.cpp
    Cursor.cpp
    GlContext.cpp
    GlResource.cpp
    Joystick.cpp
    JoystickManager.cpp
    Keyboard.cpp
    Mouse.cpp
    Touch.cpp
    Sensor.cpp
    SensorManager.cpp
    VideoMode.cpp
    Vulkan.cpp
    Window.cpp
    WindowBase.cpp
    WindowImpl.cpp
)

# add platform specific sources
if(SFML_OS_WINDOWS)
    set(PLATFORM_SRC
        Win32/CursorImpl.cpp
        Win32/ClipboardImpl.cpp
        Win32/InputImpl.cpp
        Win32/JoystickImpl.cpp
        Win32/SensorImpl.cpp
        Win32/VideoModeImpl.cpp
        Win32/VulkanImplWin32.cpp
        Win32/WindowImplWin32.cpp
    )
    if(SFML_OPENGL_ES)
        list(APPEND PLATFORM_SRC
            EGLCheck.cpp
            EglContext.cpp
        )
    else()
        list(APPEND PLATFORM_SRC
            Win32/WglContext.cpp
        )
    endif()
elseif(SFML_OS_LINUX OR SFML_OS_FREEBSD OR SFML_OS_OPENBSD OR SFML_OS_NETBSD)
    if(SFML_USE_DRM)
        add_definitions(-DSFML_USE_DRM)
        set(PLATFORM_SRC
            EGLCheck.cpp
            DRM/CursorImpl.cpp
            DRM/ClipboardImpl.cpp
            Unix/SensorImpl.cpp
            DRM/InputImpl.cpp
            DRM/VideoModeImpl.cpp
            DRM/DRMContext.cpp
            DRM/WindowImplDRM.cpp
        )
    else()
        set(PLATFORM_SRC
            Unix/CursorImpl.cpp
            Unix/ClipboardImpl.cpp
            Unix/InputImpl.cpp
            Unix/KeyboardImpl.cpp
            Unix/KeySymToKeyMapping.cpp
            Unix/KeySymToUnicodeMapping.cpp
            Unix/SensorImpl.cpp
            Unix/Display.cpp
            Unix/VideoModeImpl.cpp
            Unix/VulkanImplX11.cpp
            Unix/WindowImplX11.cpp
        )
        if(SFML_OPENGL_ES)
            list(APPEND PLATFORM_SRC
                EGLCheck.cpp
                EglContext.cpp
            )
        else()
             list(APPEND PLATFORM_SRC
                Unix/GlxContext.cpp
            )
        endif()
    endif()
    if(SFML_OS_LINUX)
        list(APPEND PLATFORM_SRC
            Unix/JoystickImpl.cpp
        )
    elseif(SFML_OS_FREEBSD)
        list(APPEND PLATFORM_SRC
            FreeBSD/JoystickImpl.cpp
        )
    elseif(SFML_OS_OPENBSD)
        list(APPEND PLATFORM_SRC
            OpenBSD/JoystickImpl.cpp
        )
    elseif(SFML_OS_NETBSD)
        list(APPEND PLATFORM_SRC
            NetBSD/JoystickImpl.cpp
        )
    endif()
elseif(SFML_OS_MACOS)
    enable_language(OBJC OBJCXX)
    set(PLATFORM_SRC
        macOS/cg_sf_conversion.mm
        macOS/CursorImpl.mm
        macOS/ClipboardImpl.mm
        macOS/InputImpl.mm
        macOS/HIDInputManager.mm
        macOS/HIDJoystickManager.cpp
        macOS/JoystickImpl.cpp
        macOS/NSImage+raw.mm
        macOS/SensorImpl.cpp
        macOS/SFApplication.m
        macOS/SFApplicationDelegate.m
        macOS/SFContext.mm
        macOS/SFKeyboardModifiersHelper.mm
        macOS/SFOpenGLView.mm
        macOS/SFOpenGLView+keyboard.mm
        macOS/SFOpenGLView+mouse.mm
        macOS/SFSilentResponder.m
        macOS/SFWindow.m
        macOS/SFWindowController.mm
        macOS/SFViewController.mm
        macOS/VideoModeImpl.cpp
        macOS/WindowImplCocoa.mm
        macOS/AutoreleasePoolWrapper.mm
    )
elseif(SFML_OS_IOS)
    enable_language(OBJCXX)
    set(PLATFORM_SRC
        iOS/CursorImpl.cpp
        iOS/ClipboardImpl.mm
        iOS/EaglContext.mm
        iOS/InputImpl.mm
        iOS/JoystickImpl.mm
        iOS/SensorImpl.mm
        iOS/VideoModeImpl.mm
        iOS/WindowImplUIKit.mm
        iOS/SFAppDelegate.mm
        iOS/SFView.mm
        iOS/SFViewController.mm
        iOS/SFMain.mm
    )
elseif(SFML_OS_ANDROID)
    set(PLATFORM_SRC
        EGLCheck.cpp
        EglContext.cpp
        Android/CursorImpl.cpp
        Android/ClipboardImpl.cpp
        Android/WindowImplAndroid.cpp
        Android/VideoModeImpl.cpp
        Android/InputImpl.cpp
        Android/JoystickImpl.cpp
        Android/SensorImpl.cpp
    )
endif()

# define the sfml-window target
sfml_add_library(Window
                 SOURCES ${SRC} ${PLATFORM_SRC})
# DRM libraries
if(SFML_OS_LINUX OR SFML_OS_FREEBSD OR SFML_OS_OPENBSD OR SFML_OS_NETBSD)
    if(SFML_USE_DRM)
        find_package(DRM REQUIRED)
        find_package(GBM REQUIRED)
        target_link_libraries(sfml-window PRIVATE DRM::DRM GBM::GBM)
    else()
        find_package(X11 REQUIRED COMPONENTS Xrandr Xcursor Xi)
        target_link_libraries(sfml-window PRIVATE X11::X11 X11::Xrandr X11::Xcursor X11::Xi)
    endif()
endif()
target_link_libraries(sfml-window PUBLIC SFML::System)

# glad sources
target_include_directories(sfml-window SYSTEM PRIVATE "${PROJECT_SOURCE_DIR}/extlibs/headers/glad/include")

# When static linking on macOS, we need to add this flag for objective C to work
# https://developer.apple.com/library/archive/qa/qa1490/_index.html
if((NOT BUILD_SHARED_LIBS) AND SFML_OS_MACOS)
    target_link_libraries(sfml-window PRIVATE -ObjC)
endif()

# Vulkan headers
target_include_directories(sfml-window SYSTEM PRIVATE "${PROJECT_SOURCE_DIR}/extlibs/headers/vulkan")

# CMake 3.11 and later prefer to choose GLVND, but we choose legacy OpenGL for backward compatibility
# (unless the OpenGL_GL_PREFERENCE was explicitly set)
# See CMP0072 for more details (cmake --help-policy CMP0072)
if(NOT OpenGL_GL_PREFERENCE)
    set(OpenGL_GL_PREFERENCE "LEGACY")
endif()

if(SFML_OS_IOS)
    target_link_libraries(sfml-window PRIVATE "-framework OpenGLES")
elseif(SFML_OS_ANDROID)
    find_package(EGL REQUIRED)
    find_package(GLES REQUIRED)
    target_link_libraries(sfml-window PRIVATE EGL::EGL GLES::GLES)
else()
    find_package(OpenGL REQUIRED COMPONENTS OpenGL)
    target_link_libraries(sfml-window PRIVATE OpenGL::GL)
endif()

if(SFML_OS_WINDOWS AND NOT SFML_COMPILER_MSVC)
    include(CheckIncludeFileCXX)
    check_include_file_cxx(dinput.h DINPUT_H_FOUND)
    if(NOT DINPUT_H_FOUND)
        target_include_directories(sfml-window SYSTEM PRIVATE "${PROJECT_SOURCE_DIR}/extlibs/headers/mingw")
    endif()
endif()

if(SFML_OS_LINUX)
    find_package(UDev REQUIRED)
    target_link_libraries(sfml-window PRIVATE UDev::UDev dl)
elseif(SFML_OS_WINDOWS)
    target_link_libraries(sfml-window PRIVATE winmm gdi32)
elseif(SFML_OS_FREEBSD)
    target_link_libraries(sfml-window PRIVATE usbhid)
elseif(SFML_OS_MACOS)
    target_link_libraries(sfml-window PRIVATE "-framework Foundation" "-framework AppKit" "-framework IOKit" "-framework Carbon")
elseif(SFML_OS_IOS)
    target_link_libraries(sfml-window PUBLIC "-framework Foundation" "-framework UIKit" "-framework CoreGraphics" "-framework QuartzCore" "-framework CoreMotion")
elseif(SFML_OS_ANDROID)
    target_link_libraries(sfml-window PRIVATE android)
endif()

# on some platforms (e.g. Raspberry Pi 3 armhf), GCC requires linking libatomic to use <atomic> features
# that aren't supported by native CPU instructions (64-bit atomic operations on 32-bit architecture)
if(SFML_COMPILER_GCC)
    include(CheckCXXSourceCompiles)
    check_cxx_source_compiles("#include <atomic>
        int main(){std::atomic_ullong x(1); return x.fetch_add(1);}" ATOMIC_TEST)
    if(NOT ATOMIC_TEST)
        target_link_libraries(sfml-window PRIVATE atomic)
    endif()
endif()
