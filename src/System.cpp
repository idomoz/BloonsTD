//
// Created by Ido Mozes on 15/07/2019.
//

#include "System.h"

std::string getSurfaceName(EntityP &entity) {
    std::string surfaceName;
    int damageStateLives = getBloonProperty<SELF_LIVES>(entity) / 5;
    switch (entity->getComponent<Type>()->value) {
        case BLOON_T: {
            auto[regrowP, camoP, fortifiedP, gooP, lives] = entity->getComponentsP<Regrow, Camo, Fortified, Goo, Lives>();
            switch (entity->getComponent<Kind>()->value) {
                case RED_BLOON:
                    surfaceName = "Red";
                    break;
                case BLUE_BLOON:
                    surfaceName = "Blue";
                    break;
                case GREEN_BLOON:
                    surfaceName = "Green";
                    break;
                case YELLOW_BLOON:
                    surfaceName = "Yellow";
                    break;
                case PINK_BLOON:
                    surfaceName = "Pink";
                    break;
                case PURPLE_BLOON:
                    surfaceName = "Purple";
                    break;
                case WHITE_BLOON:
                    surfaceName = "White";
                    break;
                case BLACK_BLOON:
                    surfaceName = "Black";
                    break;
                case ZEBRA_BLOON:
                    surfaceName = "Zebra";
                    break;
                case LEAD_BLOON:
                    surfaceName = "Lead";
                    break;
                case RAINBOW_BLOON:
                    surfaceName = "Rainbow";
                    break;
                case CERAMIC_BLOON:
                    surfaceName = "Ceramic";
                    break;
                case MOAB:
                    surfaceName = "Moab";
                    break;
                case BFB:
                    surfaceName = "Bfb";
                    break;
                case DDT:
                    surfaceName = "Ddt";
                    break;
                case ZOMG:
                    surfaceName = "Zomg";
                    break;
                case BAD:
                    surfaceName = "Bad";
                    break;
            }
            if (regrowP)
                surfaceName += "Regrow";
            if (camoP)
                surfaceName += "Camo";
            if (fortifiedP) {
                surfaceName += "Fortified";
            }
            if (damageStateLives) {
                int maxLives = getBloonProperty<TOTAL_LIVES>(entity);
                if (int damageState = (maxLives - lives->value) / damageStateLives)
                    surfaceName += "DamageState" + std::to_string(damageState);
            }
            if (gooP)
                switch (gooP->kind) {
                    case GLUE:
                        surfaceName += "Gum";
                        break;
                    case GUM:
                        surfaceName += "Glue";
                        break;
                    case CORROSIVE:
                        surfaceName += "Slime";
                        break;
                }
            break;
        }
        case TOWER_T: {
            switch (entity->getComponent<Kind>()->value) {
                case DART_MONKEY:
                    surfaceName = "DartMonkey";
                    break;
                case TACK_SHOOTER:
                    surfaceName = "TackShooter";
                    break;
                case SUPER_MONKEY:
                    surfaceName = "SuperMonkey";
                    break;
                case SNIPER_MONKEY:
                    surfaceName = "SniperMonkey";
                    break;
                case BOMB_TOWER:
                    surfaceName = "BombTower";
                    break;
                case GLUE_GUNNER:
                    surfaceName = "GlueGunner";
                    break;
            }
            break;
        }
        case SHOT_T: {
            switch (entity->getComponent<Kind>()->value) {
                case SPIKE:
                    surfaceName = "Spike";
                    break;
                case JUGGERNAUT:
                    surfaceName = "Juggernaut";
                    break;
                case LASER:
                    surfaceName = "Laser";
                    break;
                case PLASMA:
                    surfaceName = "Plasma";
                    break;
                case SUN:
                    surfaceName = "Sun";
                    break;
                case DART:
                case RADIAL_DART:
                    surfaceName = "Dart";
                    break;
                case BOMB:
                    surfaceName = "Bomb";
                    break;
                case EXPLOSION:
                    surfaceName = "Explosion";
                    break;
                case GOO_SPLASH:
                    switch (entity->getComponent<Goo>()->kind) {
                        case GLUE:
                            surfaceName = "SplashGum";
                            break;
                        case GUM:
                            surfaceName = "SplashGlue";
                            break;
                        case CORROSIVE:
                            surfaceName = "SplashSlime";
                            break;
                    }
                    break;
                case GOO_SHOT:
                    switch (entity->getComponent<Goo>()->kind) {
                        case GLUE:
                            surfaceName = "GumShot";
                            break;
                        case GUM:
                            surfaceName = "GlueShot";
                            break;
                        case CORROSIVE:
                            surfaceName = "SlimeShot";
                            break;
                    }
                    break;
            }
            break;
        }
    }
    return surfaceName;
}

float getSpeed(EntityP &entity) {
    float speed;
    switch (entity->getComponent<Type>()->value) {
        case BLOON_T: {
            auto gooP = entity->getComponent<Goo>();
            switch (entity->getComponent<Kind>()->value) {
                case RED_BLOON:
                    speed = 3 * BPS;
                    break;
                case BLUE_BLOON:
                    speed = 4 * BPS;
                    break;
                case GREEN_BLOON:
                    speed = 5 * BPS;
                    break;
                case YELLOW_BLOON:
                    speed = 10 * BPS;
                    break;
                case PINK_BLOON:
                    speed = 11 * BPS;
                    break;
                case PURPLE_BLOON:
                    speed = 9.5 * BPS;
                    break;
                case WHITE_BLOON:
                    speed = 6 * BPS;
                    break;
                case BLACK_BLOON:
                    speed = 5 * BPS;
                    break;
                case ZEBRA_BLOON:
                    speed = 5 * BPS;
                    break;
                case LEAD_BLOON:
                    speed = 3 * BPS;
                    break;
                case RAINBOW_BLOON:
                    speed = 7 * BPS;
                    break;
                case CERAMIC_BLOON:
                    speed = 8 * BPS;
                    break;
                case MOAB:
                    speed = 3 * BPS;
                    break;
                case BFB:
                    speed = 1 * BPS;
                    break;
                case DDT:
                    speed = 9 * BPS;
                    break;
                case ZOMG:
                    speed = 0.5 * BPS;
                    break;
                case BAD:
                    speed = 0.2 * BPS;
                    break;
            }
            if (gooP)
                switch (gooP->kind) {
                    case CORROSIVE:
                    case GUM:
                        speed *= gooP->stickness;
                        break;
                    case GLUE:
                        speed = 0;
                }
            break;
        }
        case SHOT_T: {
            switch (entity->getComponent<Kind>()->value) {
                case DART:
                case RADIAL_DART:
                case SPIKE:
                case JUGGERNAUT:
                    speed = 12;
                    break;
                case LASER:
                case PLASMA:
                case SUN:
                    speed = 22;
                    break;
                case BOMB:
                case GOO_SHOT:
                    speed = 10;
                    break;
            }
            break;
        }
    }
    return speed;
}

