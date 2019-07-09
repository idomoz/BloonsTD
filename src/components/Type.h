//
// Created by Ido Mozes on 09/07/2019.
//

#ifndef SDL_GAME_TYPE_H
#define SDL_GAME_TYPE_H
#include "../Component.h"
enum Types{
    OBSTACLE, TOWER, BLOON, RADIOS
};
class Type: public Component{
public:
    static constexpr ComponentType type = ComponentType::TYPE;
    Type(Entity *entity):Component(entity){}
};
#endif //SDL_GAME_TYPE_H
