//
// Created by Ido Mozes on 08/07/2019.
//

#ifndef SDL_GAME_ACTION_H
#define SDL_GAME_ACTION_H

#include "../Component.h"

enum ActionType {
    DRAG, CLICK, DROP
};

class Action : public Component {

public:
    bool disabled;
    ActionType type;

    static ComponentType getComponentType() { return ComponentType::ACTION; }

    Action(Entity *entity, ActionType type, bool disabled = false) : Component(entity), type(type),
                                                                           disabled(disabled) {}
};

#endif //SDL_GAME_ACTION_H
