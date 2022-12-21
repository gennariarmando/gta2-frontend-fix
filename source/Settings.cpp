#include "plugin.h"
#include "Settings.h"
#include "tGraphics.h"
#include "cDMAudio.h"
#include "pugixml.hpp"
#include <Shlwapi.h>
#include <dinput.h>
#include "CGame.h"
#include "CText.h"

#pragma comment(lib, "shlwapi")

#include "Utility.h"

char* userFilesFolder = "ufiles\\";
char* settingsFileName = "ufiles\\settings.xml";

void CSettings::Clear() {
    // DMA Design Ltd sowwy :(
    SHDeleteKeyA(HKEY_CURRENT_USER, "SOFTWARE\\DMA Design Ltd\\GTA2");
    SHDeleteKeyA(HKEY_LOCAL_MACHINE, "SOFTWARE\\DMA Design Ltd\\GTA2");

    // Controls
    controlKeys[CONTROLKEY_FORWARD] = DIK_W;
    controlKeys[CONTROLKEY_BACKWARD] = DIK_S;
    controlKeys[CONTROLKEY_LEFT] = DIK_A;
    controlKeys[CONTROLKEY_RIGHT] = DIK_D;
    controlKeys[CONTROLKEY_ATTACK] = DIK_LSHIFT;
    controlKeys[CONTROLKEY_ENTEROREXIT] = DIK_F;
    controlKeys[CONTROLKEY_HANDBRAKEORJUMP] = DIK_SPACE;
    controlKeys[CONTROLKEY_PREVWEAPON] = DIK_Q;
    controlKeys[CONTROLKEY_NEXTWEAPON] = DIK_E;
    controlKeys[CONTROLKEY_SPECIAL1] = DIK_TAB;
    controlKeys[CONTROLKEY_SPECIAL2] = DIK_LCONTROL;
    controlKeys[CONTROLKEY_UNKNOWN] = DIK_RSHIFT;

    // Audio
    sfxVolume = 127;
    musicVolume = 127;

    // Display
    screenWidth = 640;
    screenHeight = 480;
    screenType = 1;

    // Game
    lightingType = 1;
    language = 'e';
}

void CSettings::Save() {
    pugi::xml_document doc;
    auto declarationNode = doc.append_child(pugi::node_declaration);
    declarationNode.append_attribute("version") = "1.0";
    declarationNode.append_attribute("encoding") = "UTF-8";

    auto settings = doc.append_child("Settings");
    settings.append_child("version").append_attribute("value").set_value(1.0);

    if (auto controls = settings.append_child("controls")) {
        controls.append_child("Forward").append_attribute("value").set_value(controlKeys[CONTROLKEY_FORWARD]);
        controls.append_child("Backward").append_attribute("value").set_value(controlKeys[CONTROLKEY_BACKWARD]);
        controls.append_child("Left").append_attribute("value").set_value(controlKeys[CONTROLKEY_LEFT]);
        controls.append_child("Right").append_attribute("value").set_value(controlKeys[CONTROLKEY_RIGHT]);
        controls.append_child("Attack").append_attribute("value").set_value(controlKeys[CONTROLKEY_ATTACK]);
        controls.append_child("EnterExit").append_attribute("value").set_value(controlKeys[CONTROLKEY_ENTEROREXIT]);
        controls.append_child("HandbrakeJump").append_attribute("value").set_value(controlKeys[CONTROLKEY_HANDBRAKEORJUMP]);
        controls.append_child("PreviousWeapon").append_attribute("value").set_value(controlKeys[CONTROLKEY_PREVWEAPON]);
        controls.append_child("NextWeapon").append_attribute("value").set_value(controlKeys[CONTROLKEY_NEXTWEAPON]);
        controls.append_child("Special").append_attribute("value").set_value(controlKeys[CONTROLKEY_SPECIAL1]);
        controls.append_child("Special2").append_attribute("value").set_value(controlKeys[CONTROLKEY_SPECIAL2]);
    }

    if (auto audio = settings.append_child("audio")) {
        audio.append_child("SfxVolume").append_attribute("value").set_value(sfxVolume);
        audio.append_child("MusicVolume").append_attribute("value").set_value(musicVolume);
    }

    if (auto display = settings.append_child("display")) {
        display.append_child("ScreenWidth").append_attribute("value").set_value(screenWidth);
        display.append_child("ScreenHeight").append_attribute("value").set_value(screenHeight);

        char* st = NULL;
        switch (screenType) {
        case 1:
            st = "windowed";
            break;
        default:
            st = "fullscreen";
            break;
        }
        display.append_child("ScreenType").append_attribute("value").set_value(st);
    }

    if (auto game = settings.append_child("game")) {
        char* lt = NULL;
        switch (lightingType) {
        case 1:
            lt = "dusk";
            break;
        default:
            lt = "noon";
            break;
        }

        game.append_child("LightingType").append_attribute("value").set_value(lt);

        char buff[8];
        sprintf(buff, "%c", language);
        game.append_child("Language").append_attribute("value").set_value(buff);
    }

    bool file = doc.save_file(settingsFileName);
    if (CreateDirectory(userFilesFolder, NULL) || GetLastError() == ERROR_ALREADY_EXISTS) {
        if (!file) {
            printf("XML: Settings file can't be saved.");
        }
    }

    PassSettingsToGame();
}

