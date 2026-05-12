//#include <iostream>
#include "SDL.h"
#include "Game.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
static void emscriptenLoop(void *arg) {
    auto *game = static_cast<Game *>(arg);
    if (game->running()) game->update();
    else emscripten_cancel_main_loop();
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
