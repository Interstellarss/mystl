cmake_minimum_required(VERSION 3.16)

project(mystl LANGUAGES CXX)

# Generate compile_commands.json for IDEs and tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Options
option(MYSTL_BUILD_EXAMPLES "Build example executables" ON)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Warnings
if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Werror=return-type
    )
endif()

# Header-only library exposing include/
add_library(mystl INTERFACE)

target_include_directories(mystl
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Example executable built from main.cpp
if(MYSTL_BUILD_EXAMPLES)
    add_executable(mystl_demo
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    )
    target_link_libraries(mystl_demo PRIVATE mystl)
endif()

# Installation (headers + export)
install(TARGETS mystl EXPORT mystlTargets)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include)

# (Optional) Export targets for package usage
install(EXPORT mystlTargets
    NAMESPACE mystl::
    DESTINATION lib/cmake/mystl
)
