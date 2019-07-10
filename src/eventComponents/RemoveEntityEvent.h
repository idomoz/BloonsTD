//
// Created by Ido Mozes on 10/07/2019.
//

#ifndef SDL_GAME_REMOVEENTITYEVENT_H
#define SDL_GAME_REMOVEENTITYEVENT_H
#include "../Component.h"

class RemoveEntityEvent: public Component{
public:
    static constexpr ComponentType type = ComponentType::REMOVE_ENTITY_EVENT;
    RemoveEntityEvent() = default;
    ~RemoveEntityEvent() override = default;
};
#endif //SDL_GAME_REMOVEENTITYEVENT_H
