//
// Created by Ido Mozes on 20/06/2019.
//

#include "Entity.h"

Entity::Entity():componentsMask() ,components(){
    id = getNewId();
}

