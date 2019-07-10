//
// Created by Ido Mozes on 07/07/2019.
//

#ifndef SDL_GAME_KIND_H
#define SDL_GAME_KIND_H

#include "../Component.h"
#include <string>

class Kind : public Component {


public:
    std::string kind;
    static constexpr ComponentType type = ComponentType::KIND;

    explicit Kind(std::string kind) :kind(std::move(kind)) {}
    ~Kind() override = default;
};

#endif //SDL_GAME_KIND_H
