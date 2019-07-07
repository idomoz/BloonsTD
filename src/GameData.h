//
// Created by Ido Mozes on 03/07/2019.
//

#ifndef SDL_GAME_GAMEDATA_H
#define SDL_GAME_GAMEDATA_H

#include <vector>
#include <unordered_map>
#include "Component.h"
#include "SDL.h"

class GameData {
public:
    bool isRunning = true;
    int level = 0;
    int map = 0;
    float mapScale = 1.5;
    bool fullscreen;
    std::vector<char> path;
//    char **mapData;
    Point startingPoint;
    Point finishPoint;
    std::unordered_map<std::string,SDL_Surface *> assets;
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    ~GameData();
};

#endif //SDL_GAME_GAMEDATA_H
