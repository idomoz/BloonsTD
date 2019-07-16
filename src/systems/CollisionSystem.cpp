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
        auto[pierce, damage] = shot->getComponents<Pierce, Damage>().value();
        if (pierce.value > 0) {
            bloon->addComponent<DamageEvent>(damage.value,shot);
            if (--pierce.value == 0)
                shot->addComponent<RemoveEntityEvent>();
        }
    }

}