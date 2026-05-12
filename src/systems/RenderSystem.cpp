//
// Created by Ido Mozes on 23/06/2019.
//

#include "RenderSystem.h"
#include "../Assets.h"

namespace {
    // Filled disc via horizontal scanlines. Replaces SDL2_gfx filledCircleRGBA
    // so we don't need to drag SDL2_gfx to iOS.
    void drawFilledCircle(SDL_Renderer *r, int cx, int cy, int radius,
                          Uint8 cr, Uint8 cg, Uint8 cb, Uint8 ca) {
        SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(r, cr, cg, cb, ca);
        for (int dy = -radius; dy <= radius; ++dy) {
            int dx = (int) SDL_sqrt((double) (radius * radius - dy * dy));
            SDL_RenderDrawLine(r, cx - dx, cy + dy, cx + dx, cy + dy);
        }
    }

    // Midpoint circle outline. Not anti-aliased, but visually fine for the
    // range indicator at game resolution.
    void drawCircleOutline(SDL_Renderer *r, int cx, int cy, int radius,
                           Uint8 cr, Uint8 cg, Uint8 cb, Uint8 ca) {
        SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(r, cr, cg, cb, ca);
        int x = radius;
        int y = 0;
        int err = 1 - x;
        while (x >= y) {
            SDL_RenderDrawPoint(r, cx + x, cy + y);
            SDL_RenderDrawPoint(r, cx + y, cy + x);
            SDL_RenderDrawPoint(r, cx - y, cy + x);
            SDL_RenderDrawPoint(r, cx - x, cy + y);
            SDL_RenderDrawPoint(r, cx - x, cy - y);
            SDL_RenderDrawPoint(r, cx - y, cy - x);
            SDL_RenderDrawPoint(r, cx + y, cy - x);
            SDL_RenderDrawPoint(r, cx + x, cy - y);
            ++y;
            if (err < 0) {
                err += 2 * y + 1;
            } else {
                --x;
                err += 2 * (y - x) + 1;
            }
        }
    }
}

std::string formatCommas(int num) {
    std::string numWithCommas = std::to_string(num);
    int insertPosition = numWithCommas.length() - 3;
    while (insertPosition > 0) {
        numWithCommas.insert(insertPosition, ",");
        insertPosition -= 3;
    }
    return numWithCommas;
}

