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
# Options
#
option(WARNINGS_AS_ERRORS "Set warnings as errors" ON)

# 
# Set compiler flags using dummy library
#
add_library(compiler_flags INTERFACE)

# Set C++20 standard required
target_compile_features(compiler_flags INTERFACE cxx_std_20)

# Set warning flags
set(GCC_WARNING_FLAGS
    -Wall
    -Wextra # reasonable and standard
    -Wshadow # warn the user if a variable declaration shadows one from a parent context
    -Wnon-virtual-dtor # warn the user if a class with virtual functions has a non-virtual destructor. This helps
    # catch hard to track down memory errors
    -Wold-style-cast # warn for c-style casts
    -Wcast-align # warn for potential performance problem casts
    -Wunused # warn on anything being unused
    -Woverloaded-virtual # warn if you overload (not override) a virtual function
    -Wpedantic # warn if non-standard C++ is used
    -Wconversion # warn on type conversions that may lose data
    -Wsign-conversion # warn on sign conversions
    -Wnull-dereference # warn if a null dereference is detected
    -Wdouble-promotion # warn if float is implicit promoted to double
    -Wformat=2 # warn on security issues around functions that format output (ie printf)
    -Wimplicit-fallthrough # warn on statements that fallthrough without an explicit annotation
    -Wmisleading-indentation # warn if indentation implies blocks where blocks do not exist
    -Wduplicated-cond # warn if if / else chain has duplicated conditions
    -Wduplicated-branches # warn if if / else branches have duplicated code
    -Wlogical-op # warn about logical operations being used where bitwise were probably wanted
    -Wuseless-cast # warn if you perform a cast to the same type
)

if(WARNINGS_AS_ERRORS)
    list(APPEND GCC_WARNING_FLAGS -Werror)
endif()

target_compile_options(compiler_flags INTERFACE
    "$<$<COMPILE_LANG_AND_ID:CXX,GNU>:$<BUILD_INTERFACE:${GCC_WARNING_FLAGS}>>"
)

#
# Configure main executable target
#
add_executable(rt "src/main.cpp")
target_link_libraries(rt PRIVATE compiler_flags)
