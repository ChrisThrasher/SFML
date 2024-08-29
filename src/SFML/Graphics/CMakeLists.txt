# all source files
set(SRC
    BlendMode.cpp
    Font.cpp
    Glsl.cpp
    GLCheck.cpp
    GLExtensions.cpp
    Image.cpp
    RenderStates.cpp
    RenderTexture.cpp
    RenderTarget.cpp
    RenderWindow.cpp
    Shader.cpp
    StencilMode.cpp
    Texture.cpp
    TextureSaver.cpp
    Transform.cpp
    Transformable.cpp
    View.cpp
    Shape.cpp
    CircleShape.cpp
    RectangleShape.cpp
    ConvexShape.cpp
    Sprite.cpp
    Text.cpp
    VertexArray.cpp
    VertexBuffer.cpp
    RenderTextureImplFBO.cpp
    RenderTextureImplDefault.cpp
)

# define the sfml-graphics target
sfml_add_library(Graphics
                 SOURCES ${SRC})

# setup dependencies
target_link_libraries(sfml-graphics PUBLIC SFML::Window)

# stb_image sources
target_include_directories(sfml-graphics SYSTEM PRIVATE "${PROJECT_SOURCE_DIR}/extlibs/headers/stb_image")

# glad sources
target_include_directories(sfml-graphics SYSTEM PRIVATE "${PROJECT_SOURCE_DIR}/extlibs/headers/glad/include")

# let CMake know about our additional graphics libraries paths
if(SFML_OS_WINDOWS)
    list(APPEND CMAKE_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/extlibs/headers/freetype2")
elseif(SFML_OS_MACOS)
    list(APPEND CMAKE_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/extlibs/headers/freetype2")
    list(APPEND CMAKE_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/extlibs/libs-macos/Frameworks")
elseif(SFML_OS_IOS)
    list(APPEND CMAKE_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/extlibs/headers/freetype2")
elseif(SFML_OS_ANDROID)
    list(APPEND CMAKE_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/extlibs/headers/freetype2")
endif()

# find external libraries
if(SFML_OS_ANDROID)
    target_link_libraries(sfml-graphics PRIVATE z)
elseif(SFML_OS_IOS)
    target_link_libraries(sfml-graphics PRIVATE z bz2)
endif()

find_package(Freetype REQUIRED)
target_link_libraries(sfml-graphics PRIVATE Freetype::Freetype)

# add preprocessor symbols
target_compile_definitions(sfml-graphics PRIVATE "STBI_FAILURE_USERMSG")

# Image.cpp must be compiled with the -fno-strict-aliasing
# when gcc is used; otherwise saving PNGs may crash in stb_image_write
if(SFML_COMPILER_GCC)
    set_source_files_properties(Image.cpp PROPERTIES COMPILE_FLAGS -fno-strict-aliasing)
endif()
