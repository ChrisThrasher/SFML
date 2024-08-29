# all source files
set(SRC
    AudioResource.cpp
    AudioDevice.cpp
    Listener.cpp
    Miniaudio.cpp
    MiniaudioUtils.cpp
    Music.cpp
    PlaybackDevice.cpp
    Sound.cpp
    SoundBuffer.cpp
    SoundBufferRecorder.cpp
    InputSoundFile.cpp
    OutputSoundFile.cpp
    SoundRecorder.cpp
    SoundSource.cpp
    SoundStream.cpp
    SoundFileFactory.cpp
    SoundFileReaderFlac.cpp
    SoundFileReaderMp3.cpp
    SoundFileReaderOgg.cpp
    SoundFileReaderWav.cpp
    SoundFileWriterFlac.cpp
    SoundFileWriterOgg.cpp
    SoundFileWriterWav.cpp
)

# Ensure certain files are compiled as Objective-C++
# See: https://miniaud.io/docs/manual/index.html#Building
if(SFML_OS_IOS)
    enable_language(OBJCXX)
    set_source_files_properties(Miniaudio.cpp PROPERTIES LANGUAGE OBJCXX)
endif()

# let CMake know about our additional audio libraries paths (on Android and macOS)
if(SFML_OS_MACOS)
    list(APPEND CMAKE_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/extlibs/libs-macos/Frameworks")
elseif(SFML_OS_ANDROID)
    list(APPEND CMAKE_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/extlibs/android")
endif()

# find external libraries
find_package(Vorbis REQUIRED)
find_package(FLAC REQUIRED)

# define the sfml-audio target
sfml_add_library(Audio
                 SOURCES ${SRC})

# avoids warnings in vorbisfile.h
target_compile_definitions(sfml-audio PRIVATE OV_EXCLUDE_STATIC_CALLBACKS FLAC__NO_DLL)

# disable miniaudio features we do not use
target_compile_definitions(sfml-audio PRIVATE MA_NO_MP3 MA_NO_FLAC MA_NO_ENCODING MA_NO_RESOURCE_MANAGER MA_NO_GENERATION)

# setup dependencies
target_link_libraries(sfml-audio
                      PUBLIC SFML::System
                      PRIVATE Vorbis::vorbis FLAC::FLAC)
if(SFML_OS_IOS)
    target_link_libraries(sfml-audio PRIVATE "-framework Foundation" "-framework CoreFoundation" "-framework CoreAudio" "-framework AudioToolbox" "-framework AVFoundation")
else()
    target_link_libraries(sfml-audio PRIVATE Vorbis::vorbisfile Vorbis::vorbisenc)
endif()

# miniaudio sources
target_include_directories(sfml-audio SYSTEM PRIVATE "${PROJECT_SOURCE_DIR}/extlibs/headers/miniaudio")

# minimp3 sources
target_include_directories(sfml-audio SYSTEM PRIVATE "${PROJECT_SOURCE_DIR}/extlibs/headers/minimp3")

if(SFML_OS_ANDROID)
    target_link_libraries(sfml-audio PRIVATE android OpenSLES)
endif()

if(SFML_OS_LINUX)
    target_link_libraries(sfml-audio PRIVATE dl)
endif()
