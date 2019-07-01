//
// Created by Ido Mozes on 23/06/2019.
//

#include "Visibility.h"
SDL_Texture *loadTexture(SDL_Renderer * renderer, const char *path) {
    SDL_Surface *tmpSurface = IMG_Load(path);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);
    return tex;
}
Visibility::Visibility(Entity *entity, SDL_Renderer *renderer, const char *path, SDL_Rect *dstR, SDL_Rect *srcR)
        : Component(entity) {
    texture = loadTexture(renderer, path);
    setDstRect(dstR);
    setSrcRect(srcR);
}

Visibility::~Visibility() {
    delete dstRect;
    delete srcRect;
    SDL_DestroyTexture(texture);
}
