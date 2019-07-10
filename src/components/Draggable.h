//
// Created by Ido Mozes on 09/07/2019.
//

#ifndef SDL_GAME_DRAGGABLE_H
#define SDL_GAME_DRAGGABLE_H
#include "../Component.h"

class Draggable: public Component{
public:
    bool isPlaceable = false;
    static constexpr ComponentType type = ComponentType::DRAGGABLE;
    Draggable() = default;
    ~Draggable() override = default;
};
#endif //SDL_GAME_DRAGGABLE_H
