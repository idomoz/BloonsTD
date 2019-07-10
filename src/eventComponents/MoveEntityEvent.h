//
// Created by Ido Mozes on 10/07/2019.
//

#ifndef SDL_GAME_MOVEENTITYEVENT_H
#define SDL_GAME_MOVEENTITYEVENT_H
#include "../Component.h"

class MoveEntityEvent: public Component{
public:
    int toLayer;
    static constexpr ComponentType type = ComponentType::MOVE_ENTITY_EVENT;
    explicit MoveEntityEvent(int toLayer):toLayer(toLayer){}
    ~MoveEntityEvent() override = default;
};
#endif //SDL_GAME_MOVEENTITYEVENT_H
