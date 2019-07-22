//
// Created by Ido Mozes on 03/07/2019.
//

#ifndef SDL_GAME_GAMEDATA_H
#define SDL_GAME_GAMEDATA_H

#include <vector>
#include <memory>
#include <unordered_map>
#include "Component.h"
#include "Entity.h"
#include "Settings.h"
#include "SDL.h"
#include "SDL_FontCache.h"

constexpr char FREE = 0;
constexpr char OBSTACLE = 1;
constexpr char TOWER = 2;

class GameData {
public:
    int FPS = 60;
    bool isRunning = true;
    bool lost = false;
    bool isDragging = false;
    int cash = 100000;
    bool levelRunning = false;
    bool levelReady = false;
    int lives = 200;
    int level = 1;
    int finalLevel = 2;
    int map = 0;
    float mapScale;
    bool fullscreen;
    std::vector<char> path;
    char mapData[MAP_WIDTH][MAP_HEIGHT];
    Point startingPoint;
    Point finishPoint;
    std::unordered_map<std::string, SDL_Surface *> assets;
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    EntityP selected;
    FC_Font* fonts[FONTS_LENGTH];
    EntityP playButton;
    ~GameData();
};

#endif //SDL_GAME_GAMEDATA_H
