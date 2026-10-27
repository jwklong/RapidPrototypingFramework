cmake_minimum_required(VERSION 3.21...4.2)

option(BUILD_SHARED_LIBS "Build as a shared library" OFF)
option(BUILD_DEMOS "Build the bundled demos" ON)

project(BoyFramework VERSION 1.4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_BUILD_RPATH_USE_ORIGIN ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

file(GLOB_RECURSE BOYLIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/BoyLib/*.cpp)
file(GLOB_RECURSE BOY_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Boy/*.cpp)

find_package(CURL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(OpenGL REQUIRED)

add_library(BoyFramework ${BOYLIB_SOURCES} ${BOY_SOURCES})

target_include_directories(BoyFramework PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/external)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(BoyFramework PRIVATE $<$<CONFIG:Debug>:_DEBUG>)
endif()

if (WIN32)
    target_link_libraries(BoyFramework PRIVATE CURL::libcurl SDL2::SDL2-static SDL2_image::SDL2_image-static SDL2_mixer::SDL2_mixer-static OpenGL::GL AES TinyXML)
    target_compile_definitions(BoyFramework PRIVATE CURL_STATICLIB)
else()
    target_link_libraries(BoyFramework PRIVATE CURL::libcurl SDL2 SDL2_image SDL2_mixer OpenGL::GL AES TinyXML)
    target_include_directories(BoyFramework PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_image_INCLUDE_DIRS} ${SDL2_mixer_INCLUDE_DIRS})
endif()

add_subdirectory(external)

if(BUILD_DEMOS)
    add_subdirectory(demos)
endif()
