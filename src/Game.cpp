//
// Created by Ido Mozes on 18/06/2019.
//

#include <array>
#include "Game.h"


Game::Game(bool fullscreen, float mapScale) {
    gameData.mapScale = mapScale;
    gameData.fullscreen = fullscreen;
    renderSystem.init(gameData);
    loadLevel();
    auto mapEntity = new Entity();
    mapEntity->addComponent<Visibility>(renderSystem.getRenderer(), "../assets/level0.jpg");
    entities.emplace_back(mapEntity);

    auto player = new Entity();
    player->addComponent<Visibility>(renderSystem.getRenderer(), "../assets/knight.png", new SDL_Rect{0, 0, 50, 0});
    player->addComponent<Position>(gameData.startingPoint.X, gameData.startingPoint.Y);
    player->addComponent<Speed>(10);
    player->addComponent<PathIndex>(0);
    entities.emplace_back(player);
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

void Game::loadLevel() {
    gameData.path.clear();
    std::string fileName = "../assets/level" + std::to_string(gameData.level);
    std::ifstream file(fileName + "_path.data", std::ios::binary);
    unsigned int length;
    file.read((char *) &gameData.startingPoint, 4);
    file.read((char *) &length, 4);
    gameData.path.resize(length);
    file.read(&gameData.path[0], length);
    file.read((char *) &gameData.finishPoint, 4);
}


