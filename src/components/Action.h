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
    ActionType actionType;

    static constexpr ComponentType type = ComponentType::ACTION;

    Action(Entity *entity, ActionType actionType, bool disabled = false) : Component(entity), actionType(actionType),
                                                                           disabled(disabled) {}

    ~Action() override = default;
};

#endif //SDL_GAME_ACTION_H
