// Path resolution for game assets across platforms.
//
// Desktop builds (macOS / Windows): assets are loaded relative to the working
// directory — the binary lives in build/ and assets live in ../assets/, which
// matches every existing `cd build && ./BloonsTD` invocation.
//
// iOS builds: assets are bundled inside the .app at runtime. SDL_GetBasePath()
// returns the bundle's Resources path, and we copy `assets/` in there as a
// folder reference, so the resolved path becomes "<bundle>/assets/<rel>".
//
// Web builds (Emscripten): assets are packed into the .data file via
// `--preload-file assets@/assets`, so the virtual filesystem path is /assets/.

#ifndef BLOONSTD_ASSETS_H
#define BLOONSTD_ASSETS_H

#include <string>
#include <SDL.h>

inline const std::string &assetRoot() {
    static const std::string root = []() {
#ifdef BLOONSTD_IOS
        char *base = SDL_GetBasePath();
        std::string s = base ? std::string(base) : "./";
        if (base) SDL_free(base);
        return s + "assets/";
#elif defined(__EMSCRIPTEN__)
        return std::string("/assets/");
#else
        return std::string("../assets/");
#endif
    }();
    return root;
}

inline std::string assetPath(const std::string &rel) {
    return assetRoot() + rel;
}

#endif // BLOONSTD_ASSETS_H
