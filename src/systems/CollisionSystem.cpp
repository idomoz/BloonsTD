//
// Created by Ido Mozes on 11/07/2019.
//
#include "CollisionSystem.h"

inline float getLeft(EntityP &entity) {
    return entity->getComponent<Position>()->value.X - entity->getComponent<Range>()->value;
}

int binarySearch(Entities &layer, float left, int low, int high) {
    if (high <= low)
        return left > getLeft(layer[low]) ? (low + 1) : low;

    int mid = (low + high) / 2;

    if (left == getLeft(layer[mid]))
        return mid + 1;

    if (left > getLeft(layer[mid]))
        return binarySearch(layer, left, mid + 1, high);
    return binarySearch(layer, left, low, mid - 1);
}

int
binarySearch(std::vector<std::tuple<EntityP, EntityP, float>> collided, float distance,
             int low, int high) {
    if (high <= low)
        return distance > std::get<2>(collided[low]) ? (low + 1) : low;

    int mid = (low + high) / 2;

    if (distance == std::get<2>(collided[mid]))
        return mid + 1;

    if (distance > std::get<2>(collided[mid]))
        return binarySearch(collided, distance, mid + 1, high);
    return binarySearch(collided, distance, low, mid - 1);
}

void insertionSort(Entities &layer) {
    // can be optimized with binary search instead of comparison to every item
    for (int i = 1, j = 0; i < layer.size(); j = i++) {
        auto &entity = layer[i];
        EntityP entityP(entity);
        auto &previousEntity = layer[i - 1];
        auto[position, range] = entity->getComponents<Position, Range>().value();
        float entityLeft = position.value.X - range.value;
        if (entityLeft >= getLeft(layer[j]))
            continue;
        int loc = binarySearch(layer, entityLeft, 0, j);
        while (j >= loc) {
            layer[j + 1] = layer[j];
            --j;
        }
        if (j != i - 1) {
            layer[j + 1] = entityP;
        }
    }
}

