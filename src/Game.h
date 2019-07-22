//
// Created by Ido Mozes on 18/06/2019.
//

#ifndef SDL2_GAME_GAME_H
#define SDL2_GAME_GAME_H

#include <iostream>
#include <cmath>
#include <fstream>
#include <memory>
#include <vector>

#include "SDL.h"
#include "SDL_image.h"
#include <comdef.h>
#include "Entity.h"
#include "System.h"
#include "systems/RenderSystem.h"
#include "systems/MovementSystem.h"
#include "systems/EventSystem.h"
#include "systems/BloonsSpawnSystem.h"
#include "systems/ShotsSpawnSystem.h"
#include "systems/DraggingSystem.h"
#include "systems/RemoveEntitiesSystem.h"
#include "systems/CollisionSystem.h"
#include "systems/DamageSystem.h"
#include "systems/MenuSystem.h"
#include "systems/LoadLevelSystem.h"
#include "GameData.h"
#include "boost/filesystem.hpp"
#include <iostream>

struct TempPoint {
    short X;
    short Y;
};

class Game {
    std::vector<std::unique_ptr<System>> systems;
    std::array<Entities, N_LAYERS> layers;
    RenderSystem *renderSystem;
public:
    GameData gameData;

    explicit Game(bool fullscreen, float mapScale = 1.5);

    ~Game();

    void update();

    bool running() { return gameData.isRunning; }

    void loadMap();
};

#endif //SDL2_GAME_GAME_H
