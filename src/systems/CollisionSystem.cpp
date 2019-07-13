//
// Created by Ido Mozes on 11/07/2019.
//
#include "CollisionSystem.h"

void insertionSort(Entities &layer) {
    // can be optimized with binary search instead of comparison to every item
    for (int i = 1, j = 0; i < layer.size(); j = i++) {
        auto &entity = layer[i];
        std::shared_ptr<Entity> entityP(entity);
        auto &previousEntity = layer[i - 1];
        auto[position, range] = entity->getComponents<Position, Range>().value();
        float X = position.value.X - range.value;

        while (j >= 0 and X < layer[j]->getComponent<Position>()->value.X - layer[j]->getComponent<Range>()->value) {
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
    std::vector<std::tuple<std::shared_ptr<Entity>, std::shared_ptr<Entity>>> collided;
    int bloonIndex = 0;
    for (auto &shot : shotsLayer) {
        auto[shotX, shotY] = shot->getComponent<Position>()->value;
        auto shotRange = shot->getComponent<Range>()->value;
        float shotLeft = shotX - shotRange;
        float shotRight = shotX + shotRange;
        float shotTop = shotY - shotRange;
        float shotBottom = shotY + shotRange;
        for (int i = bloonIndex; i < bloonsLayer.size(); ++i) {
            auto &bloon = bloonsLayer[i];
            auto[bloonX, bloonY] = bloon->getComponent<Position>()->value;
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
            if (shotBottom >= bloonTop and shotTop <= bloonBottom)
                collided.emplace_back(shot, bloon);
        }
    }
    for (auto[shot, bloon] :collided) {
        bloon->addComponent<RemoveEntityEvent>();
        shot->addComponent<RemoveEntityEvent>();
    }

}