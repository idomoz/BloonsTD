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
    // Window dimensions are picked in screen "points" — what the user
    // perceives as the window's size — so mapScale at this stage is the
    // points-space scale (default 1.5 on macOS). After the renderer is
    // created we read the backing-pixel/points ratio (Retina = 2.0,
    // standard = 1.0) and bump gameData.mapScale by it so the source
    // render lands in backing pixels — eliminating the OS bilinear
    // upscale that made text and shapes blurry on Retina.
    // SDL_WINDOW_ALLOW_HIGHDPI is what makes the renderer expose the
    // high-DPI backing in the first place.
    //
    // Emscripten is excluded from both: main.cpp already computes
    // mapScale from canvas physical pixels, the SDL2 emscripten port
    // doesn't implement HIGHDPI semantics, and the bump check would be
    // a no-op anyway (canvas backing == window-size request).
    gameData.pointsScale = gameData.mapScale;
    Uint32 winFlags = gameData.fullscreen ? SDL_WINDOW_FULLSCREEN : 0;
#ifndef __EMSCRIPTEN__
    winFlags |= SDL_WINDOW_ALLOW_HIGHDPI;
#endif
    gameData.window = SDL_CreateWindow("BloonsTD", SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED, /* NOLINT(hicpp-signed-bitwise)*/
                                       int((MAP_WIDTH + SIDEBAR_WIDTH + MENU_WIDTH) * gameData.pointsScale),
                                       int(MAP_HEIGHT * gameData.pointsScale),
                                       winFlags);
#endif
    gameData.renderer = SDL_CreateRenderer(gameData.window, -1, 0);
    SDL_SetRenderDrawColor(gameData.renderer, 255, 255, 255, 255);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
#if !defined(BLOONSTD_IOS) && !defined(__EMSCRIPTEN__)
    // Renderer output size is in backing pixels; window size is in points.
    // Their ratio is the OS's Retina factor (2 on macOS Retina, 1 elsewhere).
    int outW = 0, outH = 0, winW = 0, winH = 0;
    SDL_GetRendererOutputSize(gameData.renderer, &outW, &outH);
    SDL_GetWindowSize(gameData.window, &winW, &winH);
    if (winW > 0 && outW > winW) {
        float backingScale = (float) outW / (float) winW;
        gameData.mapScale *= backingScale;
    }
#endif
#ifdef BLOONSTD_IOS
    int outW = 0, outH = 0;
    SDL_GetRendererOutputSize(gameData.renderer, &outW, &outH);
    SDL_Log("BloonsTD iOS renderer output: %dx%d", outW, outH);
    // Scale the game's fixed logical canvas to the device screen with
    // letterbox bars where the aspect ratios disagree. The trick to
    // staying sharp is picking the LOGICAL SIZE at backing-pixel
    // resolution (preserving game aspect) rather than at 1086×511 —
    // SDL still letterboxes for us, but every glyph and sprite is now
    // rasterised at native pixel density instead of being bilinear-
    // upscaled from a 1086-wide source. mapScale absorbs the new
    // logical:LOGICAL_W ratio so the rest of the renderer keeps drawing
    // in 1086-space coords (everything is multiplied by mapScale when
    // drawing). Mouse handling divides by mapScale after
    // SDL_RenderWindowToLogical to recover 1086-space input coords.
    constexpr int LOGICAL_W = MAP_WIDTH + SIDEBAR_WIDTH + MENU_WIDTH;
    constexpr int LOGICAL_H = MAP_HEIGHT;
    int newLogW, newLogH;
    // outW/outH = backing pixels. Pick the larger game-aspect rect that
    // fits inside it; SDL handles the bars outside that rect.
    if (outW * LOGICAL_H >= outH * LOGICAL_W) {
        newLogH = outH;
        newLogW = (outH * LOGICAL_W) / LOGICAL_H;
    } else {
        newLogW = outW;
        newLogH = (outW * LOGICAL_H) / LOGICAL_W;
    }
    SDL_RenderSetLogicalSize(gameData.renderer, newLogW, newLogH);
    gameData.mapScale = (float) newLogW / (float) LOGICAL_W;
    SDL_Log("BloonsTD iOS logical: %dx%d, mapScale=%.3f", newLogW, newLogH, gameData.mapScale);
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


