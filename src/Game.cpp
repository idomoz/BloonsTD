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
    gameData.assets["upgrade_bar"] = IMG_Load("../assets/upgrade_bar.png");
    gameData.assets["menu"] = IMG_Load("../assets/menu.jpg");
    gameData.assets["UpgradesBackground"] = IMG_Load("../assets/upgrade_bar_items.png");
    renderSystem = new RenderSystem();
    renderSystem->init(gameData);
    loadMap();
    std::initializer_list<std::tuple<int, std::string, Point, float>> sprites[]{
            {{MAP,   "map",         {SIDEBAR_WIDTH, 0}, 1}},
            {},
            {},
            {},
            {},
            {},
            {},
            {
             {OTHER, "upgrade_bar", {0,             0}, 1},
                    {OTHER, "menu", {MAP_WIDTH + SIDEBAR_WIDTH, 0}, 1},
                    {OTHER, "Cash", {MAP_WIDTH + SIDEBAR_WIDTH + 20, 9}, 0.6},
                    {OTHER, "Lives", {MAP_WIDTH + SIDEBAR_WIDTH + 20, 31}, 0.6},
                    {UPGRADES_BACKGROUND, "UpgradesBackground", {0, 0}, 1}
            },
    };
    for (int i = 0; i < N_LAYERS; i++) {
        for (auto[kind, surfaceName, position, scale] :sprites[i]) {
            auto spriteEntity = new Entity();
            auto[texture, surface]=gameData.getTexture(surfaceName);
            spriteEntity->addComponent<Visibility>(texture, surface,
                                                   SDL_Rect{int(position.X), int(position.Y), int(surface->w * scale)});
            if (kind != OTHER)
                spriteEntity->addComponent<Kind>(kind);
            layers[i].emplace_back(spriteEntity);
        }

    }
    // Buttons

    EntityP button(new Entity());
    auto[texture, surface]=gameData.getTexture("Play");
    Point position{SIDEBAR_WIDTH + MAP_WIDTH + 67, 480};
    button->addComponent<Visibility>(texture, surface, SDL_Rect{int(position.X - surface->w / 4),
                                                                int(position.Y - surface->h / 4),
                                                                int(surface->w / 2)});
    button->addComponent<Action>(CLICK);
    button->addComponent<Kind>(PLAY_FAST_FORWARD);
    layers[MENU_LAYER].emplace_back(button);
    gameData.playButton = button;

    auto nextStrategyButton = new Entity();
    std::tie(texture, surface) = gameData.getTexture("RightArrow");
    position = {116, 21};
    nextStrategyButton->addComponent<Visibility>(texture, surface, SDL_Rect{int(position.X - 7.5),
                                                                            int(position.Y - 7.5), 15});
    nextStrategyButton->addComponent<Action>(CLICK);
    nextStrategyButton->addComponent<Kind>(NEXT_STRATEGY);
    layers[MENU_LAYER].emplace_back(nextStrategyButton);

    auto previousStrategyButton = new Entity();
    std::tie(texture, surface) = gameData.getTexture("LeftArrow");
    position = {32, 21};
    previousStrategyButton->addComponent<Visibility>(texture, surface, SDL_Rect{int(position.X - 7.5),
                                                                                int(position.Y - 7.5), 15});
    previousStrategyButton->addComponent<Action>(CLICK);
    previousStrategyButton->addComponent<Kind>(PREVIOUS_STRATEGY);
    layers[MENU_LAYER].emplace_back(previousStrategyButton);

    for (int j = 0; j < 3; ++j) {
        auto upgradePath = new Entity();
        upgradePath->addComponent<Visibility>(texture, surface, SDL_Rect{0, 0, 0});
        upgradePath->addComponent<Action>(CLICK);
        upgradePath->addComponent<Kind>(UPGRADE_PATH_1 + j);
        layers[MENU_LAYER].emplace_back(upgradePath);
    }
    auto sellTowerButton = new Entity();
    sellTowerButton->addComponent<Action>(CLICK);
    sellTowerButton->addComponent<Kind>(SELL_TOWER);
    layers[MENU_LAYER].emplace_back(sellTowerButton);

    // Towers
    auto tower = new Entity();
    tower->addComponent<Kind>(SUPER_MONKEY);
    tower->addComponent<ShotKind>(DART);
    std::tie(texture, surface) = gameData.getTexture("SuperMonkey");
    position = {SIDEBAR_WIDTH + MAP_WIDTH + 65, 85};
    tower->addComponent<Visibility>(texture, surface,
                                    SDL_Rect{int(position.X - surface->w / 3), int(position.Y - surface->h / 3),
                                             int(surface->w / 1.5), 0});
    tower->addComponent<Action>(DRAG);
    tower->addComponent<Range>(150);
    tower->addComponent<AttackSpeed>(17.2);
    tower->addComponent<Pierce>(1);
    tower->addComponent<Damage>(1);
    tower->addComponent<Distance>(170);
    tower->addComponent<Type>(TOWER_T);
    tower->addComponent<Cost>(2975);
    auto &superMonkeyUpgrades = tower->addComponent<Upgrades>();
    std::string image = "SuperMonkey_LaserBlasts";
    std::tie(texture, surface) = gameData.getTexture(image);
    auto[lockedTexture, lockedSurface] = gameData.getTexture(image + "Locked");
    superMonkeyUpgrades.paths[0].emplace_back(
            Upgrade("Laser Blasts", 2295, surface, texture, lockedTexture, {{PIERCE_UPGRADE,    1},
                                                                            {SHOT_KIND_UPGRADE, LASER}}));
    image = "SuperMonkey_PlasmaVision";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    superMonkeyUpgrades.paths[0].emplace_back(
            Upgrade("Plasma Blasts", 4250, surface, texture, lockedTexture, {{PIERCE_UPGRADE,       1},
                                                                             {ATTACK_SPEED_UPGRADE, 2},
                                                                             {SHOT_KIND_UPGRADE,    PLASMA}}));
    image = "SuperMonkey_SunAvatar";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    superMonkeyUpgrades.paths[0].emplace_back(
            Upgrade("Sun Avatar", 17850, surface, texture, lockedTexture, {{SHOTS_AMOUNT_UPGRADE, 3},
                                                                           {PIERCE_UPGRADE,       3},
                                                                           {SHOT_KIND_UPGRADE,    SUN}}));

    image = "SuperMonkey_SuperRange";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    superMonkeyUpgrades.paths[1].emplace_back(
            Upgrade("Super Range", 850, surface, texture, lockedTexture,
                    {{RANGE_UPGRADE,    1.25},
                     {DISTANCE_UPGRADE, 1.25}}));
    image = "SuperMonkey_EpicRange";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    superMonkeyUpgrades.paths[1].emplace_back(
            Upgrade("Epic Range", 1190, surface, texture, lockedTexture, {{RANGE_UPGRADE,    1.25},
                                                                          {DISTANCE_UPGRADE, 1.25},
                                                                          {PIERCE_UPGRADE,   1}}));
    image = "SuperMonkey_RoboMonkey";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    superMonkeyUpgrades.paths[1].emplace_back(
            Upgrade("Robo Monkey", 7650, surface, texture, lockedTexture, {{SHOTS_AMOUNT_UPGRADE, 2},
                                                                           {DAMAGE_UPGRADE,       4}}));

    image = "SuperMonkey_Ultravision";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    superMonkeyUpgrades.paths[2].emplace_back(
            Upgrade("Ultravision", 1020, surface, texture, lockedTexture, {{RANGE_UPGRADE,    1.1},
                                                                           {DISTANCE_UPGRADE, 1.1},
                                                                           {CAMO_UPGRADE,     0}}));
    superMonkeyUpgrades.selectedPathUpgrades[0] = 1;
    superMonkeyUpgrades.selectedPathUpgrades[1] = 1;
    superMonkeyUpgrades.selectedPathUpgrades[2] = 0;
    layers[MENU_LAYER].emplace_back(tower);


    tower = new Entity();
    position = {SIDEBAR_WIDTH + MAP_WIDTH + 190, 85};
    tower->addComponent<Kind>(SNIPER_MONKEY);
    tower->addComponent<ShotKind>(BULLET);
    std::tie(texture, surface) = gameData.getTexture("SniperMonkey");
    tower->addComponent<Visibility>(texture, surface,
                                    SDL_Rect{int(position.X - surface->w / 3), int(position.Y - surface->h / 3),
                                             int(surface->w / 1.5), 0});
    tower->addComponent<Action>(DRAG);
    tower->addComponent<Range>(500);
    tower->addComponent<AttackSpeed>(0.45);
    tower->addComponent<Pierce>(1);
    tower->addComponent<Damage>(1);
    tower->addComponent<Distance>(500);
    tower->addComponent<Cost>(300);
    tower->addComponent<Type>(TOWER_T);
    auto &sniperMonkeyUpgrades = tower->addComponent<Upgrades>();
    image = "SniperMonkey_FullMetalJacket";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    sniperMonkeyUpgrades.paths[0].emplace_back(
            Upgrade("Full Metal Jacket", 295, surface, texture, lockedTexture, {{DAMAGE_UPGRADE,    3},
                                                                                {SHOT_KIND_UPGRADE, ENHANCED_BULLET}}));
    image = "SniperMonkey_LargeCalibre";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    sniperMonkeyUpgrades.paths[0].emplace_back(
            Upgrade("Large Calibre", 1275, surface, texture, lockedTexture, {{DAMAGE_UPGRADE, 3}}));
    image = "SniperMonkey_DeadlyPrecision";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    sniperMonkeyUpgrades.paths[0].emplace_back(
            Upgrade("Deadly Precision", 2250, surface, texture, lockedTexture, {{DAMAGE_UPGRADE, 11}}));

    image = "SniperMonkey_NightVisionGoggles";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    sniperMonkeyUpgrades.paths[1].emplace_back(
            Upgrade("Night Vision Goggles", 225, surface, texture, lockedTexture, {{CAMO_UPGRADE, 0}}));

    image = "SniperMonkey_FasterFiring";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    sniperMonkeyUpgrades.paths[2].emplace_back(
            Upgrade("Faster Firing", 340, surface, texture, lockedTexture, {{ATTACK_SPEED_UPGRADE, 1.54}}));

    image = "SniperMonkey_EvenFasterFiring";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    sniperMonkeyUpgrades.paths[2].emplace_back(
            Upgrade("Even Faster Firing", 340, surface, texture, lockedTexture, {{ATTACK_SPEED_UPGRADE, 1.66}}));
    image = "SniperMonkey_Semi-Automatic";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    sniperMonkeyUpgrades.paths[2].emplace_back(
            Upgrade("Semi-Automatic", 2975, surface, texture, lockedTexture, {{ATTACK_SPEED_UPGRADE, 3}}));
    image = "SniperMonkey_FullAutoRifle";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    sniperMonkeyUpgrades.paths[2].emplace_back(
            Upgrade("Full Auto Rifle", 4035, surface, texture, lockedTexture, {{ATTACK_SPEED_UPGRADE, 2},
                                                                               {SHOT_KIND_UPGRADE,    ENHANCED_BULLET}}));
    sniperMonkeyUpgrades.selectedPathUpgrades[0] = 3;
    sniperMonkeyUpgrades.selectedPathUpgrades[1] = 1;
    sniperMonkeyUpgrades.selectedPathUpgrades[2] = 2;
    layers[MENU_LAYER].emplace_back(tower);

    tower = new Entity();
    position = {SIDEBAR_WIDTH + MAP_WIDTH + 65, 141};
    tower->addComponent<Kind>(DART_MONKEY);
    tower->addComponent<ShotKind>(DART);
    std::tie(texture, surface) = gameData.getTexture("DartMonkey");
    tower->addComponent<Visibility>(texture, surface,
                                    SDL_Rect{int(position.X - surface->w / 3), int(position.Y - surface->h / 3),
                                             int(surface->w / 1.5), 0});
    tower->addComponent<Action>(DRAG);
    tower->addComponent<Range>(100);
    tower->addComponent<AttackSpeed>(0.95);
    tower->addComponent<Pierce>(1);
    tower->addComponent<Damage>(1);
    tower->addComponent<Distance>(150);
    tower->addComponent<Cost>(170);
    tower->addComponent<Type>(TOWER_T);
    auto &upgrades = tower->addComponent<Upgrades>();
    image = "DartMonkey_SharpShots";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    upgrades.paths[0].emplace_back(
            Upgrade("Sharp Shots", 120, surface, texture, lockedTexture, {{PIERCE_UPGRADE, 1}}));
    image = "DartMonkey_EnhancedEyesight_RazorSharpShots";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    upgrades.paths[0].emplace_back(
            Upgrade("Razor Sharp Shots", 185, surface, texture, lockedTexture, {{PIERCE_UPGRADE, 2}}));
    image = "DartMonkey_Spike-O-Pult";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    upgrades.paths[0].emplace_back(
            Upgrade("Spike-O-Pult", 225, surface, texture, lockedTexture, {{DISTANCE_UPGRADE,     6},
                                                                           {ATTACK_SPEED_UPGRADE, 0.76},
                                                                           {SHOT_KIND_UPGRADE,    SPIKE},
                                                                           {PIERCE_UPGRADE,       14}}));
    image = "DartMonkey_Juggernaut";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    upgrades.paths[0].emplace_back(
            Upgrade("Juggernaut", 1530, surface, texture, lockedTexture,
                    {{RANGE_UPGRADE,        1.25},
                     {ATTACK_SPEED_UPGRADE, 1.25},
                     {SHOT_KIND_UPGRADE,    JUGGERNAUT},
                     {PIERCE_UPGRADE,       86}}));

    image = "DartMonkey_LongRangeDarts_QuickShots";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    upgrades.paths[1].emplace_back(
            Upgrade("Quick Shots", 85, surface, texture, lockedTexture, {{ATTACK_SPEED_UPGRADE, 1.15}}));
    image = "DartMonkey_VeryQuickShots";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    upgrades.paths[1].emplace_back(
            Upgrade("Very Quick Shots", 160, surface, texture, lockedTexture, {{ATTACK_SPEED_UPGRADE, 1.33}}));
    image = "DartMonkey_TripleDarts";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    upgrades.paths[1].emplace_back(
            Upgrade("Triple Shot", 340, surface, texture, lockedTexture, {{SHOTS_AMOUNT_UPGRADE, 3}}));

    image = "DartMonkey_LongRangeDarts_QuickShots";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    upgrades.paths[2].emplace_back(
            Upgrade("Long Range Darts", 75, surface, texture, lockedTexture, {{DISTANCE_UPGRADE, 1.25},
                                                                              {RANGE_UPGRADE,    1.25}}));
    image = "DartMonkey_EnhancedEyesight_RazorSharpShots";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    upgrades.paths[2].emplace_back(
            Upgrade("Enhanced Eyesight", 170, surface, texture, lockedTexture, {{DISTANCE_UPGRADE, 1.2},
                                                                                {RANGE_UPGRADE,    1.2},
                                                                                {CAMO_UPGRADE,     0}}));
    image = "DartMonkey_Crossbow";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    upgrades.paths[2].emplace_back(
            Upgrade("Crossbow", 530, surface, texture, lockedTexture, {{DISTANCE_UPGRADE, 1.16},
                                                                       {RANGE_UPGRADE,    1.16},
                                                                       {PIERCE_UPGRADE,   1},
                                                                       {DAMAGE_UPGRADE,   2}}));

    upgrades.selectedPathUpgrades[0] = 2;
    upgrades.selectedPathUpgrades[1] = 1;
    upgrades.selectedPathUpgrades[2] = 1;
    layers[MENU_LAYER].emplace_back(tower);


    tower = new Entity();
    position = {SIDEBAR_WIDTH + MAP_WIDTH + 65, 197};
    tower->addComponent<Kind>(BOMB_TOWER);
    tower->addComponent<ShotKind>(BOMB);
    std::tie(texture, surface) = gameData.getTexture("BombTower");
    tower->addComponent<Visibility>(texture, surface,
                                    SDL_Rect{int(position.X - surface->w / 3), int(position.Y - surface->h / 3),
                                             int(surface->w / 1.5), 0});
    tower->addComponent<Action>(DRAG);
    tower->addComponent<Range>(100);
    tower->addComponent<AttackSpeed>(0.63);
    tower->addComponent<Pierce>(18);
    tower->addComponent<Damage>(1);
    tower->addComponent<Distance>(120);
    tower->addComponent<Cost>(555);
    tower->addComponent<Spread>(20);
    tower->addComponent<Type>(TOWER_T);
    auto &bombShooterUpgrades = tower->addComponent<Upgrades>();
    image = "BombShooter_BiggerBombs";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    bombShooterUpgrades.paths[0].emplace_back(
            Upgrade("Bigger Bombs", 340, surface, texture, lockedTexture, {{PIERCE_UPGRADE, 10},
                                                                           {SPREAD_UPGRADE, 1.25}}));
    image = "BombShooter_HeavyBombs";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    bombShooterUpgrades.paths[0].emplace_back(
            Upgrade("Heavy Bombs", 680, surface, texture, lockedTexture, {{DAMAGE_UPGRADE, 1}}));
    image = "BombShooter_ReallyBigBombs";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    bombShooterUpgrades.paths[0].emplace_back(
            Upgrade("Really Big Bombs", 1020, surface, texture, lockedTexture, {{PIERCE_UPGRADE, 20},
                                                                                {SPREAD_UPGRADE, 1.5}}));
    image = "BombShooter_BloonImpact";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    bombShooterUpgrades.paths[0].emplace_back(
            Upgrade("Bloon Impact", 2720, surface, texture, lockedTexture, {{DAMAGE_UPGRADE,    1},
                                                                            {SHOT_KIND_UPGRADE, ENHANCED_BOMB},
                                                                            {ADD_GOO_UPGRADE,   STUN}}));
    image = "BombShooter_BloonCrush";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    bombShooterUpgrades.paths[0].emplace_back(
            Upgrade("Bloon Crush", 46750, surface, texture, lockedTexture, {{SPREAD_UPGRADE,               1.5},
                                                                            {MOAB_CLASS_AFFECTING_UPGRADE, 0},
                                                                            {GOO_DURATION_UPGRADE,         120},
                                                                            {DAMAGE_UPGRADE,               9}}));

    image = "BombShooter_FasterReload";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    bombShooterUpgrades.paths[1].emplace_back(
            Upgrade("Faster Reload", 210, surface, texture, lockedTexture, {{ATTACK_SPEED_UPGRADE, 1.2}}));
    image = "BombShooter_MissileLauncher";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    bombShooterUpgrades.paths[1].emplace_back(
            Upgrade("Missile Launcher", 340, surface, texture, lockedTexture, {{SHOT_KIND_UPGRADE, MISSILE},
                                                                               {RANGE_UPGRADE,     1.15},
                                                                               {DISTANCE_UPGRADE,  1.15}}));
    image = "BombShooter_MOABMauler";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    bombShooterUpgrades.paths[1].emplace_back(
            Upgrade("MOAB Mauler", 765, surface, texture, lockedTexture, {{SHOT_KIND_UPGRADE, MOAB_MAULER}}));
    image = "BombShooter_MOABAssassin";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    bombShooterUpgrades.paths[1].emplace_back(
            Upgrade("MOAB Assassin", 2720, surface, texture, lockedTexture, {{SHOT_KIND_UPGRADE, MOAB_ASSASSIN}}));
    image = "BombShooter_MOABEliminator";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    bombShooterUpgrades.paths[1].emplace_back(
            Upgrade("MOAB Eliminator", 21250, surface, texture, lockedTexture, {{SHOT_KIND_UPGRADE, MOAB_ELIMINATOR}}));

    image = "BombShooter_ExtraRange";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    bombShooterUpgrades.paths[2].emplace_back(
            Upgrade("Extra Range", 170, surface, texture, lockedTexture, {{RANGE_UPGRADE,    1.5},
                                                                          {DISTANCE_UPGRADE, 1.5}}));
    bombShooterUpgrades.selectedPathUpgrades[0] = 2;
    bombShooterUpgrades.selectedPathUpgrades[1] = 4;
    bombShooterUpgrades.selectedPathUpgrades[2] = 0;
    layers[MENU_LAYER].emplace_back(tower);


    tower = new Entity();
    position = {SIDEBAR_WIDTH + MAP_WIDTH + 190, 197};
    tower->addComponent<Kind>(GLUE_GUNNER);
    tower->addComponent<ShotKind>(GOO_SHOT);
    std::tie(texture, surface) = gameData.getTexture("GlueGunner");
    tower->addComponent<Visibility>(texture, surface,
                                    SDL_Rect{int(position.X - surface->w / 3), int(position.Y - surface->h / 3),
                                             int(surface->w / 1.5), 0});
    tower->addComponent<Action>(DRAG);
    tower->addComponent<Range>(150);
    tower->addComponent<AttackSpeed>(0.97);
    tower->addComponent<Pierce>(1);
    tower->addComponent<Damage>(0);
    tower->addComponent<Distance>(170);
    tower->addComponent<Cost>(230);
    tower->addComponent<Spread>(70);
    tower->addComponent<Type>(TOWER_T);
    tower->addComponent<Goo>(GUM, 120);
    auto &upgrades2 = tower->addComponent<Upgrades>();
    image = "GlueGunner_GlueSoak_BiggerGlobs";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    upgrades2.paths[0].emplace_back(
            Upgrade("Glue Soak", 170, surface, texture, lockedTexture, {{GOO_SOAK_UPGRADE, 0}}));
    image = "GlueGunner_CorrosiveGlue";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    upgrades2.paths[0].emplace_back(
            Upgrade("Corrosive Glue", 225, surface, texture, lockedTexture, {{GOO_KIND_UPGRADE, CORROSIVE}}));
    image = "GlueGunner_BloonDissolver";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    upgrades2.paths[0].emplace_back(
            Upgrade("Bloon Dissolver", 2805, surface, texture, lockedTexture, {{DAMAGE_UPGRADE,             1},
                                                                               {CORROSIVE_INTERVAL_UPGRADE, 60}}));
    image = "GlueGunner_BloonLiquefier";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    upgrades2.paths[0].emplace_back(
            Upgrade("Bloon Liquefier", 5950, surface, texture, lockedTexture,
                    {{CORROSIVE_INTERVAL_UPGRADE, 60 / 10.0}}));

    image = "GlueGunner_GlueSoak_BiggerGlobs";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    upgrades2.paths[1].emplace_back(
            Upgrade("Bigger Globs", 85, surface, texture, lockedTexture, {{PIERCE_UPGRADE, 1}}));
    image = "GlueGunner_GlueSplatter";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    upgrades2.paths[1].emplace_back(
            Upgrade("Glue Splatter", 1530, surface, texture, lockedTexture, {{PIERCE_UPGRADE, 4}}));
    image = "GlueGunner_GlueHose";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    upgrades2.paths[1].emplace_back(
            Upgrade("Glue Hose", 2760, surface, texture, lockedTexture, {{ATTACK_SPEED_UPGRADE, 3}}));

    image = "GlueGunner_StickierGlue";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    upgrades2.paths[2].emplace_back(
            Upgrade("Stickier Glue", 100, surface, texture, lockedTexture, {{GOO_DURATION_UPGRADE, 240}}));
    image = "GlueGunner_StrongerGlue";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    upgrades2.paths[2].emplace_back(
            Upgrade("Stronger Glue", 340, surface, texture, lockedTexture, {{GOO_STICKINESS_UPGRADE, 0.3}}));
    image = "GlueGunner_SuperGlue";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    upgrades2.paths[2].emplace_back(
            Upgrade("Super Glue", 2720, surface, texture, lockedTexture, {{GOO_KIND_UPGRADE, GLUE}}));

    upgrades2.selectedPathUpgrades[0] = 3;
    upgrades2.selectedPathUpgrades[1] = 2;
    upgrades2.selectedPathUpgrades[2] = 1;
    layers[MENU_LAYER].emplace_back(tower);

    tower = new Entity();
    position = {SIDEBAR_WIDTH + MAP_WIDTH + 190, 141};
    tower->addComponent<Kind>(TACK_SHOOTER);
    tower->addComponent<ShotKind>(TACK);
    std::tie(texture, surface) = gameData.getTexture("TackShooter");
    tower->addComponent<Visibility>(texture, surface,
                                    SDL_Rect{int(position.X - surface->w / 3), int(position.Y - surface->h / 3),
                                             int(surface->w / 1.5), 0});
    tower->addComponent<Action>(DRAG);
    tower->addComponent<Range>(90);
    tower->addComponent<AttackSpeed>(0.6);
    tower->addComponent<ShotsAmount>(8);
    tower->addComponent<Pierce>(1);
    tower->addComponent<Damage>(1);
    tower->addComponent<Distance>(100);
    tower->addComponent<Cost>(305);
    tower->addComponent<Type>(TOWER_T);
    auto &tackShooterUpgrades = tower->addComponent<Upgrades>();
    image = "TackShooter_FasterShooting";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    tackShooterUpgrades.paths[0].emplace_back(
            Upgrade("Faster Shooting", 125, surface, texture, lockedTexture, {{ATTACK_SPEED_UPGRADE, 1.33}}));
    image = "TackShooter_EvenFasterShooting";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    tackShooterUpgrades.paths[0].emplace_back(
            Upgrade("Even Faster Shooting", 225, surface, texture, lockedTexture, {{ATTACK_SPEED_UPGRADE, 2}}));
    image = "TackShooter_HotShots";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    tackShooterUpgrades.paths[0].emplace_back(
            Upgrade("Hot Shots", 510, surface, texture, lockedTexture, {{SHOT_KIND_UPGRADE, HOT_TACK},
                                                                        {DAMAGE_UPGRADE,    1}}));

    image = "TackShooter_LongRangeTacks";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    tackShooterUpgrades.paths[1].emplace_back(
            Upgrade("Long Range Tacks", 85, surface, texture, lockedTexture, {{RANGE_UPGRADE,    1.15},
                                                                              {DISTANCE_UPGRADE, 1.15}}));
    image = "TackShooter_SuperRangeTacks";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    tackShooterUpgrades.paths[1].emplace_back(
            Upgrade("Super Range Tacks", 190, surface, texture, lockedTexture, {{RANGE_UPGRADE,    1.25},
                                                                                {DISTANCE_UPGRADE, 1.25}}));
    image = "TackShooter_BladeShooter";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    tackShooterUpgrades.paths[1].emplace_back(
            Upgrade("Blade Shooter", 465, surface, texture, lockedTexture, {{ATTACK_SPEED_UPGRADE, 1.18},
                                                                            {PIERCE_UPGRADE,       2},
                                                                            {DAMAGE_UPGRADE,       2},
                                                                            {SHOT_KIND_UPGRADE,    BLADE}}));
    image = "TackShooter_MoreTacks";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    tackShooterUpgrades.paths[2].emplace_back(
            Upgrade("More Tacks", 85, surface, texture, lockedTexture, {{SHOTS_AMOUNT_UPGRADE, 10}}));
    image = "TackShooter_EvenMoreTacks";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    tackShooterUpgrades.paths[2].emplace_back(
            Upgrade("Even More Tacks", 85, surface, texture, lockedTexture, {{SHOTS_AMOUNT_UPGRADE, 12}}));
    image = "TackShooter_TackSprayer";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    tackShooterUpgrades.paths[2].emplace_back(
            Upgrade("Tack Sprayer", 350, surface, texture, lockedTexture, {{ATTACK_SPEED_UPGRADE, 1.33},
                                                                           {SHOTS_AMOUNT_UPGRADE, 16}}));
    image = "TackShooter_Overdrive";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    tackShooterUpgrades.paths[2].emplace_back(
            Upgrade("Overdrive", 2125, surface, texture, lockedTexture, {{ATTACK_SPEED_UPGRADE, 3}}));
    image = "TackShooter_TheTackZone";
    std::tie(texture, surface) = gameData.getTexture(image);
    std::tie(lockedTexture, lockedSurface) = gameData.getTexture(image + "Locked");
    tackShooterUpgrades.paths[2].emplace_back(
            Upgrade("The Tack Zone", 17000, surface, texture, lockedTexture, {{ATTACK_SPEED_UPGRADE, 2},
                                                                              {SHOT_KIND_UPGRADE,    ENHANCED_TACK},
                                                                              {RANGE_UPGRADE,        2},
                                                                              {SHOTS_AMOUNT_UPGRADE, 32},
                                                                              {DISTANCE_UPGRADE,     2},
                                                                              {PIERCE_UPGRADE,       8}}));


    tackShooterUpgrades.selectedPathUpgrades[0] = 2;
    tackShooterUpgrades.selectedPathUpgrades[1] = 2;
    tackShooterUpgrades.selectedPathUpgrades[2] = 2;
    layers[MENU_LAYER].emplace_back(tower);


    // Systems
    systems.emplace_back(new LoadLevelSystem);
    systems.emplace_back(new BloonsSpawnSystem);
    systems.emplace_back(new ShotsSpawnSystem);
    systems.emplace_back(new EventSystem);
    systems.emplace_back(new DraggingSystem);
    systems.emplace_back(new CollisionSystem);
    systems.emplace_back(new DamageSystem);
    systems.emplace_back(new PopEffectSystem);
    systems.emplace_back(new MenuSystem);
    systems.emplace_back(new MovementSystem);
    systems.emplace_back(new RemoveEntitiesSystem);
    systems.emplace_back(renderSystem);

}

Game::~Game() {
    SDL_Quit();
}


void Game::update() {
    for (auto &system : systems) {
        system->update(layers.begin(), gameData);
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


