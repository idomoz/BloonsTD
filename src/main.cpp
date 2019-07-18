//#include <iostream>
#include "SDL.h"
#include "Game.h"


int main(int argc, char *argv[]) {
    int FPS = 60, frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;
    Game game(false, 1.5);
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
    return 0;
}