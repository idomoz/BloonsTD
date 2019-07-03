//
// Created by Ido Mozes on 23/06/2019.
//

#ifndef SDL2_GAME_VISIBILITY_H
#define SDL2_GAME_VISIBILITY_H

#include <fstream>
#include "SDL.h"
#include "SDL_image.h"
#include "../Component.h"

SDL_Texture *loadTexture(SDL_Renderer *renderer, const char *path);

class Visibility : public Component {
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect *dstRect;

    void loadSurface(const char *filePath) { surface = IMG_Load(filePath); }

public:
    static ComponentType getComponentType() { return ComponentType::VISIBILITY; }

    Visibility(Entity *entity, SDL_Renderer *renderer, const char *filePath, SDL_Rect *dstR = nullptr);

    ~Visibility();

    SDL_Texture *getTexture() { return texture; }

    SDL_Rect *getDstRect() { return dstRect; }

    void setDstRect(SDL_Rect *dstR) {
        dstRect = dstR ? new SDL_Rect(*dstR) : nullptr;
        delete dstR;
    }

    void setPosition(int x, int y);

    void loadTexture(SDL_Renderer *renderer, const char *filePath);

    void reloadTexture(SDL_Renderer *renderer);


};

#endif //SDL2_GAME_VISIBILITY_H
