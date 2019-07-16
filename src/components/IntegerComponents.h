//
// Created by Ido Mozes on 14/07/2019.
//

#ifndef SDL_GAME_INTEGERCOMPONENTS_H
#define SDL_GAME_INTEGERCOMPONENTS_H
#define INTEGER_COMPONENT(className, classType)  class className : public IntegerComponent { public:  static constexpr ComponentType type = ComponentType::classType; using IntegerComponent::IntegerComponent; }

#include "../Component.h"

class IntegerComponent : public Component {
public:
    int value;

    explicit IntegerComponent(int value) : value(value) {}
};

enum Types {
    OBSTACLE_T, TOWER_T, BLOON_T, SHOT_T, SEQUENCE_T
};
enum Strategies {
    FIRST, LAST, CLOSEST, STRONGEST
};
enum BloonKinds{
    RED_BLOON,
    BLUE_BLOON,
    GREEN_BLOON,
    YELLOW_BLOON,
    PINK_BLOON,
    PURPLE_BLOON,
    WHITE_BLOON,
    BLACK_BLOON,
    ZEBRA_BLOON,
    LEAD_BLOON,
    RAINBOW_BLOON,
    CERAMIC_BLOON,
    MOAB,
    BFB,
    DDT,
    ZOMG,
    BAD
};
enum TowerKinds{
    DART_MONKEY,
    SUPER_MONKEY,
    SNIPER_MONKEY,
};
enum ShotKinds{
    DART,
    BOMB,
    SPRAY,
    GUN,
};

INTEGER_COMPONENT(Lives,LIVES);
INTEGER_COMPONENT(Damage,DAMAGE);
INTEGER_COMPONENT(Pierce,PIERCE);
INTEGER_COMPONENT(Type,TYPE);
INTEGER_COMPONENT(Strategy,STRATEGY);
INTEGER_COMPONENT(Cost,COST);
INTEGER_COMPONENT(Value,VALUE);
INTEGER_COMPONENT(Kind,KIND);
INTEGER_COMPONENT(ShotKind,SHOT_KIND);


#endif //SDL_GAME_INTEGERCOMPONENTS_H