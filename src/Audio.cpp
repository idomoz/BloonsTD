#include "Audio.h"
#include "Assets.h"

#include <cstdlib>
#include <SDL.h>
#include <SDL_mixer.h>

namespace {
    // Throttle for bloon pops. SDL_mixer has a fixed channel count and rapid
    // identical sounds quickly become noise; one pop every 25ms (~40/s) keeps
    // big waves audible without melting the speakers.
    constexpr unsigned long MIN_POP_GAP_MS = 25;

    // Path *fragments* relative to the assets root. assetPath() prepends the
    // platform-specific root at load time.
    const char *SFX_FILES[SFX_COUNT] = {
            "sounds/Pop1.mp3",
            "sounds/Pop2.mp3",
            "sounds/Pop3.mp3",
            "sounds/Pop4.mp3",
            "sounds/Place.mp3",
            "sounds/Sell.mp3",
            "sounds/Upgrade.mp3",
            "sounds/Select.mp3",
            "sounds/GameOver.mp3",
            "sounds/GameWin.mp3",
            "sounds/MoabDestroyedBig.mp3",
            "sounds/ExplosionSmall.mp3",
            "sounds/ExplosionMedium.mp3",
            "sounds/ExplosionBig.mp3",
            "sounds/ExplosionHuge.mp3",
            "sounds/MoabAssassinMissile.mp3",
            "sounds/GlueSplatter.mp3",
            "sounds/MoabDamage1.mp3",
            "sounds/MoabDamage2.mp3",
            "sounds/MoabDamage3.mp3",
            "sounds/CeramicBloonHit.mp3",
            "sounds/MetalBloonHit.mp3",
    };

    const char *MUSIC_FILES[MUSIC_COUNT] = {
            "sounds/MainTheme.mp3",
    };
}

bool Audio::init() {
    if (initialized)
        return true;

    int wantFlags = MIX_INIT_MP3;
    int gotFlags = Mix_Init(wantFlags);
    if ((gotFlags & wantFlags) != wantFlags) {
        SDL_Log("Mix_Init MP3 missing: %s", Mix_GetError());
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
        SDL_Log("Mix_OpenAudio failed: %s", Mix_GetError());
        return false;
    }
    Mix_AllocateChannels(32);

    for (int i = 0; i < SFX_COUNT; ++i) {
        std::string p = assetPath(SFX_FILES[i]);
        sfx[i] = Mix_LoadWAV(p.c_str());
        if (!sfx[i])
            SDL_Log("Failed to load sfx '%s': %s", p.c_str(), Mix_GetError());
    }
    for (int i = 0; i < MUSIC_COUNT; ++i) {
        std::string p = assetPath(MUSIC_FILES[i]);
        music[i] = Mix_LoadMUS(p.c_str());
        if (!music[i])
            SDL_Log("Failed to load music '%s': %s", p.c_str(), Mix_GetError());
    }

    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    Mix_Volume(-1, MIX_MAX_VOLUME / 2);

    initialized = true;
    return true;
}

void Audio::shutdown() {
    if (!initialized)
        return;
    Mix_HaltMusic();
    for (auto &m: music) {
        if (m) {
            Mix_FreeMusic(m);
            m = nullptr;
        }
    }
    for (auto &c: sfx) {
        if (c) {
            Mix_FreeChunk(c);
            c = nullptr;
        }
    }
    Mix_CloseAudio();
    Mix_Quit();
    initialized = false;
}

Audio::~Audio() {
    shutdown();
}

void Audio::playSound(SoundId id) {
    if (!initialized || id < 0 || id >= SFX_COUNT || !sfx[id])
        return;
    Mix_PlayChannel(-1, sfx[id], 0);
}

void Audio::playThrottled(SoundId id, unsigned long gapMs) {
    if (!initialized || id < 0 || id >= SFX_COUNT || !sfx[id])
        return;
    unsigned long now = SDL_GetTicks();
    if (now - lastThrottledMs[id] < gapMs)
        return;
    lastThrottledMs[id] = now;
    Mix_PlayChannel(-1, sfx[id], 0);
}

void Audio::playPop() {
    if (!initialized)
        return;
    unsigned long now = SDL_GetTicks();
    if (now - lastPopMs < MIN_POP_GAP_MS)
        return;
    lastPopMs = now;
    SoundId id = static_cast<SoundId>(SFX_POP1 + (std::rand() % 4));
    playSound(id);
}

void Audio::playMoabDamage() {
    SoundId id = static_cast<SoundId>(SFX_MOAB_DAMAGE_1 + (std::rand() % 3));
    // Throttle the *family* (any MOAB-damage variant) on a single timer so
    // rolls of the dice don't bypass the gap.
    if (!initialized)
        return;
    unsigned long now = SDL_GetTicks();
    if (now - lastThrottledMs[SFX_MOAB_DAMAGE_1] < 120)
        return;
    lastThrottledMs[SFX_MOAB_DAMAGE_1] = now;
    Mix_PlayChannel(-1, sfx[id], 0);
}

void Audio::playCeramicHit() {
    playThrottled(SFX_CERAMIC_HIT, 60);
}

void Audio::playMetalHit() {
    playThrottled(SFX_METAL_HIT, 60);
}

void Audio::playGameOverOnce() {
    if (gameOverPlayed)
        return;
    gameOverPlayed = true;
    playSound(SFX_GAME_OVER);
}

void Audio::playGameWinOnce() {
    if (gameWinPlayed)
        return;
    gameWinPlayed = true;
    playSound(SFX_GAME_WIN);
}

void Audio::playMusic(MusicId id) {
    if (!initialized || id < 0 || id >= MUSIC_COUNT || !music[id])
        return;
    Mix_PlayMusic(music[id], -1);
}

void Audio::stopMusic() {
    if (!initialized)
        return;
    Mix_HaltMusic();
}
