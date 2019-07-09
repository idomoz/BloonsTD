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
constexpr float NO_RADIOS = -1;
SDL_Texture *loadTexture(SDL_Renderer *renderer, const char *path);

class Visibility : public Component {
    SDL_Surface *surface = nullptr;
    SDL_Texture *texture = nullptr;
    std::optional<SDL_Rect> dstRect;
    std::optional<float> radios;

public:
    static constexpr inline ComponentType getComponentType() { return ComponentType::VISIBILITY; }

    Visibility(Entity *entity, SDL_Renderer *renderer, SDL_Surface *newSurface,
               std::optional<SDL_Rect> dstR = std::nullopt, std::optional<float> radios = std::nullopt);


    ~Visibility();

    SDL_Texture *getTexture() { return texture; }

    SDL_Rect *getDstRect() { return dstRect ? &dstRect.value() : nullptr; }

    float getRadios() { return radios ? radios.value() : NO_RADIOS; }

    void setRadios(std::optional<float> newRadios) { radios = newRadios; }

    void setPosition(int x, int y);

    void loadTexture(SDL_Renderer *renderer, SDL_Surface *newSurface);

    void reloadTexture(SDL_Renderer *renderer);


};

#endif //SDL2_GAME_VISIBILITY_H
