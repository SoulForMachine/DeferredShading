cmake_minimum_required(VERSION 3.1)
project(DeferredShading)

set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
set(CMAKE_INSTALL_RPATH "$ORIGIN/")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${DeferredShading_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${DeferredShading_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${DeferredShading_BINARY_DIR})

add_subdirectory(Source)
add_subdirectory(Libs)
