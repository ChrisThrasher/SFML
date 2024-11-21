find_package(Threads REQUIRED)

# all source files
set(SRC Island.cpp)

# define the island target
sfml_add_example(island GUI_APP
                 SOURCES ${SRC}
                 DEPENDS SFML::Graphics
                 RESOURCES_DIR resources)

# external dependency headers
find_package(Stb REQUIRED)
target_include_directories(island SYSTEM PRIVATE ${Stb_INCLUDE_DIR})

target_link_libraries(island PRIVATE Threads::Threads)
