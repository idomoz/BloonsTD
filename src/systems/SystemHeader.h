#include "../System.h"
class __ : public System {
public:
    void update(std::vector<std::shared_ptr<Entity>> *layers, GameData &gameData) override;
};