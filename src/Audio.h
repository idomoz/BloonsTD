#ifndef BLOONSTD_AUDIO_H
#define BLOONSTD_AUDIO_H

#include <array>
#include <SDL_mixer.h>

enum SoundId {
    SFX_POP1,
    SFX_POP2,
    SFX_POP3,
    SFX_POP4,
    SFX_PLACE,
    SFX_SELL,
    SFX_UPGRADE,
    SFX_SELECT,
    SFX_GAME_OVER,
    SFX_GAME_WIN,
    SFX_MOAB_DESTROYED,
    SFX_EXPLOSION_SMALL,
    SFX_EXPLOSION_MEDIUM,
    SFX_EXPLOSION_BIG,
    SFX_EXPLOSION_HUGE,
    SFX_MOAB_ASSASSIN_MISSILE,
    SFX_GLUE_SPLATTER,
    SFX_MOAB_DAMAGE_1,
    SFX_MOAB_DAMAGE_2,
    SFX_MOAB_DAMAGE_3,
    SFX_CERAMIC_HIT,
    SFX_METAL_HIT,
    SFX_COUNT
};

enum MusicId {
    MUSIC_MAIN_THEME,
    MUSIC_COUNT
};

class Audio {
public:
    bool init();
    void shutdown();

    void playSound(SoundId id);
    // Plays `id` only if at least `gapMs` have passed since the last throttled
    // call for that same id. Useful for high-rate triggers (shots, splatter).
    void playThrottled(SoundId id, unsigned long gapMs = 50);
    // Random Pop1..Pop4, throttled so a wave of pops doesn't blow out the mixer.
    void playPop();
    // Random MoabDamage1..3, throttled — fires every time a MOAB-class bloon
    // takes damage but doesn't die.
    void playMoabDamage();
    // CeramicBloonHit, throttled — fires per damage tick on a ceramic that
    // survives (the "clink" of something striking the shell).
    void playCeramicHit();
    // MetalBloonHit, throttled — plays when a shot pings off a lead bloon
    // (the only damage-skip branch in the current game mechanics).
    void playMetalHit();
    // Plays once per game; subsequent calls are ignored.
    void playGameOverOnce();
    void playGameWinOnce();
    void playMusic(MusicId id);
    void stopMusic();

    ~Audio();

private:
    bool initialized = false;
    bool gameOverPlayed = false;
    bool gameWinPlayed = false;
    unsigned long lastPopMs = 0;
    std::array<unsigned long, SFX_COUNT> lastThrottledMs{};
    std::array<Mix_Chunk *, SFX_COUNT> sfx{};
    std::array<Mix_Music *, MUSIC_COUNT> music{};
};

#endif //BLOONSTD_AUDIO_H
