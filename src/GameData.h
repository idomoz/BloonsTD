//
// Created by Ido Mozes on 03/07/2019.
//

#ifndef SDL_GAME_GAMEDATA_H
#define SDL_GAME_GAMEDATA_H

#include <vector>
#include "Component.h"

class GameData {
public:
    bool isRunning = true;
    int level = 0;
    float mapScale = 1.5;
    bool fullscreen;
    std::vector<char> path;
    char **map;
    Point startingPoint;
    Point finishPoint;
};

#endif //SDL_GAME_GAMEDATA_H
