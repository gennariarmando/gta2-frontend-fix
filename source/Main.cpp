#include "plugin.h"
#include "common.h"
#include "CMenuManager.h"
#include "CSprite2d.h"
#include "Utility.h"
#include "CText.h"
#include "CPlayerPed.h"
#include "CPed.h"
#include "CGame.h"
#include "CObject.h"
#include "CGlobal.h"
#include "CKeybrd.h"
#include "Settings.h"
#include "CChar.h"

#include "tVideo.h"
#include "cDMAudio.h"
#include "cDMAVideo.h"
#include "PsxSprites.h"

#include <shellscalingapi.h>

#pragma comment(lib, "Shcore")

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
    CUSTOMMENUACTION_REDEFINEKEY,
};

#include <array>

std::array<CSprite2d, 6> psxSprites;

std::vector<CSprite2d*> frontendSprites;
std::vector<std::string> frontendTexFileNames = {
    "data/frontend/1.tga",
    "data/frontend/1_Options.tga",
    "data/frontend/1_Play.tga",
    "data/frontend/1_Quit.tga",
    "data/frontend/2.tga",
    "data/frontend/2_Bonus1.tga",
    "data/frontend/2_Bonus2.tga",
    "data/frontend/2_Bonus3.tga",
    "data/frontend/2_League.tga",
    "data/frontend/2_Level1.tga",
    "data/frontend/2_Level2.tga",
    "data/frontend/2_Level3.tga",
    "data/frontend/2_Name.tga",
    "data/frontend/2_Restart.tga",
    "data/frontend/3.tga",
    "data/frontend/3_Tables.tga",
    "data/frontend/GameComplete.tga",
    "data/frontend/LevelComplete.tga",
    "data/frontend/MPLose.tga",
    "data/frontend/PlayerDead.tga",
    "data/frontend/Mask.tga",
    "data/frontend/Mask2.tga",
    "data/frontend/Credits.tga",
    "data/frontend/Mask3.tga",
    "data/frontend/DemoInfo.tga",
};

#if HD_FRONTEND
CSprite2d logo;
#endif

bool full_screen = false;
bool wsfix = false;
CSettings gameSettings;

struct tResList {
    int w;
    int h;
    wchar_t str[16];
};

std::vector<tResList> listOfRes;
int currentScreenResIndex = 0;
int previousScreenResIndex = 0;
int currentLanguage = 0;
bool redefineKey = false;

char oldKeys[256];
char newKeys[256];
int previousMenuItem = 0;


const char lang[6] = { 'e', 'f', 'g', 'i', 's', 'j' };

const wchar_t* dinputKeyNames[DIK_MEDIASELECT + 1] = {
    L"UNBOUND", //0
    L"ESCAPE", //1
    L"1", //2
    L"2", //3
    L"3", //4
    L"4", //5
    L"5", //6
    L"6", //7
    L"7", //8
    L"8", //9
    L"9", //10
    L"0", //11
    L"MINUS", //12
    L"EQUALS", //13
    L"BACK", //14
    L"TAB", //15
    L"Q", //16
    L"W", //17
    L"E", //18
    L"R", //19
    L"T", //20
    L"Y", //21
    L"U", //22
    L"I", //23
    L"O", //24
    L"P", //25
    L"LBRACKET", //26
    L"RBRACKET", //27
    L"RETURN", //28
    L"LCONTROL", //29
    L"A", //30
    L"S", //31
    L"D", //32
    L"F", //33
    L"G", //34
    L"H", //35
    L"J", //36
    L"K", //37
    L"L", //38
    L"SEMICOLON", //39
    L"APOSTROPHE", //40
    L"GRAVE", //41
    L"LSHIFT", //42
    L"BACKSLASH", //43
    L"Z", //44
    L"X", //45
    L"C", //46
    L"V", //47
    L"B", //48
    L"N", //49
    L"M", //50
    L"COMMA", //51
    L"PERIOD", //52
    L"SLASH ", //53
    L"RSHIFT", //54
    L"MULTIPLY", //55
    L"LMENU", //56
    L"SPACE", //57
    L"CAPITAL", //58
    L"F1", //59
    L"F2", //60
    L"F3", //61
    L"F4", //62
    L"F5", //63
    L"F6", //64
    L"F7", //65
    L"F8", //66
    L"F9", //67
    L"F10", //68
    L"NUMLOCK", //69
    L"SCROLL", //70
    L"NUMPAD7", //71
    L"NUMPAD8", //72
    L"NUMPAD9", //73
    L"SUBTRACT", //74
    L"NUMPAD4", //75
    L"NUMPAD5", //76
    L"NUMPAD6", //77
    L"ADD", //78
    L"NUMPAD1", //79
    L"NUMPAD2", //80
    L"NUMPAD3", //81
    L"NUMPAD0", //82
    L"DECIMAL", //83
    L"UNK", //84
    L"UNK", //85
    L"OEM_102", //86
    L"F11", //87
    L"F12", //88
    L"UNK", //89
    L"UNK", //90
    L"UNK", //91
    L"UNK", //92
    L"UNK", //93
    L"UNK", //94
    L"UNK", //95
    L"UNK", //96
    L"UNK", //97
    L"UNK", //98
    L"UNK", //99
    L"F13", //100
    L"F14", //101
    L"F15", //102
    L"UNK", //103
    L"UNK", //104
    L"UNK", //105
    L"UNK", //106
    L"UNK", //107
    L"UNK", //108
    L"UNK", //109
    L"UNK", //110
    L"UNK", //111
    L"KANA", //112
    L"UNK", //113
    L"UNK", //114
    L"ABNT_C1", //115
    L"UNK", //116
    L"UNK", //117
    L"UNK", //118
    L"UNK", //119
    L"UNK", //120
    L"CONVERT", //121
    L"UNK", //122
    L"NOCONVERT", //123
    L"UNK", //124
    L"YEN", //125
    L"ABNT_C2", //126
    L"UNK", //127
    L"UNK", //128
    L"UNK", //129
    L"UNK", //130
    L"UNK", //131
    L"UNK", //132
    L"UNK", //133
    L"UNK", //134
    L"UNK", //135
    L"UNK", //136
    L"UNK", //137
    L"UNK", //138
    L"UNK", //139
    L"UNK", //140
    L"NUMPADEQUALS", //141
    L"UNK", //142
    L"UNK", //143
    L"PREVTRACK", //144
    L"AT", //145
    L"COLON", //146
    L"UNDERLINE", //147
    L"KANJI", //148
    L"STOP", //149
    L"AX", //150
    L"UNLABELED", //151
    L"UNK", //152
    L"NEXTTRACK", //153
    L"UNK", //154
    L"UNK", //155
    L"NUMPADENTER ", //156
    L"RCONTROL ", //157
    L"UNK", //158
    L"UNK", //159
    L"MUTE", //160
    L"CALCULATOR", //161
    L"PLAYPAUSE", //162
    L"UNK", //163
    L"MEDIASTOP", //164
    L"UNK", //165
    L"UNK", //166
    L"UNK", //167
    L"UNK", //168
    L"UNK", //169
    L"UNK", //170
    L"UNK", //171
    L"UNK", //172
    L"UNK", //173
    L"VOLUMEDOWN", //174
    L"UNK", //175
    L"VOLUMEUP ", //176
    L"UNK", //177
    L"WEBHOME", //178
    L"NUMPADCOMMA", //179
    L"UNK", //180
    L"DIVIDE", //181
    L"UNK", //182
    L"SYSRQ", //183
    L"RMENU", //184
    L"UNK", //185
    L"UNK", //186
    L"UNK", //187
    L"UNK", //188
    L"UNK", //189
    L"UNK", //190
    L"UNK", //191
    L"UNK", //192
    L"UNK", //193
    L"UNK", //194
    L"UNK", //195
    L"UNK", //196
    L"PAUSE", //197
    L"UNK", //198
    L"HOME", //199
    L"UP", //200
    L"PRIOR", //201
    L"UNK", //202
    L"LEFT", //203
    L"UNK", //204
    L"RIGHT", //205
    L"UNK", //206
    L"END", //207
    L"DOWN", //208
    L"NEXT", //209
    L"INSERT", //210
    L"DELETE", //211
    L"UNK", //212
    L"UNK", //213
    L"UNK", //214
    L"UNK", //215
    L"UNK", //216
    L"UNK", //217
    L"UNK", //218
    L"LWIN", //219
    L"RWIN", //220
    L"APPS", //221
    L"POWER", //222
    L"SLEEP", //223
    L"UNK", //224
    L"UNK", //225
    L"UNK", //226
    L"WAKE", //227
    L"UNK", //228
    L"WEBSEARCH", //229
    L"WEBFAVORITES", //230
    L"WEBREFRESH", //231
    L"WEBSTOP", //232
    L"WEBFORWARD", //233
    L"WEBBACK", //234
    L"MYCOMPUTER  ", //235
    L"MAIL", //236
    L"MEDIASELECT", //237
};

