//
// Created by Ido Mozes on 23/06/2019.
//

#include "Visibility.h"

Visibility::Visibility(Entity *entity, SDL_Renderer *renderer, const char *filePath, SDL_Rect *dstR)
        : Component(entity) {
    setDstRect(dstR);
    loadTexture(renderer, filePath);
}

Visibility::~Visibility() {
    delete dstRect;
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void Visibility::loadTexture(SDL_Renderer *renderer, const char *filePath) {
    loadSurface(filePath);
    reloadTexture(renderer);
    if (dstRect) {
        if (dstRect->w == 0)
            dstRect->w = int((float(surface->w) / surface->h) * dstRect->h);
        else if (dstRect->h == 0)
            dstRect->h = int((float(surface->h) / surface->w) * dstRect->w);
    }

}

void Visibility::reloadTexture(SDL_Renderer *renderer) {
    texture = SDL_CreateTextureFromSurface(renderer, surface);
}

void Visibility::setPosition(int x, int y) {
    if (dstRect != nullptr) {
        dstRect->x = x;
        dstRect->y = y;
    }
}


