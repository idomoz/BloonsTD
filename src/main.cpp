//#include <iostream>
#include "SDL.h"
#include "Game.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
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
    auto *game = new Game(false, 1.0f);
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
