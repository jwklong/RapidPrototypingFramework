file(GLOB_RECURSE ASTEROIDS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/game/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/game/src/*.h)

add_executable(Asteroids ${ASTEROIDS_SOURCES})

target_include_directories(Asteroids PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/game/src)
target_link_libraries(Asteroids PRIVATE BoyFramework)

set_target_properties(Asteroids PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/demos/Asteroids
    BUILD_RPATH "$ORIGIN")

if(BUILD_SHARED_LIBS AND NOT WIN32)
    add_custom_command(TARGET Asteroids POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:BoyFramework>
        $<TARGET_FILE_DIR:Asteroids>
        COMMENT "Copying BoyFramework library...")
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/game/res)
    add_custom_command(TARGET Asteroids POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/game/res
        $<TARGET_FILE_DIR:Asteroids>/game/res
        COMMENT "Adding resources to Asteroids...")
endif()

if (WIN32)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_link_options(Asteroids PRIVATE -static)
    else()
        target_link_options(Asteroids PRIVATE -static -mwindows)
    endif()

    set(REQUIRED_DLLS
        libaom.dll
        libavif-16.dll
        libbrotlicommon.dll
        libbrotlidec.dll
        libbrotlienc.dll
        libcrypto-3-x64.dll
        libcurl-4.dll
        libdav1d-7.dll
        libdeflate.dll
        libFLAC.dll
        libgcc_s_seh-1.dll
        libhwy.dll
        libiconv-2.dll
        libidn2-0.dll
        libintl-8.dll
        libjbig-0.dll
        libjpeg-8.dll
        libjxl.dll
        libjxl_cms.dll
        liblcms2-2.dll
        libLerc.dll
        liblzma-5.dll
        libmpg123-0.dll
        libnghttp2-14.dll
        libnghttp3-9.dll
        libngtcp2-16.dll
        libngtcp2_crypto_ossl-0.dll
        libogg-0.dll
        libopus-0.dll
        libopusfile-0.dll
        libpng16-16.dll
        libpsl-5.dll
        librav1e.dll
        libsharpyuv-0.dll
        libssh2-1.dll
        libssl-3-x64.dll
        libstdc++-6.dll
        libSvtAv1Enc-4.dll
        libtiff-6.dll
        libunistring-5.dll
        libvorbis-0.dll
        libvorbisfile-3.dll
        libwavpack-1.dll
        libwebp-7.dll
        libwebpdemux-2.dll
        libwinpthread-1.dll
        libxmp.dll
        libyuv.dll
        libzstd.dll
        zlib1.dll
    )

    get_filename_component(MINGW_BIN_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)

    set(DLL_PATHS "")
    foreach(DLL_NAME ${REQUIRED_DLLS})
        list(APPEND DLL_PATHS "${MINGW_BIN_DIR}/${DLL_NAME}")
    endforeach()

    if(BUILD_SHARED_LIBS)
        list(APPEND DLL_PATHS $<TARGET_FILE:BoyFramework>)
    endif()

    add_custom_command(TARGET Asteroids POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${DLL_PATHS}
            $<TARGET_FILE_DIR:Asteroids>
        COMMENT "Copying runtime DLLs..."
    )
endif()
