//
// Created by Ido Mozes on 23/06/2019.
//

#ifndef SDL2_GAME_VISIBILITY_H
#define SDL2_GAME_VISIBILITY_H
#include "SDL.h"
#include "SDL_image.h"


#include "../Component.h"

SDL_Texture *loadTexture(SDL_Renderer *renderer, const char *path);

class Visibility : public Component {
    SDL_Texture *texture;
    SDL_Rect *srcRect;
    SDL_Rect *dstRect;

public:
    static ComponentType getComponentType() { return ComponentType::VISIBILITY; };

    Visibility(Entity *entity, SDL_Renderer *renderer, const char *path, SDL_Rect *dstR = nullptr,
               SDL_Rect *srcR = nullptr);

    ~Visibility();

    SDL_Texture *getTexture() { return texture; }

    SDL_Rect *getSrcRect() { return srcRect; }

    SDL_Rect *getDstRect() { return dstRect; }

    void setSrcRect(SDL_Rect *srcR) {
        srcRect = srcR ? new SDL_Rect(*srcR) : nullptr;
        delete srcR;
    }

    void setDstRect(SDL_Rect *dstR) {
        dstRect = dstR ? new SDL_Rect(*dstR) : nullptr;
        delete dstR;
    }

};

#endif //SDL2_GAME_VISIBILITY_H
