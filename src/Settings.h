//
// Created by Ido Mozes on 09/07/2019.
//

#ifndef SDL_GAME_SETTINGS_H
#define SDL_GAME_SETTINGS_H
enum Layers {
    BACKGROUND_LAYER, SEQUENCES_LAYER, SHADOW_LAYER, SHOTS_LAYER, TOWERS_LAYER, BLOONS_LAYER,POP_LAYER, MENU_LAYER,
    N_LAYERS
};
enum fonts{
    WHITE12,BLACK12,RED12,
    WHITE8,BLACK8,RED8,
    FONTS_LENGTH
};
constexpr int SIDEBAR_WIDTH = 150;
constexpr int MENU_WIDTH = 250;
constexpr int MAP_WIDTH = 686;
constexpr int MAP_HEIGHT = 511;
constexpr float BPS = 1 / 3.0;
#endif //SDL_GAME_SETTINGS_H
