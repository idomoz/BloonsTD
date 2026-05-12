//#include <iostream>
#include "SDL.h"
#include "Game.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#include "Settings.h"
#include <algorithm>
static void emscriptenLoop(void *arg) {
    auto *game = static_cast<Game *>(arg);
    if (!game->running()) {
        emscripten_cancel_main_loop();
        return;
    }
    // Decouple simulation rate from display refresh.
    //
    // requestAnimationFrame fires once per vsync, which is 60 Hz on most
    // mobile screens but 120/144/240+ Hz on gaming laptops and ProMotion
    // displays. Running update() once per rAF would therefore make bloons
    // move 2–4× faster on a 144 Hz monitor than on a 60 Hz phone — the
    // exact thing the user reported.
    //
    // Fix: track wall-clock time and step the sim at exactly
    // gameData.FPS ticks/sec (60 normal, 180 fast-forward), regardless of
    // how often rAF actually fires. Matches the native build's behaviour
    // (which gets the same rate via SDL_Delay).
    static double lastMs = emscripten_get_now();
    static double accumMs = 0.0;
    double now = emscripten_get_now();
    accumMs += now - lastMs;
    lastMs = now;
    // Background tabs pause rAF, so on resume the accumulator can be huge —
    // cap it to avoid the spiral-of-death where the catch-up loop never
    // returns.
    if (accumMs > 250.0) accumMs = 250.0;

    int targetHz = std::max(60, game->gameData.FPS);
    double stepMs = 1000.0 / targetHz;
    while (accumMs >= stepMs && game->running()) {
        game->update();
        accumMs -= stepMs;
    }
}
#endif

int main(int argc, char *argv[]) {
    std::cout << "Loading game..." << std::endl;
#ifdef __EMSCRIPTEN__
    // Browser owns the event loop. Heap-allocate Game so it survives main()
    // returning — Emscripten keeps the page alive and drives the registered
    // callback via requestAnimationFrame. fps=0 → use vsync (~60Hz).
    //
    // Pick mapScale so the SDL canvas backing store matches the canvas's
    // actual on-screen size in physical pixels. Otherwise the browser
    // up- or down-samples the rendered frame and produces the blurry
    // text/sprites most visible on iPhone/iPad.
    //
    //   physical px = CSS px (from getBoundingClientRect) × devicePixelRatio
    //   mapScale    = physical px width / logical game width (1086)
    //
    // mapScale=DPR isn't enough on its own: on a wide viewport
    // (iPad landscape) the canvas CSS width is > 1086, so even at
    // backing=1086*DPR the browser still has to upscale. Computing from
    // the live canvas size handles every viewport correctly.
    //
    // Mouse/touch coords stay correct because SDL on emscripten reports
    // them in canvas-internal-pixel space, and EventSystem already
    // divides by mapScale to get logical coords.
    double cssW = 0, cssH = 0;
    emscripten_get_element_css_size("#canvas", &cssW, &cssH);
    double dpr = emscripten_get_device_pixel_ratio();
    if (!(dpr >= 1.0 && dpr <= 4.0)) dpr = 1.0;
    constexpr int LOGICAL_W = MAP_WIDTH + SIDEBAR_WIDTH + MENU_WIDTH;
    float mapScale = cssW > 0 ? (float)(cssW * dpr / LOGICAL_W) : (float)dpr;
    // Clamp: <1 would render below logical resolution; >4 wastes memory
    // (4× == 16× pixel count) without visible benefit.
    if (mapScale < 1.0f) mapScale = 1.0f;
    if (mapScale > 4.0f) mapScale = 4.0f;
    auto *game = new Game(false, mapScale);
    std::cout << "Game loaded!" << std::endl;
    emscripten_set_main_loop_arg(emscriptenLoop, game, 0, 1);
#else
    int FPS = 60, frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;
    Game game(false, 1.5);
    std::cout << "Game loaded!" << std::endl;
    while (game.running()) {
        if (game.gameData.FPS != FPS) {
            FPS = game.gameData.FPS;
            frameDelay = 1000 / FPS;
        }
        frameStart = SDL_GetTicks();

        game.update();
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
#endif
    return 0;
}
