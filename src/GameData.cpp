//
// Created by Ido Mozes on 07/07/2019.
//
#include "GameData.h"

GameData::~GameData() {
    for (auto &item:assets) {
        SDL_FreeSurface(item.second);
    }
    for (auto &item:assetsCache) {
        SDL_DestroyTexture(item.second);
    }
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    for (int i = 0; i < FONTS_LENGTH; ++i) {
        FC_FreeFont(fonts[i]);
    }

}

std::tuple<SDL_Texture *, SDL_Surface *> GameData::getTexture(const std::string &name) {
    SDL_Surface *surface = assets[name];
    if (auto texture = assetsCache[name])
        return std::make_tuple(texture, surface);
    SDL_Texture *newTexture = SDL_CreateTextureFromSurface(renderer, surface);
    assetsCache[name] = newTexture;
    return std::make_tuple(newTexture, surface);
}
