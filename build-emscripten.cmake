# WebAssembly build via Emscripten. SDL2 + SDL2_image + SDL2_ttf + SDL2_mixer
# are bundled with emsdk via -sUSE_SDL=2 et al — no third-party clones needed,
# so this build target adds zero dependencies to the repo.
#
# Build with:
#   source /path/to/emsdk/emsdk_env.sh
#   emcmake cmake -S . -B build-web -DCMAKE_BUILD_TYPE=Release
#   cmake --build build-web -j
# Output: build-web/BloonsTD.html (+ .js, .wasm, .data). Serve over HTTP(S);
# any static host works (GitHub Pages, `python -m http.server`, ...).

# SDL ports + JPEG/PNG image formats + MP3 audio. The exact same flag list
# must appear at compile time and link time, otherwise emsdk re-fetches the
# port without the right build settings and link fails.
set(BLOONSTD_EM_PORT_FLAGS
        "-sUSE_SDL=2"
        "-sUSE_SDL_IMAGE=2"
        "-sSDL2_IMAGE_FORMATS=[\"png\",\"jpg\"]"
        "-sUSE_SDL_TTF=2"
        "-sUSE_SDL_MIXER=2"
        "-sSDL2_MIXER_FORMATS=[\"mp3\"]")

foreach(flag IN LISTS BLOONSTD_EM_PORT_FLAGS)
    add_compile_options(SHELL:${flag})
    add_link_options(SHELL:${flag})
endforeach()

function(bloonstd_configure_target tgt)
    # Pack the assets/ tree into the .data file at virtual path /assets/.
    # Assets.h's __EMSCRIPTEN__ branch reads from /assets/.
    target_link_options(${tgt} PRIVATE
            "SHELL:--preload-file ${CMAKE_CURRENT_LIST_DIR}/assets@/assets"
            # Custom HTML wrapper: just a fullscreen canvas, no Emscripten
            # chrome (no "powered by emscripten" bar, no <textarea> console).
            "SHELL:--shell-file ${CMAKE_CURRENT_LIST_DIR}/web/shell.html"
            # WASM heap. Default 16MB is too small for the loaded PNG set;
            # 64MB initial + ALLOW_MEMORY_GROWTH lets it expand on demand.
            "SHELL:-sALLOW_MEMORY_GROWTH=1"
            "SHELL:-sINITIAL_MEMORY=64MB"
            # Keep the runtime alive after main() returns so the registered
            # emscripten_set_main_loop callback keeps firing.
            "SHELL:-sEXIT_RUNTIME=0")

    # Emit BloonsTD.html alongside .js/.wasm/.data so a quick local
    # `python -m http.server` works.
    set_target_properties(${tgt} PROPERTIES SUFFIX ".html")
endfunction()
