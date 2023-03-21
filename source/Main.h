#pragma once
#define REDEFINE_TEXT_Y (154)
#define REDEFINE_TEXT_SPACING (22)
#define REDEFINE_RECT_SIZE (22)


#define DIK_LMB 238
#define DIK_MMB 239
#define DIK_RMB 240
#define DIK_WHEELUP 241
#define DIK_WHEELDN 242

#define MENU_LEFT_PERCENTAGE (0.44f)
#define MENU_RIGHT_PERCENTAGE (0.56f)

#define MENU_BACKGROUND_WIDTH (640.0f)
#define MENU_BACKGROUND_HEIGHT (480.0f)

#define MENU_BACKGROUND_LEFT_WIDTH (MENU_BACKGROUND_WIDTH * MENU_LEFT_PERCENTAGE)
#define MENU_BACKGROUND_RIGHT_WIDTH (MENU_BACKGROUND_WIDTH * MENU_RIGHT_PERCENTAGE)

#define HD_FRONTEND 0

#define MAX_LOCK_ON_DIST 15.0f

enum {
    CUSTOMMENUACTION_SFX = 10,
    CUSTOMMENUACTION_MUSIC,
    CUSTOMMENUACTION_CHANGERES,
    CUSTOMMENUACTION_SCREENTYPE,
    CUSTOMMENUACTION_LANGUAGE,
    CUSTOMMENUACTION_LIGHTING,
    CUSTOMMENUACTION_MOUSECONTROLLEDHEADING,
    CUSTOMMENUACTION_REDEFINEKEY,
    CUSTOMMENUACTION_RESTOREDEFAULT,
};

enum {
    PSXSPRITE_SFX,
    PSXSPRITE_MUSIC,
    PSXSPRITE_RES,
    PSXSPRITE_SCREENTYPE,
    PSXSPRITE_LANG,
    PSXSPRITE_LGHT,
    PSXSPRITE_MOUSE,
};

struct tResList {
    int w;
    int h;
    wchar_t str[16];
};

struct CMouseState {
    char buttons[5]; // lmb, rmb, mmb, wheelup, wheeldn
    float x;
    float y;

    CMouseState() {
        Clear();
    }

    ~CMouseState() {
        Clear();
    }

    void Clear() {
        buttons[0] = 0;
        buttons[1] = 0;
        buttons[2] = 0;
        buttons[3] = 0;
        buttons[4] = 0;
        x = 0;
        y = 0;
    }
};

#include <locale>
#include <iostream>
#include <ctype.h>

class CText;

//extern CText* BobTheText;

extern void CText__Load(CText* text, bool bob = false);