void CSettings::Load() {
    Clear();

    pugi::xml_document doc;
    pugi::xml_parse_result file = doc.load_file(settingsFileName);
    if (file) {
        auto settings = doc.child("Settings");
        auto version = settings.child("version").attribute("value");

        if (version.as_double() != 1.0) {
            printf("XML: Settings file is not compatible.");
        }
        else {
            if (auto controls = settings.child("controls")) {
                controlKeys[CONTROLKEY_FORWARD] = controls.child("Forward").attribute("value").as_int(controlKeys[CONTROLKEY_FORWARD]);
                controlKeys[CONTROLKEY_BACKWARD] = controls.child("Backward").attribute("value").as_int(controlKeys[CONTROLKEY_BACKWARD]);
                controlKeys[CONTROLKEY_LEFT] = controls.child("Left").attribute("value").as_int(controlKeys[CONTROLKEY_LEFT]);
                controlKeys[CONTROLKEY_RIGHT] = controls.child("Right").attribute("value").as_int(controlKeys[CONTROLKEY_RIGHT]);
                controlKeys[CONTROLKEY_ATTACK] = controls.child("Attack").attribute("value").as_int(controlKeys[CONTROLKEY_ATTACK]);
                controlKeys[CONTROLKEY_ENTEROREXIT] = controls.child("EnterExit").attribute("value").as_int(controlKeys[CONTROLKEY_ENTEROREXIT]);
                controlKeys[CONTROLKEY_HANDBRAKEORJUMP] = controls.child("HandbrakeJump").attribute("value").as_int(controlKeys[CONTROLKEY_HANDBRAKEORJUMP]);
                controlKeys[CONTROLKEY_PREVWEAPON] = controls.child("PreviousWeapon").attribute("value").as_int(controlKeys[CONTROLKEY_PREVWEAPON]);
                controlKeys[CONTROLKEY_NEXTWEAPON] = controls.child("NextWeapon").attribute("value").as_int(controlKeys[CONTROLKEY_NEXTWEAPON]);
                controlKeys[CONTROLKEY_SPECIAL1] = controls.child("Special").attribute("value").as_int(controlKeys[CONTROLKEY_SPECIAL1]);
                controlKeys[CONTROLKEY_SPECIAL2] = controls.child("Special2").attribute("value").as_int(controlKeys[CONTROLKEY_SPECIAL2]);
            }

            if (auto audio = settings.child("audio")) {
                sfxVolume = audio.child("SfxVolume").attribute("value").as_int(sfxVolume);
                musicVolume = audio.child("MusicVolume").attribute("value").as_int(musicVolume);

                sfxVolume = Clamp(sfxVolume, 0, 127);
                musicVolume = Clamp(sfxVolume, 0, 127);
            }

            if (auto display = settings.child("display")) {
                screenWidth = display.child("ScreenWidth").attribute("value").as_int(screenWidth);
                screenHeight = display.child("ScreenHeight").attribute("value").as_int(screenWidth);
                const char* st = display.child("ScreenType").attribute("value").as_string("windowed");
                if (!strcmp(st, "windowed")) {
                    screenType = 1;
                }
                else if (!strcmp(st, "fullscreen")) {
                    screenType = 0;
                }
            }

            if (auto game = settings.child("game")) {
                const char* lt = game.child("LightingType").attribute("value").as_string("noon");
                if (!strcmp(lt, "dusk")) {
                    lightingType = 1;
                }
                else if (!strcmp(lt, "noon")) {
                    lightingType = 0;
                }

                const char* la = game.child("Language").attribute("value").as_string("e");
                switch (la[0]) {
                case 'e':
                case 'f':
                case 'g':
                case 'i':
                case 's':
                case 'j':
                    language = la[0];
                    break;
                default:
                    language = 'e';
                    break;
                };
            }
        }
    }

    PassSettingsToGame();
}

void CSettings::PassSettingsToGame() {
    // Controls 
    for (int i = 0; i < NUM_CONTROLKEYS; i++)
        ControlKeys[i] = controlKeys[i];

    // Audio
    DMAudio.SetCurrent3DProvider(0);
    DMAudio.SetEffectsMasterVolume(sfxVolume);
    DMAudio.SetMusicMasterVolume(musicVolume);

    // Display
    window_width = screenWidth;
    window_height = screenHeight;
    start_mode = screenType;
    max_frame_rate = 1;
    min_frame_rate = 1;
    triple_buffer = 1;
    exploding_on = 1;

    // Game
    lighting = lightingType;
    if (GetTheText()) {
        GetTheText()->language = language;
        GetTheText()->Unload();
        GetTheText()->Load();
    }
}
