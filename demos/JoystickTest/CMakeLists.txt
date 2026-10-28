file(GLOB_RECURSE JOYSTICKTEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

add_executable(JoystickTest ${JOYSTICKTEST_SOURCES})

target_include_directories(JoystickTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(JoystickTest PRIVATE BoyFramework)

set_target_properties(JoystickTest PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/demos/JoystickTest
    BUILD_RPATH "$ORIGIN")

if(BUILD_SHARED_LIBS AND NOT WIN32)
    add_custom_command(TARGET JoystickTest POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:BoyFramework>
        $<TARGET_FILE_DIR:JoystickTest>
        COMMENT "Copying BoyFramework library...")
endif()

if (WIN32)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_link_options(JoystickTest PRIVATE -static)
    else()
        target_link_options(JoystickTest PRIVATE -static -mwindows)
    endif()

    set(REQUIRED_DLLS
        libavif-16.dll
        libcurl-4.dll
        libFLAC.dll
        libjpeg-8.dll
        libjxl.dll
        libmpg123-0.dll
        libopusfile-0.dll
        libpng16-16.dll
        libtiff-6.dll
        libvorbisfile-3.dll
        libwavpack-1.dll
        libwebp-7.dll
        libwebpdemux-2.dll
        libxmp.dll
    )

    get_filename_component(MINGW_BIN_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)

    set(DLL_PATHS "")
    foreach(DLL_NAME ${REQUIRED_DLLS})
        list(APPEND DLL_PATHS "${MINGW_BIN_DIR}/${DLL_NAME}")
    endforeach()

    if(BUILD_SHARED_LIBS)
        list(APPEND DLL_PATHS $<TARGET_FILE:BoyFramework>)
    endif()

    add_custom_command(TARGET JoystickTest POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${DLL_PATHS}
            $<TARGET_FILE_DIR:JoystickTest>
        COMMENT "Copying runtime DLLs..."
    )
endif()