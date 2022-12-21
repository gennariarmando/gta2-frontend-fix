#pragma once
#include "CGame.h"
#include <array>

class CSettings {
public:
    // Controls
    std::array<int, NUM_CONTROLKEYS> controlKeys;

    // Audio
    char sfxVolume;
    char musicVolume;

    // Display
    int screenWidth;
    int screenHeight;
    int screenType;

    // Game
    int lightingType;
    char language;

public:
    void Clear();
    void Save();
    void Load();
    void PassSettingsToGame();
};
