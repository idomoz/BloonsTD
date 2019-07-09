//
// Created by Ido Mozes on 07/07/2019.
//
#include "GameData.h"

GameData::~GameData() {
    for(auto & item:assets){
        SDL_FreeSurface(item.second);
    }
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}
