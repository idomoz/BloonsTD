//
// Created by Ido Mozes on 07/07/2019.
//
#include "GameData.h"

GameData::~GameData() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}
