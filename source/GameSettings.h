#pragma once
#include "CGame.h"
#include <array>

class CGameSettings {
public:
    // Controls
    std::array<int, NUM_CONTROLKEYS> controlKeys;

    // Audio
    int sfxVolume;
    int musicVolume;

    // Display
    int screenWidth;
    int screenHeight;
    int screenType;

    // Game
    int lightingType;
    char language;
    bool mouseControlledHeading;

    // Internal
    bool initialised;

public:
    void Clear(bool clearOnly = false);
    void Save();
    void Load();
    void PassSettingsToGame();
};
