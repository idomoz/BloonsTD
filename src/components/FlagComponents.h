//
// Created by Ido Mozes on 15/07/2019.
//

#ifndef SDL_GAME_FLAGCOMPONENTS_H
#define SDL_GAME_FLAGCOMPONENTS_H
#define FLAG_COMPONENT(className, classType)  class className : public FlagComponent { public:  static constexpr ComponentType type = ComponentType::classType;}

#include "../Component.h"

class FlagComponent : public Component {};

FLAG_COMPONENT(RemoveEntityEvent,REMOVE_ENTITY_EVENT);
FLAG_COMPONENT(Camo,CAMO);
FLAG_COMPONENT(Fortified,FORTIFIED);
FLAG_COMPONENT(Regrow,REGROW);
FLAG_COMPONENT(Glue,GLUE);
FLAG_COMPONENT(Gum,GUM);
FLAG_COMPONENT(Corrosive,CORROSIVE);
#endif //SDL_GAME_FLAGCOMPONENTS_H
