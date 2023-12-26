#pragma once

class Settings {
public:
    static const int DEFAULT_CONTROLS_FRONTEND_FIX = 0;
    static const int DEFAULT_CONTROLS_ORIGINAL = 1;
    static const int DEFAULT_CONTROLS_CLASSIC = 2;

    // AvailPatch
    bool EnablePauseMenu;
    bool EnableLoadingScreen;

    // Mouse
    bool ClipMousePosition;

    // Controls
    int DefaultControls = DEFAULT_CONTROLS_FRONTEND_FIX;

public:
    void Read();
};

extern Settings settings;