void RenderSystem::init(GameData &gameData) {
    if (gameData.window != nullptr)
        SDL_DestroyWindow(gameData.window);
    if (gameData.renderer != nullptr)
        SDL_DestroyRenderer(gameData.renderer);
#ifdef BLOONSTD_IOS
    // iOS: cover the entire device screen at native resolution. Use the
    // display's reported size rather than (0,0) so SDL knows to claim the
    // full screen even if the simulator launched in portrait while the
    // bundle requests landscape.
    gameData.mapScale = 1.0f;
    SDL_DisplayMode dm{};
    SDL_GetDesktopDisplayMode(0, &dm);
    SDL_Log("BloonsTD iOS display mode: %dx%d", dm.w, dm.h);
    int winW = dm.w > 0 ? dm.w : 1024;
    int winH = dm.h > 0 ? dm.h : 768;
    gameData.window = SDL_CreateWindow("BloonsTD", 0, 0, winW, winH,
                                       SDL_WINDOW_FULLSCREEN | SDL_WINDOW_ALLOW_HIGHDPI |
                                       SDL_WINDOW_BORDERLESS);
#else
    gameData.window = SDL_CreateWindow("BloonsTD", SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED, /* NOLINT(hicpp-signed-bitwise)*/
                                       int((MAP_WIDTH + SIDEBAR_WIDTH + MENU_WIDTH) * gameData.mapScale),
                                       int(MAP_HEIGHT * gameData.mapScale),
                                       gameData.fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
#endif
    gameData.renderer = SDL_CreateRenderer(gameData.window, -1, 0);
    SDL_SetRenderDrawColor(gameData.renderer, 255, 255, 255, 255);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
#ifdef BLOONSTD_IOS
    int outW = 0, outH = 0;
    SDL_GetRendererOutputSize(gameData.renderer, &outW, &outH);
    SDL_Log("BloonsTD iOS renderer output: %dx%d", outW, outH);
    // Scale the game's fixed logical canvas to the device screen, with
    // letterbox bars where the aspect ratios disagree. All game coordinates
    // continue to use the desktop-native pixel scheme.
    SDL_RenderSetLogicalSize(gameData.renderer,
                             MAP_WIDTH + SIDEBAR_WIDTH + MENU_WIDTH,
                             MAP_HEIGHT);
#endif

    std::string fontPath = assetPath("LuckiestGuy-Regular.ttf");
    gameData.fonts[WHITE12] = FC_CreateFont();
    FC_LoadFont(gameData.fonts[WHITE12], gameData.renderer, fontPath.c_str(), 12 * gameData.mapScale,
                FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
    gameData.fonts[BLACK12] = FC_CreateFont();
    FC_LoadFont(gameData.fonts[BLACK12], gameData.renderer, fontPath.c_str(), 12 * gameData.mapScale,
                FC_MakeColor(0, 0, 0, 255), TTF_STYLE_NORMAL);
    gameData.fonts[RED12] = FC_CreateFont();
    FC_LoadFont(gameData.fonts[RED12], gameData.renderer, fontPath.c_str(), 12 * gameData.mapScale,
                FC_MakeColor(255, 49, 49, 255), TTF_STYLE_NORMAL);
    gameData.fonts[WHITE8] = FC_CreateFont();
    FC_LoadFont(gameData.fonts[WHITE8], gameData.renderer, fontPath.c_str(), 10 * gameData.mapScale,
                FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
    gameData.fonts[BLACK8] = FC_CreateFont();
    FC_LoadFont(gameData.fonts[BLACK8], gameData.renderer, fontPath.c_str(), 10 * gameData.mapScale,
                FC_MakeColor(0, 0, 0, 255), TTF_STYLE_NORMAL);
    gameData.fonts[RED8] = FC_CreateFont();
    FC_LoadFont(gameData.fonts[RED8], gameData.renderer, fontPath.c_str(), 10 * gameData.mapScale,
                FC_MakeColor(255, 49, 49, 255), TTF_STYLE_NORMAL);

}

void RenderSystem::update(Entities *layers, GameData &gameData) {
    // SDL_RenderClear paints the entire physical surface with the current
    // draw colour — including the iOS letterbox bars outside the logical
    // area. Without this reset, those bars inherit whatever colour the
    // previous frame left set (e.g. red, after drawing an invalid range
    // circle), so the margins flash red along with the circle.
    SDL_SetRenderDrawColor(gameData.renderer, 0, 0, 0, 255);
    SDL_RenderClear(gameData.renderer);
    for (int i = 0; i < N_LAYERS; ++i) {
        if (i == SEQUENCES_LAYER)
            continue;

        for (auto &entity: layers[i]) {
            if (i == MENU_LAYER) {
                auto[visibilityP, kindP, actionP] = entity->getComponentsP<Visibility, Kind, Action>();
                if (visibilityP and visibilityP->hidden)
                    continue;
                if (kindP and actionP)
                    switch (actionP->actionType) {
                        case CLICK:
                            switch (kindP->value) {
                                case UPGRADE_PATH_1:
                                case UPGRADE_PATH_2:
                                case UPGRADE_PATH_3: {
                                    auto &upgradeP = *entity->getComponent<UpgradeP>();
                                    int path = kindP->value - UPGRADE_PATH_1;
                                    int cost = upgradeP.value->cost;
                                    FC_Draw(gameData.fonts[WHITE12], gameData.renderer, 25 * gameData.mapScale,
                                            (68 + path * 135) * gameData.mapScale, upgradeP.value->name.c_str());
                                    int font = WHITE8;
                                    if (cost > gameData.cash)
                                        font = RED8;
                                    FC_Draw(gameData.fonts[font], gameData.renderer, 47 * gameData.mapScale,
                                            (81 + path * 135) * gameData.mapScale, "$%s", formatCommas(cost).c_str());
                                    break;
                                }
                            }
                            break;
                        case DRAG: {
                            int cost = entity->getComponent<Cost>()->value;
                            int font = cost > gameData.cash ? RED8 : WHITE8;
                            FC_Draw(gameData.fonts[font], gameData.renderer,
                                    (visibilityP->getDstRect()->x +(visibilityP->getDstRect()->w/2)- 50) * gameData.mapScale,
                                    (visibilityP->getDstRect()->y+(visibilityP->getDstRect()->h/2) ) * gameData.mapScale, "$%s",
                                    formatCommas(cost).c_str());
                        }
                        break;
                    }
            }
            auto rangeShadowP = entity->getComponent<RangeShadow>();
            auto &currentEntity = rangeShadowP ? rangeShadowP->entity : entity;
            if (auto visibilityP = currentEntity->getComponent<Visibility>()) {
                auto &visibility = *visibilityP;
                SDL_Rect *dstRect = visibility.getDstRect();
                SDL_Rect newDstRect = {int(dstRect->x * gameData.mapScale), int(dstRect->y * gameData.mapScale),
                                       int(dstRect->w * gameData.mapScale), int(dstRect->h * gameData.mapScale)};
                SDL_Point entityCenter;

                auto positionP = currentEntity->getComponent<Position>();
                if (positionP) {
                    auto &position = *positionP;
                    entityCenter.x = (position.value.X + SIDEBAR_WIDTH) * gameData.mapScale;
                    entityCenter.y = position.value.Y * gameData.mapScale;
                    newDstRect.x = int((position.value.X + SIDEBAR_WIDTH) * gameData.mapScale - newDstRect.w / 2.0);
                    newDstRect.y = int(position.value.Y * gameData.mapScale - newDstRect.h / 2.0);
                } else {
                    entityCenter.x = int(dstRect->x * gameData.mapScale + (dstRect->w * gameData.mapScale) / 2.0);
                    entityCenter.y = int(dstRect->y * gameData.mapScale + (dstRect->h * gameData.mapScale) / 2.0);
                }

                if (currentEntity != entity) {
                    auto draggableP = currentEntity->getComponent<Draggable>();
                    bool isRed = draggableP ? !draggableP->isPlaceable : false;
                    float range = currentEntity->getComponent<Range>()->value;
                    // Clip the range disc + outline to the map rect so a circle
                    // near the map edge doesn't bleed onto the top/bottom UI
                    // bars (where it'd recolour them red on invalid placement).
                    SDL_Rect mapClip = {
                            int(SIDEBAR_WIDTH * gameData.mapScale),
                            0,
                            int(MAP_WIDTH * gameData.mapScale),
                            int(MAP_HEIGHT * gameData.mapScale)};
                    SDL_RenderSetClipRect(gameData.renderer, &mapClip);
                    drawFilledCircle(gameData.renderer, entityCenter.x, entityCenter.y,
                                     int(range * gameData.mapScale),
                                     isRed ? 255 : 0, 0, 0, 100);
                    drawCircleOutline(gameData.renderer, entityCenter.x, entityCenter.y,
                                      int(range * gameData.mapScale),
                                      isRed ? 255 : 0, 0, 0, 150);
                    SDL_RenderSetClipRect(gameData.renderer, nullptr);
                }
                if (entity == currentEntity) {
                    SDL_RenderCopyEx(gameData.renderer, visibility.getTexture(), nullptr, &newDstRect, visibility.angle,
                                     nullptr, SDL_FLIP_NONE);

                }
            }
        }
    }
    if (gameData.cash > 99999999)
        gameData.cash = 99999999;

    FC_Draw(gameData.fonts[WHITE12], gameData.renderer, (MAP_WIDTH + SIDEBAR_WIDTH + 45) * gameData.mapScale,
            14 * gameData.mapScale,
            formatCommas(gameData.cash).c_str());
    FC_Draw(gameData.fonts[WHITE12], gameData.renderer, (MAP_WIDTH + SIDEBAR_WIDTH + 45) * gameData.mapScale,
            36 * gameData.mapScale,
            std::to_string(gameData.lives).c_str());
    FC_Draw(gameData.fonts[WHITE12], gameData.renderer, (MAP_WIDTH + SIDEBAR_WIDTH + 160) * gameData.mapScale,
            22 * gameData.mapScale,
            "Level: %s", std::to_string(gameData.level).c_str());
    if (gameData.selected and !gameData.isDragging) {
        std::string strategy;
        int x;
        switch (gameData.selected->getComponent<Strategy>()->value) {
            case CLOSEST:
                strategy = "Close";
                x = 56;
                break;
            case FIRST:
                strategy = "First";
                x = 57;
                break;
            case LAST:
                strategy = "Last";
                x = 59;
                break;
            case STRONGEST:
                strategy = "Strong";
                x = 51;
                break;
        }
        FC_Draw(gameData.fonts[WHITE12], gameData.renderer, x * gameData.mapScale, 16 * gameData.mapScale,
                strategy.c_str());
        FC_Draw(gameData.fonts[WHITE12], gameData.renderer, 25 * gameData.mapScale, 451 * gameData.mapScale,
                "Sell for:");
        FC_Draw(gameData.fonts[WHITE8], gameData.renderer, 25 * gameData.mapScale, 463 * gameData.mapScale,
                "$%s", formatCommas(int(gameData.selected->getComponent<Cost>()->value * 0.75)).c_str());

    }
    SDL_RenderPresent(gameData.renderer);

}


