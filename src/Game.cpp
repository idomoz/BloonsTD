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
    renderSystem.init(gameData);
    loadMap();
    auto mapEntity = new Entity();
    mapEntity->addComponent<Visibility>(gameData.renderer, gameData.assets["map"]);
    entities.emplace_back(mapEntity);

    auto s = new Entity();
    s->addComponent<Sequence>(10, 60, 0);
    s->addComponent<Kind>(std::string("Red"));
    s->addComponent<Speed>(1);
    entities.emplace_back(s);
     s = new Entity();
    s->addComponent<Sequence>(10, 60, 60*5);
    s->addComponent<Kind>(std::string("Blue"));
    s->addComponent<Speed>(1.5);
    entities.emplace_back(s);

    systems.emplace_back(new SpawnSystem);
    systems.emplace_back(new MovementSystem);

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
        system->update(entities, gameData);
    }
    renderSystem.update(entities, gameData);
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


