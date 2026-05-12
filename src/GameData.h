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
#include "Audio.h"
#include "SDL.h"
#include "SDL_FontCache.h"

constexpr char FREE = 0;
constexpr char OBSTACLE = 1;
constexpr char TOWER = 2;

class GameData {
public:
    int FPS = 60;
    bool isRunning = true;
    bool isDragging = false;
    int cash = 800;
    bool levelRunning = false;
    bool levelReady = false;
    int lives = 200;
    int level = 1;
    int finalLevel = 100;
    int map = 0;
    // Source-render scale: every draw multiplies its logical coord by
    // mapScale so fonts/shapes are rasterised at the canvas-backing
    // resolution. On Retina/high-DPI screens this ends up larger than the
    // window-points scale to absorb the OS's backing-store factor.
    float mapScale;
    // Window-points scale: SDL_GetMouseState returns coords in window
    // points (NOT physical pixels) on macOS HIGHDPI windows, so mouse
    // coords are converted to logical 1086-space by dividing by this,
    // not mapScale. Equal to mapScale on platforms where backingScale=1.
    float pointsScale = 1.0f;
    bool fullscreen;
    std::vector<char> path;
    char mapData[MAP_WIDTH][MAP_HEIGHT];
    Point startingPoint;
    Point finishPoint;
    std::unordered_map<std::string, SDL_Surface *> assets;
    std::unordered_map<std::string, SDL_Texture *> assetsCache;
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    EntityP selected;
    FC_Font* fonts[FONTS_LENGTH];
    EntityP playButton;
    Audio audio;
    ~GameData();
    std::tuple<SDL_Texture *,SDL_Surface *> getTexture(const std::string &name);
};

#endif //SDL_GAME_GAMEDATA_H
