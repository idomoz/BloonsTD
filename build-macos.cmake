# macOS configuration. Locates SDL2 + extensions via Homebrew
# (Apple Silicon /opt/homebrew or Intel /usr/local).
# SDL2_gfx is no longer required: circle primitives are inlined in
# RenderSystem so the same code base can target iOS without vendoring gfx.

if(EXISTS "/opt/homebrew")
    list(APPEND CMAKE_PREFIX_PATH
            "/opt/homebrew"
            "/opt/homebrew/opt/sdl2"
            "/opt/homebrew/opt/sdl2_image"
            "/opt/homebrew/opt/sdl2_ttf"
            "/opt/homebrew/opt/sdl2_mixer")
elseif(EXISTS "/usr/local/Homebrew")
    list(APPEND CMAKE_PREFIX_PATH
            "/usr/local"
            "/usr/local/opt/sdl2"
            "/usr/local/opt/sdl2_image"
            "/usr/local/opt/sdl2_ttf"
            "/usr/local/opt/sdl2_mixer")
endif()

find_path(SDL2_INCLUDE_DIR SDL.h PATH_SUFFIXES SDL2 REQUIRED)
find_library(SDL2_LIBRARY       NAMES SDL2       REQUIRED)
find_library(SDL2_IMAGE_LIBRARY NAMES SDL2_image REQUIRED)
find_library(SDL2_TTF_LIBRARY   NAMES SDL2_ttf   REQUIRED)
find_library(SDL2_MIXER_LIBRARY NAMES SDL2_mixer REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})

function(bloonstd_configure_target tgt)
    target_link_libraries(${tgt}
            ${SDL2_LIBRARY}
            ${SDL2_IMAGE_LIBRARY}
            ${SDL2_TTF_LIBRARY}
            ${SDL2_MIXER_LIBRARY})
endfunction()
