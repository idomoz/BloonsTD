//
// Created by Ido Mozes on 18/07/2019.
//

#include "LoadLevelSystem.h"

void LoadLevelSystem::update(Entities *layers, GameData &gameData) {
    if (gameData.levelReady or gameData.level > gameData.finalLevel or gameData.lives == 0)
        return;
    if (gameData.level != 1)
        gameData.cash += 100 + gameData.level - 1;
    std::string fileName = "../assets/Levels/level" + std::to_string(gameData.level) + ".data";
    std::ifstream levelFile(fileName, std::ios::binary);
    Sequence_S sequenceS;
    while (levelFile.peek() != EOF) {
        levelFile.read((char *) &sequenceS, 19);
        auto s = new Entity();
        s->addComponent<Type>(SEQUENCE_T);
        s->addComponent<Sequence>(sequenceS.amount, sequenceS.bps, sequenceS.delay);
        s->addComponent<Kind>(sequenceS.kind);
        if (sequenceS.regrow)
            s->addComponent<Regrow>(sequenceS.kind);
        if (sequenceS.camo)
            s->addComponent<Camo>();
        if (sequenceS.fortified)
            s->addComponent<Fortified>();
        layers[SEQUENCES_LAYER].emplace_back(s);
    }
    gameData.levelReady = true;
}
