//
// Created by Ido Mozes on 23/06/2019.
//

#ifndef SDL2_GAME_VISIBILITY_H
#define SDL2_GAME_VISIBILITY_H

#include <fstream>
#include <optional>
#include "SDL.h"
#include "SDL_image.h"
#include "../Component.h"


class Visibility : public Component {
    SDL_Surface *surface = nullptr;
    SDL_Texture *texture = nullptr;
    std::optional<SDL_Rect> dstRect;
public:
    float angle = 0;
    bool hidden = false;
    static constexpr ComponentType type = ComponentType::VISIBILITY;

    Visibility(SDL_Texture *newTexture, SDL_Surface *newSurface, std::optional<SDL_Rect> dstR = std::nullopt,
               float angle = 0, bool hidden = false) : dstRect(dstR), angle(angle), hidden(hidden), texture(newTexture),
                                                       surface(newSurface) { reloadTexture(); }

    SDL_Texture *getTexture() { return texture; }

    SDL_Rect *getDstRect() { return dstRect ? &dstRect.value() : nullptr; }

    void setDstRect(SDL_Rect newDstRect) { dstRect.value() = newDstRect; }

    void setPosition(int x, int y);

    void reloadTexture(SDL_Texture *newTexture = nullptr, SDL_Surface *newSurface = nullptr);


};

#endif //SDL2_GAME_VISIBILITY_H
