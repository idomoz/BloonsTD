//
// Created by Ido Mozes on 18/06/2019.
//

#ifndef SDL2_GAME_GAME_H
#define SDL2_GAME_GAME_H

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>

#include "SDL.h"
#include "SDL_image.h"

#include "Entity.h"
#include "System.h"
#include "systems/RenderSystem.h"
#include "systems/MovementSystem.h"
#include "components/Visibility.h"
#include "components/PathIndex.h"
#include "components/Speed.h"
#include "GameData.h"

class Game {
    std::vector<std::unique_ptr<System>> systems;
    std::vector<std::shared_ptr<Entity>> entities;
    RenderSystem renderSystem;
    GameData gameData;


public:
    explicit Game(bool fullscreen, float mapScale=1.5);

    ~Game();

    void handleEvents();

    void update();


    bool running() { return gameData.isRunning; }
    void loadLevel();
};

#endif //SDL2_GAME_GAME_H
