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
#include "components/Range.h"
#include "eventComponents/MoveEntityEvent.h"
#include "eventComponents/RemoveEntityEvent.h"

typedef std::vector<std::shared_ptr<Entity>> Entities;

inline void
operator+=(Entities &originalVector, Entities &newVector) {
    originalVector.insert(originalVector.end(), std::make_move_iterator(newVector.begin()),
                          std::make_move_iterator(newVector.end()));
}

class System {
public:
    virtual void update(Entities *entities, GameData &gameData) = 0;
};

#endif //SDL2_GAME_SYSTEM_H