CPed* lockOnTarget;
bool hasLockOnTarget;

static int frontendMenuPulse = 255;

class FrontendFix {
public:
    static void DoFontPulse() {
        static float pulse = 0;
        static bool reverse = false;

        pulse += reverse ? -0.05f : 0.05f;

        if (pulse >= 1.0f)
            reverse = true;

        if (pulse <= 0.25f)
            reverse = false;

        pulse = Clamp(pulse, 0.0f, 1.0f);
        frontendMenuPulse = 255 * pulse;
    }

    static void ApplyFontChange(bool pulse) {
        plugin::patch::SetUChar(0x4CC534 + 1, pulse ? frontendMenuPulse : 255);
    }

    static void SmoothRect(CRect const& rect, CRGBA const& c0, CRGBA const& c1, CRGBA const& c2, CRGBA const& c3) {
        CSprite2d::SetVertices(rect, c0, c1, c2, c3);

        RenderStateSet(D3DRENDERSTATE_TEXTUREHANDLE, NULL);

        RenderStateSet(D3DRENDERSTATE_SHADEMODE, (void*)D3DSHADE_GOURAUD);
        RenderStateSet(D3DRENDERSTATE_SHADEMODE, (void*)D3DSHADE_GOURAUD);
        RenderStateSet(D3DRENDERSTATE_ZENABLE, FALSE);
        RenderStateSet(D3DRENDERSTATE_ZWRITEENABLE, FALSE);
        RenderStateSet(D3DRENDERSTATE_VERTEXBLEND, (void*)(c0.a != 255));

        DrawPrimitive(D3DPT_TRIANGLEFAN, CSprite2d::ms_aVertices, 4);

        RenderStateSet(D3DRENDERSTATE_ZENABLE, (void*)TRUE);
        RenderStateSet(D3DRENDERSTATE_ZWRITEENABLE, (void*)TRUE);
        RenderStateSet(D3DRENDERSTATE_SHADEMODE, (void*)D3DSHADE_GOURAUD);
    }

    static CRGBA GetColFromRatio(float ratio, char alpha) {
        ratio = Clamp(ratio, 0.0f, 1.0f);
        int normalized = int(ratio * 256 * 6);
        int region = normalized / 256;
        int x = normalized % 256;

        static char r = 0, g = 0, b = 0;
        switch (region) {
        case 0:
            r = 255;
            g = 0;
            b = 0;
            g += x;
            break;
        case 1:
            r = 255;
            g = 255;
            b = 0;
            r -= x;
            break;
        case 2:
            r = 0;
            g = 255;
            b = 0;
            b += x;
            break;
        case 3:
            r = 0;
            g = 255;
            b = 255;
            g -= x;
            break;
        case 4:
            r = 0;
            g = 0;
            b = 255;
            r += x;
            break;
        case 5:
            r = 255;
            g = 0;
            b = 255;
            b -= x;
            break;
        }
        return CRGBA(r, g, b, alpha);
    }

