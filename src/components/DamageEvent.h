//
// Created by Ido Mozes on 16/07/2019.
//

#ifndef SDL_GAME_DAMAGEEVENT_H
#define SDL_GAME_DAMAGEEVENT_H

#include "../Component.h"

class DamageEvent : public Component {
public:
    int damage;
    EntityP shot;
    static constexpr ComponentType type = ComponentType::DAMAGE_EVENT;

    DamageEvent(int damage, EntityP shot) : damage(damage), shot(shot) {}
};

#endif //SDL_GAME_DAMAGEEVENT_H
