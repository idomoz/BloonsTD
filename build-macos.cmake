# macOS configuration. Locates SDL2 + extensions and Boost via Homebrew
# (Apple Silicon /opt/homebrew or Intel /usr/local).

if(EXISTS "/opt/homebrew")
    list(APPEND CMAKE_PREFIX_PATH
            "/opt/homebrew"
            "/opt/homebrew/opt/sdl2"
            "/opt/homebrew/opt/sdl2_image"
            "/opt/homebrew/opt/sdl2_ttf"
            "/opt/homebrew/opt/sdl2_gfx"
            "/opt/homebrew/opt/sdl2_mixer"
            "/opt/homebrew/opt/boost")
elseif(EXISTS "/usr/local/Homebrew")
    list(APPEND CMAKE_PREFIX_PATH
            "/usr/local"
            "/usr/local/opt/sdl2"
            "/usr/local/opt/sdl2_image"
            "/usr/local/opt/sdl2_ttf"
            "/usr/local/opt/sdl2_gfx"
            "/usr/local/opt/sdl2_mixer"
            "/usr/local/opt/boost")
endif()

find_package(Boost REQUIRED COMPONENTS filesystem)
find_path(SDL2_INCLUDE_DIR SDL.h PATH_SUFFIXES SDL2 REQUIRED)
find_library(SDL2_LIBRARY       NAMES SDL2       REQUIRED)
find_library(SDL2_IMAGE_LIBRARY NAMES SDL2_image REQUIRED)
find_library(SDL2_TTF_LIBRARY   NAMES SDL2_ttf   REQUIRED)
find_library(SDL2_GFX_LIBRARY   NAMES SDL2_gfx   REQUIRED)
find_library(SDL2_MIXER_LIBRARY NAMES SDL2_mixer REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})

function(bloonstd_configure_target tgt)
    target_link_libraries(${tgt}
            ${SDL2_LIBRARY}
            ${SDL2_IMAGE_LIBRARY}
            ${SDL2_TTF_LIBRARY}
            ${SDL2_GFX_LIBRARY}
            ${SDL2_MIXER_LIBRARY}
            ${Boost_LIBRARIES})
endfunction()
