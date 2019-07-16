//#include <iostream>
#include "SDL.h"
#include "Game.h"


int main(int argc, char *argv[]) {
    const int FPS = 120, frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;
    Game game(false, 2);
    while (game.running()) {
        frameStart = SDL_GetTicks();

        game.update();
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    return 0;
}