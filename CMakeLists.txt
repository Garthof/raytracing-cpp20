cmake_minimum_required(VERSION 3.25)

#
# Project basic properties
# 
project(
    RayTracing-C++20
    VERSION 1.0.0
    DESCRIPTION "Ray-tracing implementation using C++20"
    LANGUAGES CXX
)

# 
# Set compiler flags using dummy library
#
add_library(compiler_flags INTERFACE)

# Set C++20 standard required
target_compile_features(compiler_flags INTERFACE cxx_std_20)

# Set warning flags
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

target_compile_options(compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

# Configure main executable target
add_executable(rt "src/main.cpp")
target_link_libraries(rt PRIVATE compiler_flags)
