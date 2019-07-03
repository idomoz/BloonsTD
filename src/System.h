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

constexpr uint64_t createMask( std::initializer_list<int> types) noexcept  {
    uint64_t mask = 0;
    for (int bit : types) {
        mask |= (uint64_t) pow(2, bit);
    }
    return mask;
}
class System{
public:
    virtual void update(std::vector<std::shared_ptr<Entity>> &entities, GameData & gameData)=0;
};
#endif //SDL2_GAME_SYSTEM_H
