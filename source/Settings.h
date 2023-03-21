#pragma once

class Settings {
public:
    // AvailPatch
    bool EnablePauseMenu;
    bool EnableLoadingScreen;

    // Mouse
    bool ClipMousePosition;

public:
    void Read();
};

extern Settings settings;
