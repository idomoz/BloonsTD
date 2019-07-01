//
// Created by Ido Mozes on 18/06/2019.
//

#include <array>
#include "Game.h"



Game::Game(bool fullscreen):renderSystem(fullscreen),isRunning(true) {
    auto player = new Entity();
    player->addComponent<Visibility>(renderSystem.getRenderer(),"../assets/knight.png",new SDL_Rect{0,0,128,128});
    entities.emplace_back(player);
    auto player2 = new Entity();
    player2->addComponent<Visibility>(renderSystem.getRenderer(),"../assets/knight.png",new SDL_Rect{0,150,128,128});
    entities.emplace_back(player2);
//    systems = {
//
//    };
}

Game::~Game() {
    SDL_Quit();
}

void Game::handleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        default:
            break;
    }
}

void Game::update() {
    renderSystem.update(entities);
}


