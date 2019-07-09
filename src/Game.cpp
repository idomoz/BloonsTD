//
// Created by Ido Mozes on 18/06/2019.
//

#include <array>

#include "Game.h"

using namespace boost::filesystem;

Game::Game(bool fullscreen, float mapScale) {
    gameData.mapScale = mapScale;
    gameData.fullscreen = fullscreen;
    path p = path("../assets/Bloons");
    directory_iterator it{p};
    for (auto &p :it) {
        gameData.assets[p.path().filename().string().substr(0, p.path().filename().string().length() - 4)] = IMG_Load(
                p.path().string().c_str());
    }
    gameData.assets["map"] = IMG_Load("../assets/map0.jpg");
    gameData.assets["upgrade_bar"] = IMG_Load("../assets/upgrade_bar.jpg");
    gameData.assets["upgrade_bar2"] = IMG_Load("../assets/upgrade_bar2.jpg");
    gameData.assets["menu"] = IMG_Load("../assets/menu.jpg");
    gameData.assets["Super_Monkey"] = IMG_Load("../assets/Super_Monkey.png");
    gameData.assets["Sniper_Monkey"] = IMG_Load("../assets/Sniper_Monkey.png");
    renderSystem = new RenderSystem();
    renderSystem->init(gameData);
    loadMap();
    std::initializer_list<std::pair<std::string, Point>> sprites[]{
            {{"map",         {SIDEBAR_WIDTH, 0}}},
            {},
            {{"upgrade_bar", {0,             0}}, {"menu", {MAP_WIDTH + SIDEBAR_WIDTH, 0}}},
            {}
    };
    for (int i = 0; i < N_LAYERS; i++) {
        for (auto &sprite :sprites[i]) {
            auto spriteEntity = new Entity();
            spriteEntity->addComponent<Visibility>(gameData.renderer, gameData.assets[sprite.first],
                                                   SDL_Rect{sprite.second.X, sprite.second.Y,
                                                            gameData.assets[sprite.first]->w,
                                                            gameData.assets[sprite.first]->h});
            layers[i].emplace_back(spriteEntity);
        }

    }
    auto button = new Entity();
    button->addComponent<Kind>("Super_Monkey");
    button->addComponent<Visibility>(gameData.renderer, gameData.assets["Super_Monkey"],
                                     SDL_Rect{SIDEBAR_WIDTH + MAP_WIDTH + 10, 10, gameData.assets["Super_Monkey"]->w,
                                              gameData.assets["Super_Monkey"]->h});
    button->addComponent<Action>(DRAG);
    layers[3].emplace_back(button);
    button = new Entity();
    button->addComponent<Kind>("Sniper_Monkey");
    button->addComponent<Visibility>(gameData.renderer, gameData.assets["Sniper_Monkey"],
                                     SDL_Rect{SIDEBAR_WIDTH + MAP_WIDTH + 100, 10, gameData.assets["Sniper_Monkey"]->w,
                                              gameData.assets["Sniper_Monkey"]->h});
    button->addComponent<Action>(DRAG);
    layers[3].emplace_back(button);
    auto s = new Entity();
    s->addComponent<Sequence>(100, 10, 0);
    s->addComponent<Kind>(std::string("Ceramic"));
    s->addComponent<Speed>(3.5);
    layers[1].emplace_back(s);
    s = new Entity();
    s->addComponent<Sequence>(100, 10, 60 * 5);
    s->addComponent<Kind>(std::string("Blue"));
    s->addComponent<Speed>(1.5);
    layers[1].emplace_back(s);

    systems.emplace_back(new EventSystem);
    systems.emplace_back(new SpawnSystem);
    systems.emplace_back(new DraggingSystem);
    systems.emplace_back(new MovementSystem);
    systems.emplace_back(renderSystem);

}

Game::~Game() {
    SDL_Quit();
}


void Game::update() {
    for (auto &system : systems) {
        system->update(layers, gameData);
    }
}

void Game::loadMap() {
    std::string fileName = "../assets/map" + std::to_string(gameData.map);
    std::ifstream obstaclesFile(fileName + "_obstacles.data", std::ios::binary);
    int x = 0, y = 0;
    for (int i = 0; i < ceilf(MAP_WIDTH * MAP_HEIGHT / 8.0); ++i) {
        unsigned char byte;
        obstaclesFile.read((char *) &byte, 1);
        for (int j = 0; j < 8; ++j) {
            char bit = (byte & int(pow(2, j))) >> j;
            gameData.mapData[x][y] = bit;
            x++;
            if(x ==MAP_WIDTH){
                x = 0;
                y++;
            }
            if (x*y >= MAP_WIDTH * MAP_HEIGHT)
                goto readPathFile;
        }
    }
    readPathFile:
    gameData.path.clear();
    std::ifstream pathFile(fileName + "_path.data", std::ios::binary);
    unsigned int length;
    pathFile.read((char *) &gameData.startingPoint, 4);
    pathFile.read((char *) &length, 4);
    gameData.path.resize(length);
    pathFile.read(&gameData.path[0], length);
    pathFile.read((char *) &gameData.finishPoint, 4);
}


