//
// Created by Ido Mozes on 08/07/2019.
//

#include "EventSystem.h"


void EventSystem::update(Entities *layers, GameData &gameData) {

    SDL_PumpEvents();
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: {
                gameData.isRunning = false;
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {

                int mouseX, mouseY, originalMouseX;
                SDL_GetMouseState(&mouseX, &mouseY);
                mouseX = originalMouseX = int(mouseX / gameData.mapScale);
                mouseY = int(mouseY / gameData.mapScale);
                bool entityClicked = false;
                Entities newEntities[N_LAYERS];
                for (int i = N_LAYERS - 1; i >= 0; --i) {
                    for (auto &entity: layers[i]) {
                        if (auto components = entity->getComponents<Action, Visibility>()) {
                            auto[action, visibility] = components.value();


                            int entityX, entityY, w, h;
                            SDL_Rect *dstRect = visibility.getDstRect();
                            entityX = dstRect->x;
                            entityY = dstRect->y;
                            w = dstRect->w;
                            h = dstRect->h;
                            auto buttonKindP = entity->getComponent<Kind>();
                            if (action.actionType == CLICK and buttonKindP->value >= UPGRADE_PATH_1 and
                                buttonKindP->value <= UPGRADE_PATH_3) {
                                entityX = 4;
                                entityY = 63 + 135 * (buttonKindP->value - UPGRADE_PATH_1);
                                w = 138;
                                h = 81;
                            }

                            if (auto positionP = entity->getComponent<Position>()) {
                                auto &position = *positionP;
                                entityX = int(position.value.X - w / 2.0);
                                entityY = int(position.value.Y - h / 2.0);
                                mouseX = originalMouseX - SIDEBAR_WIDTH;
                            } else
                                mouseX = originalMouseX;

                            if (entityX <= mouseX and mouseX <= entityX + w and entityY <= mouseY and
                                mouseY <= entityY + h) {
                                if (action.disabled or (action.actionType != DROP and gameData.isDragging)) {
                                    entityClicked = true;
                                    continue;
                                }
                                switch (action.actionType) {
                                    case DRAG: {
                                        auto[type, kind, shotKind, range, attackInterval, pierce, damage, distance, cost, upgrades] =
                                        entity->getComponents<Type, Kind, ShotKind, Range, AttackSpeed, Pierce, Damage, Distance, Cost, Upgrades>().value();
                                        auto draggable = new Entity();
                                        SDL_Surface *surface = gameData.assets[getSurfaceName(entity)];
                                        draggable->addComponent<Visibility>(gameData.renderer, surface, SDL_Rect{
                                                originalMouseX - int(surface->w / 3.0), mouseY - int(surface->h / 3.0),
                                                int(surface->w / 1.5), int(surface->h / 1.5)});
                                        draggable->addComponent<Draggable>();
                                        draggable->addComponent<Action>(DROP);
                                        draggable->addComponents(type, kind, shotKind, range, attackInterval,
                                                                 pierce, damage, distance, cost, upgrades);
                                        draggable->addComponent<Strategy>(FIRST);
                                        if (auto spreadP = entity->getComponent<Spread>())
                                            draggable->addComponent<Spread>(*spreadP);
                                        if (auto gooP = entity->getComponent<Goo>())
                                            draggable->addComponent<Goo>(*gooP);
                                        if (entity->getComponent<Camo>())
                                            draggable->addComponent<Camo>();
                                        EntityP ptr(draggable);
                                        gameData.selected = ptr;
                                        auto rangeShadow = new Entity();
                                        rangeShadow->addComponent<RangeShadow>(ptr);
                                        newEntities[SHADOW_LAYER].emplace_back(rangeShadow);
                                        newEntities[MENU_LAYER].emplace_back(ptr);
                                        gameData.isDragging = true;
                                        goto entityClicked;
                                    }

                                    case CLICK: {
                                        if (entity->getComponent<Visibility>()->hidden)
                                            break;
                                        switch (buttonKindP->value) {
                                            case PLAY_FAST_FORWARD: {
                                                if (gameData.levelRunning)
                                                    gameData.FPS = 240 - gameData.FPS;
                                                else
                                                    gameData.levelRunning = true;
                                                visibility.loadTexture(gameData.renderer,
                                                                       gameData.assets[std::string("FastForward") +
                                                                                       (gameData.FPS == 180 ? "Enabled"
                                                                                                            : "")]);
                                                break;
                                            }
                                            case NEXT_STRATEGY: {
                                                auto &strategy = gameData.selected->getComponent<Strategy>()->value;
                                                strategy = (strategy + 1) % 4;
                                                break;
                                            }
                                            case PREVIOUS_STRATEGY: {
                                                auto &strategy = gameData.selected->getComponent<Strategy>()->value;
                                                strategy -= 1;
                                                if (strategy == -1)
                                                    strategy = 3;
                                                break;
                                            }
                                            case UPGRADE_PATH_1:
                                            case UPGRADE_PATH_2:
                                            case UPGRADE_PATH_3: {
                                                auto &upgradeP = *entity->getComponent<UpgradeP>();
                                                auto &upgrades = *gameData.selected->getComponent<Upgrades>();
                                                int cost = upgradeP.value->cost;
                                                if (cost > gameData.cash or upgradeP.value->locked)
                                                    break;
                                                else {
                                                    gameData.cash -= cost;
                                                    gameData.selected->getComponent<Cost>()->value += cost;
                                                }
                                                int currentPath = buttonKindP->value - UPGRADE_PATH_1;
                                                if (upgrades.selectedPathUpgrades[currentPath] ==
                                                    upgrades.paths[currentPath].size()) {
                                                    upgrades.chosenPath = currentPath;
                                                    for (int j = 1; j <= 2; ++j) {
                                                        int nextPathIndex = (currentPath + j) % 3;
                                                        auto &nextPath = upgrades.paths[nextPathIndex];
                                                        if (upgrades.selectedPathUpgrades[nextPathIndex])
                                                            nextPath[nextPath.size() -
                                                                     upgrades.selectedPathUpgrades[nextPathIndex]].locked = true;
                                                    }
                                                }
                                                for (auto[kind, value]:upgradeP.value->improvements) {
                                                    switch (kind) {
                                                        case RANGE_UPGRADE:
                                                            gameData.selected->getComponent<Range>()->value *= value;
                                                            break;
                                                        case PIERCE_UPGRADE:
                                                            gameData.selected->getComponent<Pierce>()->value += value;
                                                            break;
                                                        case DAMAGE_UPGRADE:
                                                            gameData.selected->getComponent<Damage>()->value += value;
                                                            break;
                                                        case DISTANCE_UPGRADE:
                                                            gameData.selected->getComponent<Distance>()->value *= value;
                                                            break;
                                                        case ATTACK_SPEED_UPGRADE: {
                                                            auto &attackSpeed = *gameData.selected->getComponent<AttackSpeed>();
                                                            attackSpeed.interval /= value;
                                                            attackSpeed.timeToRecharge = 0;
                                                            break;
                                                        }
                                                        case CAMO_UPGRADE:
                                                            gameData.selected->addComponent<Camo>();
                                                            break;
                                                        case GOO_KIND_UPGRADE: {
                                                            auto &goo = *gameData.selected->getComponent<Goo>();
                                                            goo.kind = value;
                                                            goo.ttl = goo.kind == GLUE ? 60 : 120;
                                                            break;
                                                        }
                                                        case GOO_DURATION_UPGRADE: {
                                                            auto &goo = *gameData.selected->getComponent<Goo>();
                                                            goo.ttl = value;
                                                            break;
                                                        }
                                                        case CORROSIVE_INTERVAL_UPGRADE: {
                                                            auto &goo = *gameData.selected->getComponent<Goo>();
                                                            goo.interval = value;
                                                            goo.timetoRecharge = value;
                                                            break;
                                                        }
                                                        case CORROSIVE_DAMAGE_UPGRADE: {
                                                            auto &goo = *gameData.selected->getComponent<Goo>();
                                                            goo.damage = value;
                                                            break;
                                                        }
                                                        case GOO_SOAK_UPGRADE: {
                                                            auto &goo = *gameData.selected->getComponent<Goo>();
                                                            goo.soak = true;
                                                            break;
                                                        }
                                                        case GOO_STICKNESS_UPGRADE: {
                                                            auto &goo = *gameData.selected->getComponent<Goo>();
                                                            goo.stickness = value;
                                                            break;
                                                        }
                                                        case SHOTS_AMOUNT_UPGRADE:
                                                            gameData.selected->addComponent<ShotsAmount>(value);
                                                            break;
                                                        case SHOT_KIND_UPGRADE:
                                                            gameData.selected->getComponent<ShotKind>()->value = value;
                                                            break;
                                                    }
                                                }
                                                auto &path = gameData.selected->getComponent<Upgrades>()->paths[
                                                        buttonKindP->value - UPGRADE_PATH_1];
                                                path.erase(path.begin());
                                                break;
                                            }
                                        }
                                        goto entityClicked;
                                    }
                                    case DROP: {
                                        auto &draggable = *entity->getComponent<Draggable>();
                                        if (draggable.isPlaceable) {
                                            entity->removeComponent<Draggable>();
                                            entity->getComponent<Action>()->actionType = SELECT;
                                            gameData.isDragging = false;
                                            if (mouseX > SIDEBAR_WIDTH + MAP_WIDTH or mouseX < SIDEBAR_WIDTH) {
                                                entity->addComponent<RemoveEntityEvent>();
                                                gameData.selected.reset();
                                            } else {
                                                for (int x = std::max(mouseX - SIDEBAR_WIDTH - 20, 0);
                                                     x < std::min(mouseX - SIDEBAR_WIDTH + 21, MAP_WIDTH); ++x) {
                                                    for (int y = std::max(mouseY - 20, 0);
                                                         y < std::min(mouseY + 21, MAP_HEIGHT); ++y) {
                                                        if (gameData.mapData[x][y] == FREE)
                                                            gameData.mapData[x][y] = TOWER;
                                                    }
                                                }
                                                if (i == MENU_LAYER) {
                                                    gameData.cash -= entity->getComponent<Cost>()->value;
                                                    entity->addComponent<MoveEntityEvent>(TOWERS_LAYER);
                                                    auto &visibility = *entity->getComponent<Visibility>();
                                                    SDL_Rect *dstRect = entity->getComponent<Visibility>()->getDstRect();
                                                    entity->addComponent<Position>(
                                                            dstRect->x - SIDEBAR_WIDTH + dstRect->w / 2,
                                                            dstRect->y + dstRect->h / 2);
                                                }
                                            }

                                        }
                                        goto entityClicked;
                                    }
                                    case SELECT: {
                                        if (entity == gameData.selected)
                                            goto entityClicked;
                                        gameData.selected = entity;
                                        auto rangeShadow = new Entity();
                                        rangeShadow->addComponent<RangeShadow>(entity);
                                        newEntities[SHADOW_LAYER].emplace_back(rangeShadow);
                                        goto entityClicked;
                                    }
                                }
                            }
                        }
                    }
                }
                if (!entityClicked)
                    gameData.selected.reset();
                entityClicked:
                for (int i = 0; i < N_LAYERS; ++i) {
                    if (!newEntities[i].empty())
                        layers[i] += newEntities[i];
                }
                break;
            }
            default:
                break;
        }
    }

}
