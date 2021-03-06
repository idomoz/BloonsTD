//
// Created by Ido Mozes on 08/07/2019.
//

#ifndef SDL_GAME_ACTION_H
#define SDL_GAME_ACTION_H

#include "../Component.h"

enum ActionType {
    DRAG, CLICK, DROP, SELECT
};

enum VisibleObjects {
    PLAY_FAST_FORWARD,
    NEXT_STRATEGY,
    PREVIOUS_STRATEGY,
    UPGRADE_PATH_1,
    UPGRADE_PATH_2,
    UPGRADE_PATH_3,
    MAP, UPGRADES_BACKGROUND,
    SELL_TOWER,
    OTHER
};

class Action : public Component {

public:
    bool disabled;
    ActionType actionType;

    static constexpr ComponentType type = ComponentType::ACTION;

    Action(ActionType actionType, bool disabled = false) : actionType(actionType), disabled(disabled) {}

};

#endif //SDL_GAME_ACTION_H
