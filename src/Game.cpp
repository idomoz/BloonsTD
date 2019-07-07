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
    gameData.assets["menu"] = IMG_Load("../assets/menu.jpg");
    renderSystem.init(gameData);
    loadMap();
    auto mapEntity = new Entity();
    mapEntity->addComponent<Visibility>(gameData.renderer, gameData.assets["map"],
                                        SDL_Rect{150, 0, gameData.assets["map"]->w, gameData.assets["map"]->h});
    layers[0].emplace_back(mapEntity);
    auto upgrade_bar = new Entity();
    upgrade_bar->addComponent<Visibility>(gameData.renderer, gameData.assets["upgrade_bar"],
                                        SDL_Rect{0, 0, gameData.assets["upgrade_bar"]->w, gameData.assets["upgrade_bar"]->h});
    layers[1].emplace_back(upgrade_bar);
    auto menu = new Entity();
    menu->addComponent<Visibility>(gameData.renderer, gameData.assets["menu"],
                                        SDL_Rect{685+150, 0, gameData.assets["menu"]->w, gameData.assets["menu"]->h});
    layers[1].emplace_back(menu);

    auto s = new Entity();
    s->addComponent<Sequence>(100, 10, 0);
    s->addComponent<Kind>(std::string("Ceramic"));
    s->addComponent<Speed>(3.5);
    layers[0].emplace_back(s);
    s = new Entity();
    s->addComponent<Sequence>(100, 10, 60 * 5);
    s->addComponent<Kind>(std::string("Blue"));
    s->addComponent<Speed>(1.5);
    layers[0].emplace_back(s);

    systems.emplace_back(new SpawnSystem);
    systems.emplace_back(new MovementSystem);
    systems.emplace_back(&renderSystem);

}

Game::~Game() {
    SDL_Quit();
}

void Game::handleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            gameData.isRunning = false;
            break;
        default:
            break;
    }
}

void Game::update() {
    for (auto &system : systems) {
        system->update(layers, gameData);
    }
}

void Game::loadMap() {
    gameData.path.clear();
    std::string fileName = "../assets/map" + std::to_string(gameData.map);
    std::ifstream file(fileName + "_path.data", std::ios::binary);
    unsigned int length;
    file.read((char *) &gameData.startingPoint, 4);
    file.read((char *) &length, 4);
    gameData.path.resize(length);
    file.read(&gameData.path[0], length);
    file.read((char *) &gameData.finishPoint, 4);
}


