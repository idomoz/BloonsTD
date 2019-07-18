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
    p = path("../assets/Icons");
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
    std::initializer_list<std::tuple<std::string, Point, float>> sprites[]{
            {{"map",         {SIDEBAR_WIDTH, 0}, 1}},
            {},
            {},
            {},
            {},
            {},
            {
             {"upgrade_bar", {0,             0}, 1},
                    {"menu", {MAP_WIDTH + SIDEBAR_WIDTH, 0}, 1},
                    {"Cash", {MAP_WIDTH + SIDEBAR_WIDTH + 20,9}, 0.6},
                    {"Lives", {MAP_WIDTH + SIDEBAR_WIDTH + 20, 31}, 0.6}
            },
    };
    for (int i = 0; i < N_LAYERS; i++) {
        for (auto[surfaceName, position, scale] :sprites[i]) {
            auto spriteEntity = new Entity();
            SDL_Surface *surface = gameData.assets[surfaceName];
            spriteEntity->addComponent<Visibility>(gameData.renderer, surface,
                                                   SDL_Rect{int(position.X), int(position.Y), int(surface->w * scale)});
            layers[i].emplace_back(spriteEntity);
        }

    }
    // Buttons

    EntityP button(new Entity());
    SDL_Surface *surface = gameData.assets["Play"];
    Point position{SIDEBAR_WIDTH + MAP_WIDTH + 67, 480};
    button->addComponent<Visibility>(gameData.renderer, surface,
                                     SDL_Rect{int(position.X - surface->w / 4), int(position.Y - surface->h / 4),
                                              int(surface->w / 2)});
    button->addComponent<Action>(CLICK);
    button->addComponent<Kind>(PLAY_FAST_FORWARD);
    layers[MENU_LAYER].emplace_back(button);
    gameData.playButton = button;

    // Towers
    auto tower = new Entity();
    tower->addComponent<Kind>(SUPER_MONKEY);
    tower->addComponent<ShotKind>(DART);
    surface = gameData.assets["SuperMonkey"];
    position = {SIDEBAR_WIDTH + MAP_WIDTH + 65, 85};
    tower->addComponent<Visibility>(gameData.renderer, surface,
                                    SDL_Rect{int(position.X - surface->w / 3), int(position.Y - surface->h / 3),
                                             int(surface->w / 1.5), 0});
    tower->addComponent<Action>(DRAG);
    tower->addComponent<Range>(100);
    tower->addComponent<Camo>();
    tower->addComponent<AttackSpeed>(15);
    tower->addComponent<Pierce>(4);
    tower->addComponent<Damage>(50);
    tower->addComponent<Distance>(150);
    tower->addComponent<Type>(TOWER_T);
    tower->addComponent<Cost>(3000);
    layers[MENU_LAYER].emplace_back(tower);
    tower = new Entity();
    position = {SIDEBAR_WIDTH + MAP_WIDTH + 190, 85};
    tower->addComponent<Kind>(SNIPER_MONKEY);
    tower->addComponent<ShotKind>(GUN);
    surface = gameData.assets["SniperMonkey"];
    tower->addComponent<Visibility>(gameData.renderer, surface,
                                    SDL_Rect{int(position.X - surface->w / 3), int(position.Y - surface->h / 3),
                                             int(surface->w / 1.5), 0});
    tower->addComponent<Action>(DRAG);
    tower->addComponent<Range>(500);
    tower->addComponent<AttackSpeed>(2);
    tower->addComponent<Pierce>(1);
    tower->addComponent<Damage>(3);
    tower->addComponent<Distance>(600);
    tower->addComponent<Cost>(200);
    tower->addComponent<Type>(TOWER_T);
    layers[MENU_LAYER].emplace_back(tower);

    tower = new Entity();
    position = {SIDEBAR_WIDTH + MAP_WIDTH + 65, 141};
    tower->addComponent<Kind>(DART_MONKEY);
    tower->addComponent<ShotKind>(DART);
    surface = gameData.assets["DartMonkey"];
    tower->addComponent<Visibility>(gameData.renderer, surface,
                                    SDL_Rect{int(position.X - surface->w / 3), int(position.Y - surface->h / 3),
                                             int(surface->w / 1.5), 0});
    tower->addComponent<Action>(DRAG);
    tower->addComponent<Range>(65);
    tower->addComponent<AttackSpeed>(2);
    tower->addComponent<Pierce>(2);
    tower->addComponent<Damage>(1);
    tower->addComponent<Distance>(80);
    tower->addComponent<Cost>(80);
    tower->addComponent<Type>(TOWER_T);
    layers[MENU_LAYER].emplace_back(tower);


    tower = new Entity();
    position = {SIDEBAR_WIDTH + MAP_WIDTH + 65, 197};
    tower->addComponent<Kind>(BOMB_TOWER);
    tower->addComponent<ShotKind>(BOMB);
    surface = gameData.assets["BombTower"];
    tower->addComponent<Visibility>(gameData.renderer, surface,
                                    SDL_Rect{int(position.X - surface->w / 3), int(position.Y - surface->h / 3),
                                             int(surface->w / 1.5), 0});
    tower->addComponent<Action>(DRAG);
    tower->addComponent<Range>(70);
    tower->addComponent<AttackSpeed>(2);
    tower->addComponent<Pierce>(1);
    tower->addComponent<Damage>(2);
    tower->addComponent<Distance>(500);
    tower->addComponent<Cost>(300);
    tower->addComponent<Spread>(30);
    tower->addComponent<Type>(TOWER_T);
    layers[MENU_LAYER].emplace_back(tower);


    tower = new Entity();
    position = {SIDEBAR_WIDTH + MAP_WIDTH + 190, 197};
    tower->addComponent<Kind>(GLUE_GUNNER);
    tower->addComponent<ShotKind>(GOO_SHOT);
    surface = gameData.assets["GlueGunner"];
    tower->addComponent<Visibility>(gameData.renderer, surface,
                                    SDL_Rect{int(position.X - surface->w / 3), int(position.Y - surface->h / 3),
                                             int(surface->w / 1.5), 0});
    tower->addComponent<Action>(DRAG);
    tower->addComponent<Range>(70);
    tower->addComponent<AttackSpeed>(2);
    tower->addComponent<Pierce>(1);
    tower->addComponent<Damage>(0);
    tower->addComponent<Distance>(100);
    tower->addComponent<Cost>(250);
    tower->addComponent<Spread>(30);
    tower->addComponent<Type>(TOWER_T);
    tower->addComponent<Goo>(CORROSIVE,120);
    layers[MENU_LAYER].emplace_back(tower);

    tower = new Entity();
    position = {SIDEBAR_WIDTH + MAP_WIDTH + 190, 141};
    tower->addComponent<Kind>(TACK_SHOOTER);
    tower->addComponent<ShotKind>(RADIAL_DART);
    surface = gameData.assets["TackShooter"];
    tower->addComponent<Visibility>(gameData.renderer, surface,
                                    SDL_Rect{int(position.X - surface->w / 3), int(position.Y - surface->h / 3),
                                             int(surface->w / 1.5), 0});
    tower->addComponent<Action>(DRAG);
    tower->addComponent<Range>(65);
    tower->addComponent<AttackSpeed>(2);
    tower->addComponent<Pierce>(1);
    tower->addComponent<Damage>(2);
    tower->addComponent<Distance>(80);
    tower->addComponent<Cost>(150);
    tower->addComponent<Type>(TOWER_T);
    layers[MENU_LAYER].emplace_back(tower);


    // Systems
    systems.emplace_back(new LoadLevelSystem);
    systems.emplace_back(new BloonsSpawnSystem);
    systems.emplace_back(new ShotsSpawnSystem);
    systems.emplace_back(new EventSystem);
    systems.emplace_back(new DraggingSystem);
    systems.emplace_back(new CollisionSystem);
    systems.emplace_back(new DamageSystem);
    systems.emplace_back(new CashSystem);
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


