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
    p = path("../assets/Sprites");
    it = directory_iterator{p};
    for (auto &p :it) {
        gameData.assets[p.path().filename().string().substr(0, p.path().filename().string().length() - 4)] = IMG_Load(
                p.path().string().c_str());
    }
    gameData.assets["map"] = IMG_Load("../assets/map0.jpg");
    gameData.assets["upgrade_bar"] = IMG_Load("../assets/upgrade_bar.jpg");
    gameData.assets["upgrade_bar2"] = IMG_Load("../assets/upgrade_bar2.jpg");
    gameData.assets["menu"] = IMG_Load("../assets/menu.jpg");
    renderSystem = new RenderSystem();
    renderSystem->init(gameData);
    loadMap();
    std::initializer_list<std::pair<std::string, Point>> sprites[]{
            {{"map",         {SIDEBAR_WIDTH, 0}}},
            {},
            {},
            {},
            {},
            {},
            {{"upgrade_bar", {0,             0}}, {"menu", {MAP_WIDTH + SIDEBAR_WIDTH, 0}}},
    };
    for (int i = 0; i < N_LAYERS; i++) {
        for (auto &sprite :sprites[i]) {
            auto spriteEntity = new Entity();
            spriteEntity->addComponent<Visibility>(gameData.renderer, gameData.assets[sprite.first],
                                                   SDL_Rect{int(sprite.second.X), int(sprite.second.Y),
                                                            gameData.assets[sprite.first]->w,
                                                            gameData.assets[sprite.first]->h});
            layers[i].emplace_back(spriteEntity);
        }

    }


    // Towers
    auto tower = new Entity();
    tower->addComponent<Kind>(SUPER_MONKEY);
    tower->addComponent<ShotKind>(DART);
    SDL_Surface *surface = gameData.assets["SuperMonkey"];
    tower->addComponent<Visibility>(gameData.renderer, surface,
                                    SDL_Rect{SIDEBAR_WIDTH + MAP_WIDTH + 10, 10, int(surface->w / 1.5), 0});
    tower->addComponent<Action>(DRAG);
    tower->addComponent<Range>(100);
    tower->addComponent<AttackSpeed>(30);
    tower->addComponent<Pierce>(4);
    tower->addComponent<Damage>(21);
    tower->addComponent<Distance>(150);
    tower->addComponent<Type>(TOWER_T);
    layers[MENU_LAYER].emplace_back(tower);
    tower = new Entity();
    tower->addComponent<Kind>(SNIPER_MONKEY);
    tower->addComponent<ShotKind>(DART);
    surface = gameData.assets["SniperMonkey"];
    tower->addComponent<Visibility>(gameData.renderer, surface,
                                    SDL_Rect{SIDEBAR_WIDTH + MAP_WIDTH + 100, 10, int(surface->w / 1.5), 0});
    tower->addComponent<Action>(DRAG);
    tower->addComponent<Range>(100);
    tower->addComponent<AttackSpeed>(6);
    tower->addComponent<Pierce>(5);
    tower->addComponent<Damage>(1);
    tower->addComponent<Distance>(300);
    tower->addComponent<Type>(TOWER_T);
    layers[MENU_LAYER].emplace_back(tower);


    // Sequences
    auto s = new Entity();
    s->addComponent<Sequence>(1, 1, 0);
    s->addComponent<Kind>(BAD);
    s->addComponent<Type>(SEQUENCE_T);
    layers[SEQUENCES_LAYER].emplace_back(s);

//    s = new Entity();
//    s->addComponent<Sequence>(100, 20.2, 60 * 5);
//    s->addComponent<Kind>(std::string("Blue"));
//    s->addComponent<Type>(SEQUENCE_T);
//    s->addComponent<Speed>(1.5);
//    layers[SEQUENCES_LAYER].emplace_back(s);


    // Systems
    systems.emplace_back(new BloonsSpawnSystem);
    systems.emplace_back(new ShotsSpawnSystem);
    systems.emplace_back(new EventSystem);
    systems.emplace_back(new DraggingSystem);
    systems.emplace_back(new CollisionSystem);
    systems.emplace_back(new DamageSystem);
    systems.emplace_back(new MovementSystem);
    systems.emplace_back(new RemoveEntitiesSystem);
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
            char bit = (byte & 2 << j) >> j;
            gameData.mapData[x][y] = bit;
            x++;
            if (x == MAP_WIDTH) {
                x = 0;
                y++;
            }
            if (x * y >= MAP_WIDTH * MAP_HEIGHT)
                goto readPathFile;
        }
    }
    readPathFile:
    gameData.path.clear();
    std::ifstream pathFile(fileName + "_path.data", std::ios::binary);
    unsigned int length;

    TempPoint startingPoint, finishPoint;
    pathFile.read((char *) &startingPoint, 4);
    pathFile.read((char *) &length, 4);
    gameData.path.resize(length);
    pathFile.read(&gameData.path[0], length);
    pathFile.read((char *) &finishPoint, 4);
    gameData.startingPoint = {float(startingPoint.X), float(startingPoint.Y)};
    gameData.finishPoint = {float(finishPoint.X), float(finishPoint.Y)};
}


