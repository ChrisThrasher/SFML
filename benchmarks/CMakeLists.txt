find_package(Catch2)

add_executable(benchmark Benchmark.cpp)
target_link_libraries(benchmark PRIVATE SFML::Graphics Catch2::Catch2)
set_target_warnings(benchmark)
