add_executable(benchmark Benchmark.cpp)
target_link_libraries(benchmark PRIVATE SFML::Graphics)
set_target_warnings(benchmark)
