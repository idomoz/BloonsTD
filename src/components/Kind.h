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
    static ComponentType getComponentType() { return ComponentType::KIND; }

    Kind(Entity *entity, std::string kind) : Component(entity), kind(std::move(kind)) {}
};

#endif //SDL_GAME_KIND_H
