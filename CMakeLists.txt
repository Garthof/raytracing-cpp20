cmake_minimum_required(VERSION 3.25)

project(
    RayTracing-C++20
    VERSION 1.0.0
    DESCRIPTION "Ray-tracing implementation using C++20"
    LANGUAGES CXX
)

# Set compiler flags using dummy library
add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_20)

# Configure main executable target
add_executable(rt main.cpp)
target_link_libraries(rt PRIVATE compiler_flags)
