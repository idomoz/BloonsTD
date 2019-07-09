//
// Created by Ido Mozes on 23/06/2019.
//

#ifndef SDL2_GAME_SYSTEM_H
#define SDL2_GAME_SYSTEM_H
#include <initializer_list>
#include <vector>
#include <cmath>
#include "Entity.h"
#include "GameData.h"
#include "components/Sequence.h"
#include "components/Speed.h"
#include "components/Kind.h"
#include "components/Position.h"
#include "components/Visibility.h"
#include "components/PathIndex.h"
#include "components/Velocity.h"
#include "components/Draggable.h"
#include "components/Action.h"


class System{
public:
    virtual void update(std::vector<std::shared_ptr<Entity>> *entities, GameData & gameData)=0;
};
#endif //SDL2_GAME_SYSTEM_H
