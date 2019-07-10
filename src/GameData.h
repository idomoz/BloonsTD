//
// Created by Ido Mozes on 03/07/2019.
//

#ifndef SDL_GAME_GAMEDATA_H
#define SDL_GAME_GAMEDATA_H

#include <vector>
#include <memory>
#include <unordered_map>
#include "Component.h"
#include "Settings.h"
#include "SDL.h"
constexpr char FREE = 0;
constexpr char OBSTACLE = 1;
constexpr char TOWER = 2;
constexpr char BLOON = 3;

class GameData {
public:
    bool isRunning = true;
    bool isDragging = false;
    int level = 0;
    int map = 0;
    float mapScale = 1.5;
    bool fullscreen;
    std::vector<char> path;
    char mapData[MAP_WIDTH][MAP_HEIGHT];
    Point startingPoint;
    Point finishPoint;
    std::unordered_map<std::string, SDL_Surface *> assets;
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    std::shared_ptr<Entity> selected;

    ~GameData();
};

#endif //SDL_GAME_GAMEDATA_H
