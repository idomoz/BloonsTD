//
// Created by Ido Mozes on 09/07/2019.
//

#ifndef SDL_GAME_TYPE_H
#define SDL_GAME_TYPE_H

#include "../Component.h"

enum Types {
    OBSTACLE_T, TOWER_T, BLOON_T, SHOT_T, SEQUENCE_T
};

class Type : public Component {
public:
    Types value;
    static constexpr ComponentType type = ComponentType::TYPE;

    Type(Types value) : value(value) {}

    ~Type() override = default;
};

#endif //SDL_GAME_TYPE_H