void CollisionSystem::update(Entities *layers, GameData &gameData) {
    if (!gameData.levelRunning)
        return;
    Entities &bloonsLayer = layers[BLOONS_LAYER];
    Entities &shotsLayer = layers[SHOTS_LAYER];
    insertionSort(bloonsLayer);
    insertionSort(shotsLayer);
    std::vector<std::tuple<EntityP, EntityP, float>> collided;
    int bloonIndex = 0;
    for (auto &shot : shotsLayer) {
        int collidedIndex = collided.size();
        auto[shotPosition, poppedBloons] = shot->getComponents<Position, PoppedBloons>().value();
        auto[shotX, shotY] = shotPosition.value;
        auto shotRange = shot->getComponent<Range>()->value;
        float shotLeft = shotX - shotRange;
        float shotRight = shotX + shotRange;
        float shotTop = shotY - shotRange;
        float shotBottom = shotY + shotRange;
        for (int i = bloonIndex; i < bloonsLayer.size(); ++i) {
            auto &bloon = bloonsLayer[i];
            auto bloonPosition = bloon->getComponent<Position>()->value;
            auto[bloonX, bloonY] = bloonPosition;
            auto bloonRange = bloon->getComponent<Range>()->value;
            float bloonLeft = bloonX - bloonRange;
            float bloonRight = bloonX + bloonRange;
            float bloonTop = bloonY - bloonRange;
            float bloonBottom = bloonY + bloonRange;
            if (bloonRight < shotLeft) {
                bloonIndex++;
                continue;
            }
            if (shotRight < bloonLeft)
                break;
            if (shotBottom >= bloonTop and shotTop <= bloonBottom and !poppedBloons.value.count(bloon.get())) {
                poppedBloons.value.emplace(bloon.get());
                float distance = twoPointsDistance(shotPosition.value, bloonPosition);
                if (collidedIndex == collided.size())
                    collided.emplace_back(shot, bloon, distance);
                else {
                    int index = binarySearch(collided, distance, collidedIndex, collided.size() - 1);
                    if (index < collided.size())
                        collided.emplace(collided.begin() + index, shot, bloon, distance);
                    else
                        collided.emplace_back(shot, bloon, distance);
                }
            }

        }
    }
    for (auto[shot, bloon, _] :collided) {
        auto[pierce, damage, kind, position] = shot->getComponents<Pierce, Damage, Kind, Position>().value();
        if (bloon->getComponent<Camo>() and !shot->getComponent<Camo>())
            continue;
        int bloonKind = bloon->getComponent<Kind>()->value;
        if (pierce.value > 0) {
            switch (kind.value) {
                case LASER:
                    if (bloonKind != PURPLE_BLOON and bloonKind != LEAD_BLOON)
                        bloon->addComponent<DamageEvent>(damage.value, shot);
                    break;
                case PLASMA:
                    if (bloonKind != PURPLE_BLOON)
                        bloon->addComponent<DamageEvent>(damage.value, shot);
                    break;
                case SUN:
                case HOT_TACK:
                case ENHANCED_TACK:
                    bloon->addComponent<DamageEvent>(damage.value, shot);
                    break;
                case DART:
                case TACK:
                case BLADE:
                case SPIKE:
                    if (bloonKind != LEAD_BLOON)
                        bloon->addComponent<DamageEvent>(damage.value, shot);
                    break;
                case JUGGERNAUT:
                    bloon->addComponent<DamageEvent>(bloonKind == CERAMIC_BLOON ? 5 : damage.value, shot);
                    break;
                case BOMB_EXPLOSION:
                case MISSILE_EXPLOSION:
                case ENHANCED_BOMB_EXPLOSION:
                case MOAB_MAULER_EXPLOSION:
                case MOAB_ASSASSIN_EXPLOSION:
                case MOAB_ELIMINATOR_EXPLOSION: {
                    bool breakFlag = false;
                    shot->addComponent<RemoveEntityEvent>();
                    switch (kind.value) {
                        case BOMB_EXPLOSION:
                        case MISSILE_EXPLOSION:
                        case MOAB_MAULER_EXPLOSION:
                            if (bloonKind == BLACK_BLOON or bloonKind == ZEBRA_BLOON)
                                breakFlag = true;
                            break;
                    }
                    if (breakFlag)
                        break;
                    auto shotGooP = shot->getComponent<Goo>();
                    if (shotGooP and (bloonKind < MOAB or shot->getComponent<MoabClassAffecting>()))
                        bloon->addComponent<Goo>(*shotGooP);
                    int _damage = damage.value;
                    if (bloonKind >= MOAB)
                        switch (kind.value) {
                            case MOAB_MAULER_EXPLOSION:
                                _damage += 15;
                                break;
                            case MOAB_ASSASSIN_EXPLOSION:
                                _damage += 20;
                                break;
                            case MOAB_ELIMINATOR_EXPLOSION:
                                _damage += 80;
                                break;
                        }
                    if (bloonKind == CERAMIC_BLOON)
                        switch (kind.value) {
                            case MOAB_ASSASSIN_EXPLOSION:
                            case MOAB_ELIMINATOR_EXPLOSION:
                                _damage = 24;
                                break;
                        }
                    bloon->addComponent<DamageEvent>(_damage, shot);
                    break;
                }
                case GOO_SPLASH: {
                    bloon->addComponent<DamageEvent>(damage.value, shot);
                    if (bloonKind < MOAB and !bloon->getComponent<Goo>()) {
                        bloon->addComponent<Goo>(*shot->getComponent<Goo>());
                        auto [texture,surface] = gameData.getTexture(getSurfaceName(bloon));
                        auto &visibility = *bloon->getComponent<Visibility>();
                        visibility.setDstRect(SDL_Rect{0, 0, surface->w / 3, 0});
                        visibility.reloadTexture(texture, surface);
                    }
                    shot->addComponent<RemoveEntityEvent>();
                    break;
                }
                case BOMB:
                case ENHANCED_BOMB:
                case MISSILE:
                case MOAB_MAULER:
                case MOAB_ASSASSIN:
                case MOAB_ELIMINATOR:
                case GOO_SHOT:
                    if (shot->getComponent<RemoveEntityEvent>())
                        break;
                    EntityP explosion(new Entity());
                    explosion->addComponent<Position>(position.value.X, position.value.Y);
                    explosion->addComponent<Type>(SHOT_T);
                    switch (kind.value) {
                        case BOMB:
                        case ENHANCED_BOMB:
                        case MISSILE:
                        case MOAB_MAULER:
                        case MOAB_ASSASSIN:
                        case MOAB_ELIMINATOR:
                            explosion->addComponent<Kind>(kind.value + 1);
                            if (auto shotGooP = shot->getComponent<Goo>())
                                explosion->addComponent<Goo>(*shotGooP);
                            break;
                        case GOO_SHOT:
                            explosion->addComponent<Kind>(GOO_SPLASH);
                            explosion->addComponent<Goo>(*shot->getComponent<Goo>());
                            break;
                    }
                    explosion->addComponent<Pierce>(pierce);
                    explosion->addComponent<PoppedBloons>();
                    explosion->addComponent<Range>(shot->getComponent<Spread>()->value);
                    explosion->addComponents(damage);
                    auto [texture,surface] = gameData.getTexture(getSurfaceName(explosion));
                    explosion->addComponent<Visibility>(texture, surface, SDL_Rect{0, 0, surface->w / 2});
                    layers[SHOTS_LAYER].emplace_back(explosion);
                    shot->addComponent<RemoveEntityEvent>();
                    break;
            }

            if (--pierce.value == 0)
                shot->addComponent<RemoveEntityEvent>();
        }
    }

}