    static void DrawRedefineCtrlPage(CMenuManager* _this) {
        CSprite2d::DrawRect(CRect(ScaleXKeepCentered(20.0f - 2.0f), ScaleY(152.0f - 2.0f), ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 20.0f + 2.0f), ScaleY(400.0f + 2.0f)), CRGBA(125, 125, 125, 255));
        CSprite2d::DrawRect(CRect(ScaleXKeepCentered(20.0f), ScaleY(152.0f), ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 20.0f), ScaleY(400.0f)), CRGBA(0, 0, 32, 255));

        PrintString(L"SETUP CONTROLS", (40) * 16384, 10 * 16384, _this->m_nFontStyle + 9, 16384);

        PrintString(L"ACTION", 40 * 16384, 154 * 16384, _this->m_nFontStyle + 9, 16384 / 2);

        int s = GetStringWidth(L"KEY", _this->m_nFontStyle + 9) / 4;
        PrintString(L"KEY", (DEFAULT_SCREEN_WIDTH - 100 - s) * 16384, (154) * 16384, _this->m_nFontStyle + 9, 16384 / 2);

        std::vector<const wchar_t*> controls = {
            L"FORWARD",
            L"BACKWARD",
            L"LEFT",
            L"RIGHT",
            L"ATTACK",
            L"ENTER/EXIT",
            L"HANDBRAKE/JUMP",
            L"PREV WEAPON",
            L"NEXT WEAPON",
            L"SPECIAL 1",
            L"SPECIAL 2",
        };

        int i = 0;
        for (auto& it : controls) {
            float x = (float)_this->m_MenuPages[_this->m_nCurrentMenuPage].items[i].x;
            float y = (float)_this->m_MenuPages[_this->m_nCurrentMenuPage].items[i].y;

            PrintString(it, x * 16384, y * 16384, _this->m_nFontStyle, 16384);
            i++;
        }
    }

    static void SetResolutionsList() {
        DEVMODE dm = {};
        dm.dmSize = sizeof(DEVMODE);

        int count = 0;
        for (auto i = 0; EnumDisplaySettings(NULL, i, &dm) != 0; i++) {
            if (dm.dmPelsWidth >= 640 && dm.dmPelsHeight >= 480 && dm.dmBitsPerPel == 32 && dm.dmDisplayFixedOutput == DMDFO_DEFAULT && dm.dmDisplayFrequency == 60) {
                tResList t;

                t.w = dm.dmPelsWidth;
                t.h = dm.dmPelsHeight;

                wsprintfW(t.str, L"%dX%d", dm.dmPelsWidth, dm.dmPelsHeight);
                listOfRes.push_back(t);

                if (t.w == gameSettings.screenWidth && t.h == gameSettings.screenHeight) {
                    currentScreenResIndex = count;
                }

                count++;
            }
        }

        previousScreenResIndex = currentScreenResIndex;
    }

    static void PopulateCustomFrontend(CMenuManager* _this) {
        // MENUPAGE_12
        _this->m_MenuPages[MENUPAGE_12].currentMenuItem = 0;
        _this->m_MenuPages[MENUPAGE_12].initialMenuItem = 0;

        _this->m_MenuPages[MENUPAGE_12].numMenuItems = 7;
        // red controls
        _this->m_MenuPages[MENUPAGE_12].items[0].action = MENUACTION_CHANGEPAGE;
        _this->m_MenuPages[MENUPAGE_12].items[0].x = 300;
        _this->m_MenuPages[MENUPAGE_12].items[0].y = 160;

        wcscpy(_this->m_MenuPages[MENUPAGE_12].items[0].str, L"SETUP CONTROLS");
        _this->m_MenuPages[MENUPAGE_12].items[0].targetPage = MENUPAGE_13;

        // res
        _this->m_MenuPages[MENUPAGE_12].items[1].action = CUSTOMMENUACTION_CHANGERES;
        _this->m_MenuPages[MENUPAGE_12].items[1].x = 300;
        _this->m_MenuPages[MENUPAGE_12].items[1].y = 200;

        wcscpy(_this->m_MenuPages[MENUPAGE_12].items[1].str, L"");
        _this->m_MenuPages[MENUPAGE_12].items[1].targetPage = MENUACTION_NONE;

        // screen type
        _this->m_MenuPages[MENUPAGE_12].items[2].action = CUSTOMMENUACTION_SCREENTYPE;
        _this->m_MenuPages[MENUPAGE_12].items[2].x = 300;
        _this->m_MenuPages[MENUPAGE_12].items[2].y = 240;

        wcscpy(_this->m_MenuPages[MENUPAGE_12].items[2].str, L"");
        _this->m_MenuPages[MENUPAGE_12].items[2].targetPage = MENUACTION_NONE;

        // sfx
        _this->m_MenuPages[MENUPAGE_12].items[3].action = CUSTOMMENUACTION_SFX;
        _this->m_MenuPages[MENUPAGE_12].items[3].x = 300;
        _this->m_MenuPages[MENUPAGE_12].items[3].y = 280;

        wcscpy(_this->m_MenuPages[MENUPAGE_12].items[3].str, L"");
        _this->m_MenuPages[MENUPAGE_12].items[3].targetPage = MENUACTION_NONE;

        // music
        _this->m_MenuPages[MENUPAGE_12].items[4].action = CUSTOMMENUACTION_MUSIC;
        _this->m_MenuPages[MENUPAGE_12].items[4].x = 300;
        _this->m_MenuPages[MENUPAGE_12].items[4].y = 320;

        wcscpy(_this->m_MenuPages[MENUPAGE_12].items[4].str, L"");
        _this->m_MenuPages[MENUPAGE_12].items[4].targetPage = MENUACTION_NONE;

        // lighting
        _this->m_MenuPages[MENUPAGE_12].items[5].action = CUSTOMMENUACTION_LIGHTING;
        _this->m_MenuPages[MENUPAGE_12].items[5].x = 300;
        _this->m_MenuPages[MENUPAGE_12].items[5].y = 360;

        wcscpy(_this->m_MenuPages[MENUPAGE_12].items[5].str, L"");
        _this->m_MenuPages[MENUPAGE_12].items[5].targetPage = MENUACTION_NONE;

        // lang
        _this->m_MenuPages[MENUPAGE_12].items[6].action = CUSTOMMENUACTION_LANGUAGE;
        _this->m_MenuPages[MENUPAGE_12].items[6].x = 300;
        _this->m_MenuPages[MENUPAGE_12].items[6].y = 400;

        wcscpy(_this->m_MenuPages[MENUPAGE_12].items[6].str, L"");
        _this->m_MenuPages[MENUPAGE_12].items[6].targetPage = MENUACTION_NONE;

        // MENUPAGE_13
        _this->m_MenuPages[MENUPAGE_13].numMenuItems = 11;
        _this->m_MenuPages[MENUPAGE_13].initialMenuItem = 0;

        // controls
        for (int i = 0; i < 11; i++) {
            _this->m_MenuPages[MENUPAGE_13].items[i].action = CUSTOMMENUACTION_REDEFINEKEY;
            _this->m_MenuPages[MENUPAGE_13].items[i].x = 40;
            _this->m_MenuPages[MENUPAGE_13].items[i].y = 178 + (20 * i);

            wcscpy(_this->m_MenuPages[MENUPAGE_13].items[i].str, L"");
            _this->m_MenuPages[MENUPAGE_13].items[i].targetPage = MENUACTION_NONE;
        }
    }

    static void DrawCustomPages(CMenuManager* _this) {
        if (_this->m_nCurrentMenuPage < MENUPAGE_12)
            return;

        for (int i = 0; i < 12; i++) {
            float x = (float)_this->m_MenuPages[_this->m_nCurrentMenuPage].items[i].x;
            float y = (float)_this->m_MenuPages[_this->m_nCurrentMenuPage].items[i].y;

            int selectedItem = (i == _this->m_MenuPages[_this->m_nCurrentMenuPage].currentMenuItem);

            ApplyFontChange(selectedItem);

            switch (_this->m_MenuPages[_this->m_nCurrentMenuPage].items[i].action) {
            case CUSTOMMENUACTION_SFX:
                psxSprites[0].Draw(ScaleXKeepCentered(x), ScaleY(y), ScaleX(34.0f), ScaleY(34.0f), CRGBA(255, !selectedItem ? 255 : 0, !selectedItem ? 255 : 0, selectedItem ? frontendMenuPulse : 255));
                DrawSlider(x + 64.0f, y + 4.0f, gameSettings.sfxVolume / 126.0f, selectedItem);
                break;
            case CUSTOMMENUACTION_MUSIC:
                psxSprites[1].Draw(ScaleXKeepCentered(x), ScaleY(y), ScaleX(34.0f), ScaleY(34.0f), CRGBA(255, !selectedItem ? 255 : 0, !selectedItem ? 255 : 0, selectedItem ? frontendMenuPulse : 255));
                DrawSlider(x + 64.0f, y + 4.0f, gameSettings.musicVolume / 126.0f, selectedItem);
                break;
            case CUSTOMMENUACTION_CHANGERES:  
                psxSprites[2].Draw(ScaleXKeepCentered(x), ScaleY(y), ScaleX(34.0f), ScaleY(34.0f), CRGBA(255, !selectedItem ? 255 : 0, !selectedItem ? 255 : 0, selectedItem ? frontendMenuPulse : 255));
                PrintString(listOfRes.at(currentScreenResIndex).str, (x + 64.0f) * 16384, (y + 4.0f) * 16384, _this->m_nFontStyle + selectedItem, 16384);
                break;
            case CUSTOMMENUACTION_SCREENTYPE:
                psxSprites[3].Draw(ScaleXKeepCentered(x), ScaleY(y), ScaleX(34.0f), ScaleY(34.0f), CRGBA(255, !selectedItem ? 255 : 0, !selectedItem ? 255 : 0, selectedItem ? frontendMenuPulse : 255));
                PrintString(gameSettings.screenType == 0 ? L"FULLSCREEN" : L"WINDOWED", (x + 64.0f) * 16384, (y + 4.0f) * 16384, _this->m_nFontStyle + selectedItem, 16384);
                break;
            case CUSTOMMENUACTION_LIGHTING: {
                psxSprites[5].Draw(ScaleXKeepCentered(x), ScaleY(y), ScaleX(34.0f), ScaleY(34.0f), CRGBA(255, !selectedItem ? 255 : 0, !selectedItem ? 255 : 0, selectedItem ? frontendMenuPulse : 255));

                wchar_t* str = L"UNKNOWN";
                switch (gameSettings.lightingType) {
                case 1:
                    str = L"MIDNIGHT";
                    break;
                default:
                    str = L"NOON";
                    break;
                }
                PrintString(str, (x + 64) * 16384, (y + 4) * 16384, _this->m_nFontStyle + selectedItem, 16384);
            }
                break;
            case CUSTOMMENUACTION_LANGUAGE: {
                psxSprites[4].Draw(ScaleXKeepCentered(x), ScaleY(y), ScaleX(34.0f), ScaleY(34.0f), CRGBA(255, !selectedItem ? 255 : 0, !selectedItem ? 255 : 0, selectedItem ? frontendMenuPulse : 255));

                wchar_t* str = L"UNKNOWN";
                switch (gameSettings.language) {
                case 'e':
                    str = L"ENGLISH";
                    break;
                case 'f':
                    str = L"FRENCH";
                    break;
                case 'g':
                    str = L"GERMAN";
                    break;
                case 'i':
                    str = L"ITALIAN";
                    break;
                case 's':
                    str = L"SPANISH";
                    break;
                case 'j':
                    str = L"JAPANESE";
                    break;
                }
                PrintString(str, (x + 64) * 16384, (y + 4) * 16384, _this->m_nFontStyle + selectedItem, 16384);
            }
                break;
            case CUSTOMMENUACTION_REDEFINEKEY: {
                const wchar_t* str = dinputKeyNames[gameSettings.controlKeys[i]];
                static int timer = 0;

                if (redefineKey && selectedItem) {
                    if (_this->m_nTime & 8)
                        str = L"???";
                    else
                        str = L" ";
                }

                int fontStyle = _this->m_nFontStyle;
                if (gameSettings.controlKeys[i] == 0 && !redefineKey) {
                    fontStyle += 1;
                }

                int s = (GetStringWidth(str, _this->m_nFontStyle) * 0.6f) / 2;
                PrintString(str, (x + 500 - s) * 16384, (y + 4) * 16384, fontStyle, (0.7f * 16384));
            } break;
            };
        }

        ApplyFontChange(false);

        switch (_this->m_nCurrentMenuPage) {
        case MENUPAGE_13: {
            int selectedItem = (_this->m_MenuPages[_this->m_nCurrentMenuPage].currentMenuItem);
            static float ratio = 0.0f;
            ratio += 0.0125f;

            float y = 180.0f;
            CRGBA c0 = GetColFromRatio(ratio, 150);
            CRGBA c1 = c0;
            CRGBA c2 = CRGBA(255 - c0.b, 255 - c0.g, 255 - c0.r, c0.a);
            CRGBA c3 = CRGBA(c0.b, c0.g, c0.r, c0.a);

            if (ratio > 1.0f) {
                ratio = 0.0f;
            }

            SmoothRect(CRect(ScaleXKeepCentered(20.0f), ScaleY(y + (selectedItem * 20.0f)), ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 20.0f), ScaleY(y + 20.0f + (selectedItem * 20.0f))),
                c0, c1, c2, c3);


            const float helpSize = 0.4f;
            if (redefineKey) {
                const wchar_t* str = L"SELECT A NEW CONTROL FOR THIS ACTION OR ESC TO CANCEL";

                int s = (GetStringWidth(str, _this->m_nFontStyle + 9) * helpSize) / 2;
                PrintString(str, (320 - s) * 16384, 420 * 16384, _this->m_nFontStyle + 9, helpSize * 16384);
            }
            else {
                const wchar_t* str = L"DELETE TO CLEAR, RETURN TO CHANGE";
                int s = (GetStringWidth(str, _this->m_nFontStyle + 9) * helpSize) / 2;
                PrintString(str, (320 - s) * 16384, 420 * 16384, _this->m_nFontStyle + 9, helpSize * 16384);
            }
        } break;
        };
    }

    static void ProcessCustomMenuOptions(CMenuManager* _this, int type, bool enter, bool del , int arrows, int arrows2) {
        if (_this->m_nCurrentMenuPage < MENUPAGE_12)
            return;
        
        switch (type) {
        case CUSTOMMENUACTION_SFX:
            gameSettings.sfxVolume += arrows | arrows2;
            gameSettings.sfxVolume = Clamp(gameSettings.sfxVolume, 0, 126);
            gameSettings.Save();
            break;
        case CUSTOMMENUACTION_MUSIC:
            gameSettings.musicVolume += arrows | arrows2;
            gameSettings.musicVolume = Clamp(gameSettings.musicVolume, 0, 126);
            gameSettings.Save();
            break;
        case CUSTOMMENUACTION_CHANGERES:
            if (arrows < 0)
                currentScreenResIndex--;
            else if (arrows > 0)
                currentScreenResIndex++;
            
            currentScreenResIndex = ClampInverse(currentScreenResIndex, 0, listOfRes.size() - 1);
            
            if (enter) {
                gameSettings.screenWidth = listOfRes.at(currentScreenResIndex).w;
                gameSettings.screenHeight = listOfRes.at(currentScreenResIndex).h;
                gameSettings.Save();
                cDMAVideo::ResetVideoDevice();
                previousScreenResIndex = currentScreenResIndex;
            }
            break;
        case CUSTOMMENUACTION_SCREENTYPE:
            if (arrows > 0 || arrows < 0) 
                gameSettings.screenType ^= 1;

            if (enter) {
                gameSettings.Save();
                cDMAVideo::ResetVideoDevice();
            }
            break;
        case CUSTOMMENUACTION_LIGHTING:
            if (arrows > 0 || arrows < 0 || enter)
                gameSettings.lightingType ^= 1;

            gameSettings.Save();
            break;
        case CUSTOMMENUACTION_LANGUAGE:
            if (arrows < 0)
                currentLanguage--;
            else if (arrows > 0)
                currentLanguage++;

            currentLanguage = ClampInverse(currentLanguage, 0, 4);

            gameSettings.language = lang[currentLanguage];
            gameSettings.Save();
            break;
        case CUSTOMMENUACTION_REDEFINEKEY:
            if (!redefineKey) {
                if (enter) {
                    redefineKey = true;
                    previousMenuItem = _this->m_MenuPages[_this->m_nCurrentMenuPage].currentMenuItem;
                }
                else if (del) {
                    gameSettings.controlKeys[_this->m_MenuPages[_this->m_nCurrentMenuPage].currentMenuItem] = 0;
                    gameSettings.Save();
                }
            }
            break;
        }
    }

    static void ProcessCustomButtonPresses(CMenuManager* _this) {
        unsigned char back = _this->NewKeyState.esc;
        unsigned char enter = _this->NewKeyState.enter;
        unsigned char left = _this->NewKeyState.left;
        unsigned char right = _this->NewKeyState.right;
        unsigned char left2 = _this->OldKeyState.left;
        unsigned char right2 = _this->OldKeyState.right;
        unsigned char del = _this->OldKeyState.del;

        short& currentMenuPage = _this->m_nCurrentMenuPage;
        unsigned short& currentItem = _this->m_MenuPages[currentMenuPage].currentMenuItem;

        switch (currentMenuPage) {
        case MENUPAGE_12:
            _this->m_nCurrentSprite = FE_SPRITE_OPTIONS;
            break;
        case MENUPAGE_13:
            _this->m_nCurrentSprite = FE_SPRITE_REDBAR;

            if (redefineKey) {
                _this->m_MenuPages[_this->m_nCurrentMenuPage].currentMenuItem = previousMenuItem;

#define NOTTHISKEY (i != DIK_DELETE && !(i >= DIK_F1 && i <= DIK_SCROLL) && i != DIK_SUBTRACT && i != DIK_ADD && !(i >= DIK_DECIMAL && i <= DIK_F12))
                for (unsigned int i = DIK_1; i < DIK_F12; i++) {
                    if (newKeys[i] && !oldKeys[i] && NOTTHISKEY) {
                        gameSettings.controlKeys[_this->m_MenuPages[_this->m_nCurrentMenuPage].currentMenuItem] = i;
                        gameSettings.Save();

                        int j = 0;
                        for (auto& it : gameSettings.controlKeys) {
                            if (it == i && j != _this->m_MenuPages[_this->m_nCurrentMenuPage].currentMenuItem) {
                                it = 0;
                                break;
                            }
                            j++;
                        }

                        redefineKey = false;
                        return;
                    }
                }
            }
            break;
        }

        if (back) {
            switch (_this->m_nCurrentMenuPage) {
            case MENUPAGE_12:
                _this->SwitchToNewScreen(MENUPAGE_START_MENU);
                //_this->m_MenuPages[currentMenuPage].currentMenuItem = 1;
                break;
            case MENUPAGE_13:
                if (!redefineKey) {
                    _this->SwitchToNewScreen(MENUPAGE_12);
                    //_this->m_MenuPages[currentMenuPage].currentMenuItem = 0;
                }
                else {
                    redefineKey = false;
                }
                break;
            case MENUPAGE_START_MENU:
            case MENUPAGE_PARENTAL_CONTROL:
            case MENUPAGE_PLAY:
            case MENUPAGE_DEAD:
            case MENUPAGE_AREA_COMPLETE:
            case MENUPAGE_GAME_COMPLETE:
            case MENUPAGE_BONUS_AREA:
            case MENUPAGE_NICE_TRY:
            case MENUPAGE_RESULTS_PLAYER_QUIT:
            case MENUPAGE_VIEW_HIGH_SCORE:
                break;
            default:
                //_this->m_nFrontendState = 1;
                break;
            }
        }
        else if (!redefineKey && (left || left2 || right || right2 || enter || del)) {
            ProcessCustomMenuOptions(_this, _this->m_MenuPages[currentMenuPage].items[currentItem].action, enter, del,
                (left ? -1 : right ? 1 : 0), (left2 ? -1 : right2 ? 1 : 0));
        }

        switch (_this->m_nCurrentMenuPage) {
        case MENUPAGE_12:
            if (_this->m_MenuPages[currentMenuPage].items[currentItem].action != CUSTOMMENUACTION_CHANGERES) {
                if (previousScreenResIndex != currentScreenResIndex) {
                    currentScreenResIndex = previousScreenResIndex;
                }
            }
            if (_this->m_MenuPages[currentMenuPage].items[currentItem].action != CUSTOMMENUACTION_SCREENTYPE) {
                if (start_mode != gameSettings.screenType) {
                    gameSettings.screenType = start_mode;
                }
            }
            break;
        }
    }

    static void DrawBackground(CMenuManager* _this) {
        unsigned char left;
        unsigned char right;
        CMenuManager::FindBackground(_this->m_nCurrentSprite, &left, &right);
        GetD3DDevice()->SetRenderState(D3DRENDERSTATE_VERTEXBLEND, TRUE);
        int offset = (SCREEN_WIDTH - 480.0f * (4.0f / 3.0f)) / 2.0f;
        bool full = GetFrontendMenuManager()->m_nCurrentSprite == FE_SPRITE_CREDITS ||
            _this->m_nCurrentSprite == FE_SPRITE_REDBAR ||
            _this->m_nCurrentSprite == FE_SPRITE_BLUEBAR ||
            _this->m_nCurrentSprite == FE_SPRITE_LOADING ||
            _this->m_nCurrentSprite == FE_SPRITE_HISCORES ||
            _this->m_nCurrentSprite == FE_SPRITE_CREDITS;

#if HD_FRONTEND
        frontendSprites.at(left)->Draw(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, CRGBA(255, 255, 255, 255));

        float x = SCREEN_WIDTH / 2;
        float y = ScaleY(98.0f);
        float w = ScaleX(216.0f);
        float h = ScaleY(216.0f);
        logo.Draw(CRect(x - w, y - h, x + w, y + h), CRGBA(255, 255, 255, 255));
#else
        if (full) {
            frontendSprites.at(left)->Draw(SCREEN_SCALE_X(0.0f), SCREEN_SCALE_Y(0.0f), SCREEN_SCALE_W(MENU_BACKGROUND_WIDTH), SCREEN_SCALE_H(MENU_BACKGROUND_HEIGHT), CRGBA(255, 255, 255, 255));
        }
        else {
            frontendSprites.at(left)->Draw(SCREEN_SCALE_X(0.0f), SCREEN_SCALE_Y(0.0f), SCREEN_SCALE_W(MENU_BACKGROUND_LEFT_WIDTH), SCREEN_SCALE_H(MENU_BACKGROUND_HEIGHT), CRGBA(255, 255, 255, 255));
            frontendSprites.at(right)->Draw(SCREEN_SCALE_X(MENU_BACKGROUND_LEFT_WIDTH), SCREEN_SCALE_Y(0.0f), SCREEN_SCALE_W(MENU_BACKGROUND_RIGHT_WIDTH), SCREEN_SCALE_H(MENU_BACKGROUND_HEIGHT), CRGBA(255, 255, 255, 255));
        }
#endif
    }

    static void DrawSlider(float x, float y, float progress, bool selected) {
        float _x = ScaleXKeepCentered(x);
        float _y = ScaleY(y);
        float _w = ScaleX(4.0f);
        float _h = ScaleY(20.0f);
        float spacing = ScaleX(2.0f);

        int alpha = 100;

        if (selected)
            alpha = 255;

        for (int i = 0; i < 32; i++) {
            CSprite2d::DrawRect(CRect(_x, _y, _x + _w, _y + _h), CRGBA(25, 25, 25, alpha));
                 
            if (i / 32.0f + 1 / 64.0f < progress)
                CSprite2d::DrawRect(CRect(_x, _y, _x + _w, _y + _h), CRGBA((i / 32.0f) * 255, (1.0f - (i / 32.0f)) * 255, 0, alpha));

            _x += spacing + _w;
        }
    }

    FrontendFix() {    
#ifdef _DEBUG
        AllocConsole();
        freopen("conin$", "r", stdin);
        freopen("conout$", "w", stdout);
        freopen("conout$", "w", stderr);
        std::setvbuf(stdout, NULL, _IONBF, 0);
#endif
        plugin::patch::Nop(0x456D0E, 5); // Don't load broken textures
        plugin::Events::initEngineEvent += []() {
            gameSettings.Load();

            for (auto& it : frontendTexFileNames) {
                CSprite2d* sprite = new CSprite2d();
                if (!sprite->SetTextureAsTGA(it.c_str()))
                    continue;

                frontendSprites.push_back(sprite);
            }

#if HD_FRONTEND
            logo.SetTexture("data\\frontend\\gtalogo.dds");
#endif

            if (plugin::IsPluginInstalled("GTA2.WidescreenFix.asi")) {
                wsfix = true;
            }

            SetProcessDPIAware();
            ShowCursor(FALSE);
            SetMousePosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
            SetResolutionsList();

            psxSprites[0].SetTextureFromRawPixels("sfx", 32, 32, (unsigned char*)sfxSprite);
            psxSprites[1].SetTextureFromRawPixels("music", 32, 32, (unsigned char*)cdVolSprite);
            psxSprites[2].SetTextureFromRawPixels("res", 32, 32, (unsigned char*)resSprite);
            psxSprites[3].SetTextureFromRawPixels("screentype", 32, 32, (unsigned char*)screenTypeSprite);
            psxSprites[4].SetTextureFromRawPixels("lang", 32, 32, (unsigned char*)langSprite);
            psxSprites[5].SetTextureFromRawPixels("lighting", 32, 32, (unsigned char*)lightingSprite);
        };

        plugin::Events::d3dResetEvent += []() {
            for (auto& it : frontendSprites) {
                it->Reset();
            }

#if HD_FRONTEND
            logo.Reset();
#endif

            for (auto& it : psxSprites) {
                it.Reset();
            }
        };

        plugin::Events::shutdownEngineEvent += []() {
            for (auto& it : frontendSprites) {
                it->Delete();
                delete it;
            }

#if HD_FRONTEND
            logo.Delete();
#endif

            for (auto& it : psxSprites) {
                it.Delete();
            }

            gameSettings.Save();
        };

        plugin::ThiscallEvent <plugin::AddressList<0x456BC2, plugin::H_CALL, 0x45B47C, plugin::H_CALL>, plugin::PRIORITY_BEFORE, plugin::ArgPickN<CText*, 0>, void(CText*)> onTextLoad;
        onTextLoad += [](CText* t) {
            t->language = gameSettings.language;
        };

        auto openSettings = []() {
            GetFrontendMenuManager()->SwitchToNewScreen(MENUPAGE_12);
        };
        plugin::patch::RedirectCall(0x4599A1, LAMBDA(void, __cdecl, openSettings));

        auto drawBackground = [](CMenuManager* _this, int) {
            DrawBackground(_this);
        };
        plugin::patch::RedirectJump(0x453020, LAMBDA(void, __fastcall, drawBackground, CMenuManager*, int));

        plugin::patch::Nop(0x453807, 25);
        plugin::patch::Set<BYTE>(0x453823, 0xEB);

        // Fix frontend
#define menu_offsetx ((SCREEN_WIDTH / 2) - DEFAULT_SCREEN_WIDTH / 2)
#define menu_offsety ((SCREEN_HEIGHT / 2) - DEFAULT_SCREEN_HEIGHT / 2)
        auto drawText = [](const wchar_t* str, int x, int y, int style, int scale, int unk2, int unk3, int unk4, int unk5) {
            if (!GetGame()) {
                float _x = SCREEN_SCALE_X(x / 16384.0f);
                float _y = SCREEN_SCALE_Y(y / 16384.0f);
        
                if (wsfix) {
                    _x -= menu_offsetx;
                    _y -= menu_offsety;
                }
                float _scale = SCREEN_SCALE_Y(scale / 16384.0f);
        
                x = (int)(_x * 16384);
                y = (int)(_y * 16384);
                scale = (int)(_scale * 16384);
            }
        
            plugin::CallStd<0x4CC100>(str, x, y, style, scale, unk2, unk3, unk4, unk5);
        };
        plugin::patch::RedirectCall(0x453A1D, LAMBDA(void, __stdcall, drawText, const wchar_t* str, int x, int y, int style, int, int, int, int, int));
        plugin::patch::RedirectCall(0x453799, LAMBDA(void, __stdcall, drawText, const wchar_t* str, int x, int y, int style, int, int, int, int, int));
        plugin::patch::RedirectCall(0x4567DC, LAMBDA(void, __stdcall, drawText, const wchar_t* str, int x, int y, int style, int, int, int, int, int));
        plugin::patch::RedirectCall(0x456A17, LAMBDA(void, __stdcall, drawText, const wchar_t* str, int x, int y, int style, int, int, int, int, int));
        plugin::patch::RedirectCall(0x4570A7, LAMBDA(void, __stdcall, drawText, const wchar_t* str, int x, int y, int style, int, int, int, int, int));
        plugin::patch::RedirectCall(0x4580C1, LAMBDA(void, __stdcall, drawText, const wchar_t* str, int x, int y, int style, int, int, int, int, int));
        plugin::patch::RedirectCall(0x458421, LAMBDA(void, __stdcall, drawText, const wchar_t* str, int x, int y, int style, int, int, int, int, int));
        plugin::patch::RedirectCall(0x4BA317, LAMBDA(void, __stdcall, drawText, const wchar_t* str, int x, int y, int style, int, int, int, int, int));
        plugin::patch::RedirectCall(0x4C72D6, LAMBDA(void, __stdcall, drawText, const wchar_t* str, int x, int y, int style, int, int, int, int, int));

        plugin::StdcallEvent <plugin::AddressList<0x457DB2, plugin::H_CALL>, plugin::PRIORITY_AFTER, plugin::ArgPick5N<const wchar_t*, 0, int, 1, int, 2, int, 3, int, 4>, void(const wchar_t* str, int x, int y, int style, int scale)> onFrontendTextDraw;
        onFrontendTextDraw.before += [](const wchar_t* str, int x, int y, int style, int scale) {
            ApplyFontChange(true);
        };
        onFrontendTextDraw.after += [](const wchar_t* str, int x, int y, int style, int scale) {
            ApplyFontChange(false);
        };

        auto drawSprite = [](int id1, int id2, int x, int y, int a5, int scale, int a7, int a8, int a9, char a10, char a11) {
            if (!GetGame()) {
                float _x = SCREEN_SCALE_X(x / 16384.0f);
                float _y = SCREEN_SCALE_Y(y / 16384.0f);
                float _scale = SCREEN_SCALE_Y(scale / 16384.0f);
        
                if (wsfix) {
                    _x -= menu_offsetx;
                    _y -= menu_offsety;
                }
        
                x = (int)(_x * 16384);
                y = (int)(_y * 16384);
                scale = (int)(_scale * 16384);
            }
        
            plugin::CallStd<0x4CBA50>(id1, id2, x, y, a5, scale, a7, a8, a9, a10, a11);
        };
        plugin::patch::RedirectCall(0x453705, LAMBDA(void, __stdcall, drawSprite, int id1, int id2, int x, int y, int a5, int a6, int a7, int a8, int a9, char a10, char a11));
        plugin::patch::RedirectCall(0x453753, LAMBDA(void, __stdcall, drawSprite, int id1, int id2, int x, int y, int a5, int a6, int a7, int a8, int a9, char a10, char a11));
        plugin::patch::RedirectCall(0x4582EE, LAMBDA(void, __stdcall, drawSprite, int id1, int id2, int x, int y, int a5, int a6, int a7, int a8, int a9, char a10, char a11));
        //plugin::patch::RedirectCall(0x4B9424, LAMBDA(void, __stdcall, drawSprite, int id1, int id2, int x, int y, int a5, int a6, int* a7, int a8, int a9, char a10, char a11));
        //plugin::patch::RedirectCall(0x4C720D, LAMBDA(void, __stdcall, drawSprite, int id1, int id2, int x, int y, int a5, int a6, int* a7, int a8, int a9, char a10, char a11));
        //plugin::patch::RedirectCall(0x4C74E5, LAMBDA(void, __stdcall, drawSprite, int id1, int id2, int x, int y, int a5, int a6, int* a7, int a8, int a9, char a10, char a11));
        //plugin::patch::RedirectCall(0x4C8346, LAMBDA(void, __stdcall, drawSprite, int id1, int id2, int x, int y, int a5, int a6, int* a7, int a8, int a9, char a10, char a11));
        //plugin::patch::RedirectCall(0x4C83AD, LAMBDA(void, __stdcall, drawSprite, int id1, int id2, int x, int y, int a5, int a6, int* a7, int a8, int a9, char a10, char a11));

        plugin::ThiscallEvent <plugin::AddressList<0x45A2B1, plugin::H_CALL>, plugin::PRIORITY_BEFORE, plugin::ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onProcessButtonPresses;      
        onProcessButtonPresses.before += [](CMenuManager* _this) {
            memcpy(oldKeys, newKeys, 256);
            memcpy(newKeys, _this->m_nKeys, 256);
        };

        onProcessButtonPresses.after += [](CMenuManager* _this) {
            ProcessCustomButtonPresses(_this);
        };

        plugin::ThiscallEvent <plugin::AddressList<0x456C70, plugin::H_CALL>, plugin::PRIORITY_AFTER, plugin::ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onPopulateFrontend;
        onPopulateFrontend += [](CMenuManager* _this) {
            PopulateCustomFrontend(_this);
        };

        plugin::ThiscallEvent <plugin::AddressList<0x4585B9, plugin::H_JUMP>, plugin::PRIORITY_AFTER, plugin::ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onFrontendDraw;

        onFrontendDraw.before += [](CMenuManager* _this) {
            DoFontPulse();
            switch (_this->m_nCurrentMenuPage) {
            case MENUPAGE_13: 
                DrawRedefineCtrlPage(_this);
                break;
            }
        };

        onFrontendDraw += [](CMenuManager* _this) {
            DrawCustomPages(_this);
        };

        plugin::ThiscallEvent <plugin::AddressList<0x4590D2, plugin::H_CALL>, plugin::PRIORITY_AFTER, plugin::ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onSetNewMenuBackground;
        onSetNewMenuBackground += [](CMenuManager* _this) {
            _this->NewKeyState.Clear();
            memset(_this->m_nKeys, 0, 256);
        };

        plugin::patch::Nop(0x459AF7, 10); // No quit on esc press

        // Skip intro
        plugin::patch::Set<BYTE>(0x4D145C + 2, 0);

        // 30 fps
        //plugin::patch::Set<BYTE>(0x45A472 + 2, 30);

        auto setVideoMode = []() {
            RECT rect;
            GetClientRect(GetDesktopWindow(), &rect);

            rect.left = (rect.right / 2) - (window_width / 2);
            rect.top = (rect.bottom / 2) - (window_height / 2);
            rect.right = window_width;
            rect.bottom = window_height;

            window_x = rect.left;
            window_y = rect.top;
            full_width = rect.right;
            full_height = rect.bottom;

            if (start_mode == 0) {
                SetWindowLongPtr(GetHWnd(), GWL_STYLE, WS_VISIBLE | WS_POPUP);
                SetWindowPos(GetHWnd(), HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, SWP_FRAMECHANGED);
            }
            else if(start_mode == 1) {
                int extraWidth = GetSystemMetrics(SM_CXSIZEFRAME);
                int extraHeight = GetSystemMetrics(SM_CYSIZE) + GetSystemMetrics(SM_CYSIZEFRAME);
                rect.left -= extraWidth / 2;
                rect.top -= extraHeight / 2;
                rect.right += extraWidth;
                rect.bottom += extraHeight;
                SetWindowLong(GetHWnd(), GWL_STYLE, WS_VISIBLE | (WS_OVERLAPPEDWINDOW & ~WS_SIZEBOX));
                SetWindowPos(GetHWnd(), HWND_NOTOPMOST, rect.left, rect.top, rect.right, rect.bottom, 0);
            }
            else {
                assert(start_mode < 1);
            }

            Vid_CloseScreen(GetVideoSystem());
            gbh_SetMode(GetVideoSystem(), GetHWnd(), -2);

            plugin::Call<0x4CC580>();

            if (GetGame()) {
                plugin::Call<0x4CB880>();
            }

            Vid_ClearScreen(GetVideoSystem(), 0, 0, 0, window_x, window_y, window_width, window_height);
            Vid_FlipBuffers(GetVideoSystem());
            Vid_ClearScreen(GetVideoSystem(), 0, 0, 0, window_x, window_y, window_width, window_height);
        };
        plugin::patch::RedirectJump(0x4CB570, LAMBDA(void, __cdecl, setVideoMode));
            
        plugin::patch::Nop(0x4CB73A, 2);

        // CRegistry
        plugin::patch::PutRetn1(0x4CB1D0); // screen
        plugin::patch::PutRetn1(0x4CB290); // screen
        plugin::patch::Nop(0x44BF77, 5); // controls

        plugin::patch::Nop(0x4D0D78, 5);
        plugin::patch::Nop(0x4D0D27, 5);
        plugin::patch::Nop(0x4CC583, 5);
        plugin::patch::Nop(0x4CC58A, 2);

        plugin::patch::Nop(0x4CC67A, 10);

        plugin::patch::Nop(0x4D0A54, 11); // sfxVol
        plugin::patch::Nop(0x4618AE, 11); // sfxVol
        plugin::patch::Nop(0x4D0A70, 11); // cdVol
        plugin::patch::Nop(0x4618CA, 11); // cdVol
};
} frontendFix;
