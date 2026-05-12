# iOS configuration. Builds SDL2, SDL2_image, SDL2_ttf, and SDL2_mixer from
# vendored sources under third_party/ as static libraries linked directly
# into the .app bundle. App Store rules forbid loose dylibs/frameworks the
# app didn't sign itself, so static linkage keeps things simple.
#
# Expects the user to have cloned the four SDL2 repos into:
#   third_party/SDL
#   third_party/SDL_image
#   third_party/SDL_ttf
#   third_party/SDL_mixer
#
# Use the SDL2 release branches (release-2.30.x or 2.32.x). See README.

set(BLOONSTD_THIRD_PARTY "${CMAKE_CURRENT_LIST_DIR}/third_party")

foreach(dep IN ITEMS SDL SDL_image SDL_ttf SDL_mixer)
    if(NOT EXISTS "${BLOONSTD_THIRD_PARTY}/${dep}/CMakeLists.txt")
        message(FATAL_ERROR
                "Missing third_party/${dep}. Clone the SDL2 source trees first; "
                "see README's 'iOS build' section for the exact commands.")
    endif()
endforeach()

# Force static everywhere — iOS apps cannot ship arbitrary dylibs.
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# Trim SDL surface area so iOS link is fast and small.
set(SDL_TEST OFF CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)

# Image: enable JPEG/PNG only (we use both); skip TIFF/WEBP/AVIF/etc.
set(SDL2IMAGE_VENDORED ON CACHE BOOL "" FORCE)
set(SDL2IMAGE_INSTALL OFF CACHE BOOL "" FORCE)
set(SDL2IMAGE_SAMPLES OFF CACHE BOOL "" FORCE)
set(SDL2IMAGE_PNG ON CACHE BOOL "" FORCE)
set(SDL2IMAGE_JPG ON CACHE BOOL "" FORCE)
set(SDL2IMAGE_TIF OFF CACHE BOOL "" FORCE)
set(SDL2IMAGE_WEBP OFF CACHE BOOL "" FORCE)
set(SDL2IMAGE_AVIF OFF CACHE BOOL "" FORCE)
set(SDL2IMAGE_BMP ON CACHE BOOL "" FORCE)

# TTF: vendor FreeType so we don't need a system FreeType on iOS.
set(SDL2TTF_VENDORED ON CACHE BOOL "" FORCE)
set(SDL2TTF_INSTALL OFF CACHE BOOL "" FORCE)
set(SDL2TTF_SAMPLES OFF CACHE BOOL "" FORCE)
set(SDL2TTF_HARFBUZZ OFF CACHE BOOL "" FORCE)

# Mixer: vendor MP3 (mpg123/minimp3) — Pop1.mp3 etc. are MP3.
set(SDL2MIXER_VENDORED ON CACHE BOOL "" FORCE)
set(SDL2MIXER_INSTALL OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_SAMPLES OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_MP3 ON CACHE BOOL "" FORCE)
set(SDL2MIXER_MP3_MPG123 OFF CACHE BOOL "" FORCE)   # use minimp3 instead
set(SDL2MIXER_MP3_MINIMP3 ON CACHE BOOL "" FORCE)
set(SDL2MIXER_OGG OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_OPUS OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_FLAC OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_MOD OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_MIDI OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_WAVPACK OFF CACHE BOOL "" FORCE)

add_subdirectory("${BLOONSTD_THIRD_PARTY}/SDL"        sdl_build       EXCLUDE_FROM_ALL)
add_subdirectory("${BLOONSTD_THIRD_PARTY}/SDL_image"  sdl_image_build EXCLUDE_FROM_ALL)
add_subdirectory("${BLOONSTD_THIRD_PARTY}/SDL_ttf"    sdl_ttf_build   EXCLUDE_FROM_ALL)
add_subdirectory("${BLOONSTD_THIRD_PARTY}/SDL_mixer"  sdl_mixer_build EXCLUDE_FROM_ALL)

# SDL2 emits both SDL2-static and SDL2main; iOS uses SDL2main as the bundle's
# entry-point shim that calls our `int main(...)`.
function(bloonstd_configure_target tgt)
    target_compile_definitions(${tgt} PRIVATE BLOONSTD_IOS=1)

    target_include_directories(${tgt} PRIVATE
            "${BLOONSTD_THIRD_PARTY}/SDL/include"
            "${BLOONSTD_THIRD_PARTY}/SDL_image/include"
            "${BLOONSTD_THIRD_PARTY}/SDL_ttf"
            "${BLOONSTD_THIRD_PARTY}/SDL_mixer/include")

    target_link_libraries(${tgt} PRIVATE
            SDL2::SDL2-static
            SDL2::SDL2main
            SDL2_image::SDL2_image-static
            SDL2_ttf::SDL2_ttf-static
            SDL2_mixer::SDL2_mixer-static)

    # SDL2_image's IMG_ImageIO.m calls into ImageIO + UTType for GIF/TIFF
    # detection. Those symbols live in two iOS system frameworks.
    target_link_libraries(${tgt} PRIVATE
            "-framework ImageIO"
            "-framework MobileCoreServices")

    # iOS app bundle metadata.
    set_target_properties(${tgt} PROPERTIES
            MACOSX_BUNDLE TRUE
            MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_LIST_DIR}/ios/Info.plist.in"
            MACOSX_BUNDLE_BUNDLE_NAME "BloonsTD"
            MACOSX_BUNDLE_GUI_IDENTIFIER "com.idmozes.BloonsTD"
            MACOSX_BUNDLE_BUNDLE_VERSION "1.0"
            MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
            XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2"  # iPhone+iPad
            XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ""
            XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO"
            XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "NO")

    # Bundle the entire assets/ directory into the .app at runtime path
    # `Resources/assets/...`. SDL_GetBasePath() on iOS returns the bundle's
    # Resources folder, so Assets.h forms `<bundle>/Resources/assets/...`.
    file(GLOB_RECURSE ASSET_FILES "${CMAKE_CURRENT_LIST_DIR}/assets/*")
    foreach(_f IN LISTS ASSET_FILES)
        file(RELATIVE_PATH _rel "${CMAKE_CURRENT_LIST_DIR}" "${_f}")
        get_filename_component(_dir "${_rel}" DIRECTORY)
        target_sources(${tgt} PRIVATE "${_f}")
        set_source_files_properties("${_f}" PROPERTIES
                MACOSX_PACKAGE_LOCATION "Resources/${_dir}"
                HEADER_FILE_ONLY TRUE)
    endforeach()
endfunction()
