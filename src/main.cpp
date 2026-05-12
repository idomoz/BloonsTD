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
    // Fast-forward support. Native main() controls speed via SDL_Delay so
    // doubling/tripling gameData.FPS literally triples the loop rate. In
    // the browser requestAnimationFrame is locked to the display refresh
    // (≈60 Hz), so the FPS field can't change the loop rate — run update()
    // multiple times per frame to match the native speed-up ratio instead.
    int ticks = std::max(1, game->gameData.FPS / 60);
    for (int i = 0; i < ticks && game->running(); ++i)
        game->update();
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
