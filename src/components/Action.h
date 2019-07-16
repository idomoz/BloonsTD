//
// Created by Ido Mozes on 08/07/2019.
//

#ifndef SDL_GAME_ACTION_H
#define SDL_GAME_ACTION_H

#include "../Component.h"

enum ActionType {
    DRAG, CLICK, DROP, SELECT
};

class Action : public Component {

public:
    bool disabled;
    ActionType actionType;

    static constexpr ComponentType type = ComponentType::ACTION;

    explicit Action(ActionType actionType, bool disabled = false) :actionType(actionType),
                                                                           disabled(disabled) {}

};

#endif //SDL_GAME_ACTION_H
