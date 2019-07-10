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

    Kind(Entity *entity, std::string kind) : Component(entity), kind(std::move(kind)) {}
    Kind(Entity *entity, Kind& kind) : Component(entity), kind(kind.kind) {}
    ~Kind() override = default;
};

#endif //SDL_GAME_KIND_H
