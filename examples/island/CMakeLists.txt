find_package(Threads REQUIRED)

# all source files
set(SRC Island.cpp)

# define the island target
sfml_add_example(island GUI_APP
                 SOURCES ${SRC}
                 DEPENDS SFML::Graphics)

# external dependency headers
target_include_directories(island SYSTEM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(island PRIVATE Threads::Threads)
