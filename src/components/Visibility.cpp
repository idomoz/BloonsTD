//
// Created by Ido Mozes on 23/06/2019.
//

#include "Visibility.h"

Visibility::Visibility(Entity *entity, SDL_Renderer *renderer, SDL_Surface *newSurface, std::optional<SDL_Rect> dstR,
                       std::optional<float> radios) : Component(entity), dstRect(dstR), radios(radios) {
    loadTexture(renderer, newSurface);
}

Visibility::~Visibility() {
    SDL_DestroyTexture(texture);
}

void Visibility::loadTexture(SDL_Renderer *renderer, SDL_Surface *newSurface) {
    surface = newSurface;
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
    if (dstRect) {
        dstRect->x = x;
        dstRect->y = y;
    }
}


