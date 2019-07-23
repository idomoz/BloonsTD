//
// Created by Ido Mozes on 23/06/2019.
//

#include "Visibility.h"


void Visibility::reloadTexture(SDL_Texture *newTexture, SDL_Surface *newSurface) {
    if (newTexture) {
        texture = newTexture;
        surface = newSurface;
    }
    if (dstRect) {
        if (dstRect->w == 0)
            dstRect->w = int((float(surface->w) / surface->h) * dstRect->h);
        else if (dstRect->h == 0)
            dstRect->h = int((float(surface->h) / surface->w) * dstRect->w);
    }

}


void Visibility::setPosition(int x, int y) {
    if (dstRect) {
        dstRect->x = x;
        dstRect->y = y;
    }
}


