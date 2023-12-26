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
#include "GameSettings.h"

#include "CChar.h"
#include "cSampleManager.h"
#include "CAudioManager.h"
#include "CHud.h"
#include "cDMAudio.h"
#include "CFileMgr.h"
#include "Main.h"
#include "CStyle.h"
#include "CTextureManager.h"
#include "CGeneral.h"
#include "CFont.h"
#include "tVideo.h"
#include "eScriptCommands.h"
#include "CReplay.h"
#include "CCamera.h"
#include "CReplay.h"

#include "ModuleList.hpp"
#include "GInputAPI.h"

#include "PsxSprites.h"

#include <fcntl.h>
#include <iostream>
#include <array>
#include <format>

#include "..\injector\calling.hpp"
#include "..\injector\utility.hpp"
#include "..\injector\hooking.hpp"
#include "..\injector\assembly.hpp"
#include "..\injector\gvm\gvm.hpp"

std::array<CSprite2d, 7> psxSprites;

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
    "data/frontend/Load00.tga",
    "data/frontend/Load01.tga",
    "data/frontend/Load02.tga",
    "data/frontend/Load03.tga",
};

#if HD_FRONTEND
CSprite2d logo;
#endif

bool full_screen = false;
CGameSettings gameSettings;

std::vector<tResList> listOfRes;
int currentScreenResIndex = 0;
int previousScreenResIndex = 0;
int currentLanguage = 0;
bool redefineKey = false;

char oldKeys[256];
char newKeys[256];
int previousMenuItem = 0;

int pauseMenuSelectedItem = 0;
bool showPauseMenu = false;
bool showPauseGameQuit = false;
int randomLoadScreen = -1;
int frontendSoundTime = 0;

const char lang[6] = { 'e', 'f', 'g', 'i', 's', 'j' };

//CText* BobTheText;

CStyle* FStyle;
CStyle* DefaultStyle;

CTextureManager* FStyleTex;
CTextureManager* DefaultTex;

const char* dinputKeyNamesBob[DIK_MEDIASELECT + 1] = {
    "kunlab", //0
    "kesc", //1
    "k1", //2
    "k2", //3
    "k3", //4
    "k4", //5
    "k5", //6
    "k6", //7
    "k7", //8
    "k8", //9
    "k9", //10
    "k0", //11
    "kminus", //12
    "kequals", //13
    "kback", //14
    "ktab", //15
    "kq", //16
    "kw", //17
    "ke", //18
    "kr", //19
    "kt", //20
    "ky", //21
    "ku", //22
    "ki", //23
    "ko", //24
    "kp", //25
    "klbrack", //26
    "krbrack", //27
    "kreturn", //28
    "klctrl", //29
    "ka", //30
    "ks", //31
    "kd", //32
    "kf", //33
    "kg", //34
    "kh", //35
    "kj", //36
    "kk", //37
    "kl", //38
    "ksemico", //39
    "kapostrophe", //40
    "kgrave", //41
    "klshift", //42
    "kbslash", //43
    "kz", //44
    "kx", //45
    "kc", //46
    "kv", //47
    "kb", //48
    "kn", //49
    "km", //50
    "kcomma", //51
    "kperiod", //52
    "kslash", //53
    "krshift", //54
    "kmultiply", //55
    "klmenu", //56
    "kspace", //57
    "kcapital", //58
    "kf1", //59
    "kf2", //60
    "kf3", //61
    "kf4", //62
    "kf5", //63
    "kf6", //64
    "kf7", //65
    "kf8", //66
    "kf9", //67
    "kf10", //68
    "knumlck", //69
    "kscroll", //70
    "knp7", //71
    "knp8", //72
    "knp9", //73
    "ksubtract", //74
    "knp4", //75
    "knp5", //76
    "knp6", //77
    "kadd", //78
    "knp1", //79
    "knp2", //80
    "knp3", //81
    "knp0", //82
    "kdecimal", //83
    "kunk", //84
    "kunk", //85
    "koem_102", //86
    "kf11", //87
    "kf12", //88
    "kunk", //89
    "kunk", //90
    "kunk", //91
    "kunk", //92
    "kunk", //93
    "kunk", //94
    "kunk", //95
    "kunk", //96
    "kunk", //97
    "kunk", //98
    "kunk", //99
    "kf13", //100
    "kf14", //101
    "kf15", //102
    "kunk", //103
    "kunk", //104
    "kunk", //105
    "kunk", //106
    "kunk", //107
    "kunk", //108
    "kunk", //109
    "kunk", //110
    "kunk", //111
    "kkana", //112
    "kunk", //113
    "kunk", //114
    "kabnt_c1", //115
    "kunk", //116
    "kunk", //117
    "kunk", //118
    "kunk", //119
    "kunk", //120
    "kconvert", //121
    "kunk", //122
    "knoconvert", //123
    "kunk", //124
    "kyen", //125
    "kabnt_c2", //126
    "kunk", //127
    "kunk", //128
    "kunk", //129
    "kunk", //130
    "kunk", //131
    "kunk", //132
    "kunk", //133
    "kunk", //134
    "kunk", //135
    "kunk", //136
    "kunk", //137
    "kunk", //138
    "kunk", //139
    "kunk", //140
    "knp=", //141
    "kunk", //142
    "kunk", //143
    "kprevtrack", //144
    "kat", //145
    "kcolon", //146
    "kunderline", //147
    "kkanji", //148
    "kstop", //149
    "kax", //150
    "kunlabeled", //151
    "kunk", //152
    "knexttrack", //153
    "kunk", //154
    "kunk", //155
    "knpent", //156
    "krctrl", //157
    "kunk", //158
    "kunk", //159
    "kmute", //160
    "kcalculator", //161
    "kplaypause", //162
    "kunk", //163
    "kmediastop", //164
    "kunk", //165
    "kunk", //166
    "kunk", //167
    "kunk", //168
    "kunk", //169
    "kunk", //170
    "kunk", //171
    "kunk", //172
    "kunk", //173
    "kvolumedown", //174
    "kunk", //175
    "kvolumeup", //176
    "kunk", //177
    "kwebhome", //178
    "knp,", //179
    "kunk", //180
    "kdivide", //181
    "kunk", //182
    "ksysrq", //183
    "krmenu", //184
    "kunk", //185
    "kunk", //186
    "kunk", //187
    "kunk", //188
    "kunk", //189
    "kunk", //190
    "kunk", //191
    "kunk", //192
    "kunk", //193
    "kunk", //194
    "kunk", //195
    "kunk", //196
    "kpause", //197
    "kunk", //198
    "khome", //199
    "kup", //200
    "kprior", //201
    "kunk", //202
    "kleft", //203
    "kunk", //204
    "kright", //205
    "kunk", //206
    "kend", //207
    "kdown", //208
    "knext", //209
    "kinsert", //210
    "kdelete", //211
    "kunk", //212
    "kunk", //213
    "kunk", //214
    "kunk", //215
    "kunk", //216
    "kunk", //217
    "kunk", //218
    "klwin", //219
    "krwin", //220
    "kapps", //221
    "kpower", //222
    "ksleep", //223
    "kunk", //224
    "kunk", //225
    "kunk", //226
    "kwake", //227
    "kunk", //228
    "kwebsearch", //229
    "kwebfavorites", //230
    "kwebrefresh", //231
    "kwebstop", //232
    "kwebforward", //233
    "kwebback", //234
    "kmycomputer", //235
    "kmail", //236
    "kmediaselect", //237
};

const wchar_t* dinputKeyNames[DIK_MEDIASELECT + 1 + 5] = {
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
    L"LCTRL", //29
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
    L"SLASH", //53
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
    L"NUMPADENTER", //156
    L"RCTRL", //157
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
    L"VOLUMEUP", //176
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
    L"MYCOMPUTER", //235
    L"MAIL", //236
    L"MEDIASELECT", //237
    L"LMB",
    L"RMB",
    L"MMB",
    L"WHEELUP",
    L"WHEELDN",
};

const int dinputAllowedKeys[] = {
    DIK_1,          // 0x02
    DIK_2,          // 0x03
    DIK_3,          // 0x04
    DIK_4,          // 0x05
    DIK_5,          // 0x06
    DIK_6,          // 0x07
    DIK_7,          // 0x08
    DIK_8,          // 0x09
    DIK_9,          // 0x0A
    DIK_0,          // 0x0B
    DIK_MINUS,      // 0x0C
    DIK_EQUALS,     // 0x0D
    DIK_BACK,       // 0x0E
    DIK_TAB,        // 0x0F
    DIK_Q,          // 0x10
    DIK_W,          // 0x11
    DIK_E,          // 0x12
    DIK_R,          // 0x13
    DIK_T,          // 0x14
    DIK_Y,          // 0x15
    DIK_U,          // 0x16
    DIK_I,          // 0x17
    DIK_O,          // 0x18
    DIK_P,          // 0x19
    DIK_LBRACKET,   // 0x1A
    DIK_RBRACKET,   // 0x1B
    DIK_RETURN,     // 0x1C
    DIK_LCONTROL,   // 0x1D
    DIK_A,          // 0x1E
    DIK_S,          // 0x1F
    DIK_D,          // 0x20
    DIK_F,          // 0x21
    DIK_G,          // 0x22
    DIK_H,          // 0x23
    DIK_J,          // 0x24
    DIK_K,          // 0x25
    DIK_L,          // 0x26
    DIK_SEMICOLON,  // 0x27
    DIK_APOSTROPHE, // 0x28
    DIK_GRAVE,      // 0x29
    DIK_LSHIFT,     // 0x2A
    DIK_BACKSLASH,  // 0x2B
    DIK_Z,          // 0x2C
    DIK_X,          // 0x2D
    DIK_C,          // 0x2E
    DIK_V,          // 0x2F
    DIK_B,          // 0x30
    DIK_N,          // 0x31
    DIK_M,          // 0x32
    DIK_COMMA,      // 0x33
    DIK_PERIOD,     // 0x34
    DIK_SLASH,      // 0x35
    DIK_RSHIFT,     // 0x36
    DIK_MULTIPLY,   // 0x37
    DIK_LMENU,      // 0x38
    DIK_SPACE,      // 0x39
    DIK_CAPITAL,    // 0x3A
    DIK_NUMPAD7,    // 0x47
    DIK_NUMPAD8,    // 0x48
    DIK_NUMPAD9,    // 0x49
    DIK_NUMPAD4,    // 0x4B
    DIK_NUMPAD5,    // 0x4C
    DIK_NUMPAD6,    // 0x4D
    DIK_NUMPAD1,    // 0x4F
    DIK_NUMPAD2,    // 0x50
    DIK_NUMPAD3,    // 0x51
    DIK_NUMPAD0,    // 0x52
    DIK_RCONTROL,   // 0x9D
    DIK_RMENU,      // 0xB8
    DIK_UP,         // 0xC8
    DIK_LEFT,       // 0xCB
    DIK_RIGHT,      // 0xCD
    DIK_DOWN,       // 0xD0
};

const unsigned int dinputAllowedKeysLength = sizeof(dinputAllowedKeys) / sizeof(int);

static int menuColorPulse = 255;

injector::hook_back<void(__stdcall*)(int, int, int, int, int, int, int, int, int, char, char)> hbDrawSprite;
injector::hook_back<void(__stdcall*)(const wchar_t* str, int x, int y, int style, int scale, const int* mode, int palette, bool enableAlpha, int alpha)> hbMenuPrintString;

void CText__Load(CText* text, bool bob) {
    char buff[64];
    sprintf(buff, bob ? "data\\bob_%c.gxt" : "data\\%c.gxt", text->language);
    CFileMgr::OpenFile(buff);

    CFileMgr::Seek(6);

    tChunkHeader gxtHeader = {};
    for (int i = sizeof(tChunkHeader); CFileMgr::ReadLine(&gxtHeader, i); i = sizeof(tChunkHeader)) {
        if (gxtHeader.size)
            text->LoadChunk(gxtHeader.code, gxtHeader.size);
    }
    CFileMgr::CloseFile();

    text->Update(text->data.chars);
}

LPDIRECTINPUTA gDInput = NULL;
LPDIRECTINPUTDEVICEA gMouse;

CMouseState OldMouseState;
CMouseState NewMouseState;

bool ShowMouse = false;
float PrevMousePosX = 0.0f;
float PrevMousePosY = 0.0f;
float MousePosX = 0.0f;
float MousePosY = 0.0f;
bool UsingGamePad = false;

short hoverMenuItem = -1;
bool wasMouseClick = false;
char arrowClick = 0;

class FrontendFixII {
public:
    static HRESULT InitMouse() {
        if (gDInput || gMouse)
            return -1;

        HRESULT hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&gDInput, NULL);
        if (FAILED(hr))
            return hr;

        hr = gDInput->CreateDevice(GUID_SysMouse, &gMouse, NULL);
        if (FAILED(hr))
            return hr;

        hr = gMouse->SetDataFormat(&c_dfDIMouse);
        if (FAILED(hr))
            return hr;

        hr = gMouse->SetCooperativeLevel(GetHWnd(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
        if (FAILED(hr))
            return hr;

        gMouse->Acquire();

        return hr;
    }

    static void UpdateMouse() {
        if (!gMouse)
            return;

        DIMOUSESTATE dims = {};
        HRESULT hr = gMouse->GetDeviceState(sizeof(DIMOUSESTATE), &dims);

        if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
            gMouse->Acquire();

        OldMouseState = NewMouseState;
        NewMouseState.buttons[0] = (dims.rgbButtons[0] & 0x80) ? 255 : 0;
        NewMouseState.buttons[1] = (dims.rgbButtons[1] & 0x80) ? 255 : 0;
        NewMouseState.buttons[2] = (dims.rgbButtons[2] & 0x80) ? 255 : 0;
        NewMouseState.x = static_cast<float>(dims.lX) / 255.0f;
        NewMouseState.y = static_cast<float>(dims.lY) / 255.0f;

        NewMouseState.buttons[3] = dims.lZ > 0 ? 255 : 0;
        NewMouseState.buttons[4] = dims.lZ < 0 ? 255 : 0;

        if (settings.ClipMousePosition) {
            RECT rect;
            GetWindowRect(GetHWnd(), &rect);
            if (GetActiveWindow() == GetHWnd())
                ClipCursor(&rect);
        }

        POINT p;
        if (GetActiveWindow() == GetHWnd()) {
            if (GetCursorPos(&p)) {
                if (ScreenToClient(GetHWnd(), &p)) {
                    PrevMousePosX = MousePosX;
                    PrevMousePosY = MousePosY;
                    MousePosX = static_cast<float>(p.x);
                    MousePosY = static_cast<float>(p.y);
                }
            }
        }
    }
    
    static void CentreCursor() {
        RECT rect;
        GetWindowRect(GetHWnd(), &rect);

        int x = rect.left + (rect.right - rect.left) / 2;
        int y = rect.top + (rect.bottom - rect.top) / 2;

        SetCursorPos(x, y);
    }

    static void ShutdownMouse() {
        if (gMouse) {
            gMouse->Unacquire();
            gMouse->Release();
            gMouse = nullptr;
        }

        if (gDInput) {
            gDInput->Release();
            gDInput = nullptr;
        }
    }

    static void DoColorPulse() {
        static float pulse = 1.0f;
        static bool reverse = false;

        pulse += reverse ? -0.03f : 0.03f;

        if (pulse >= 1.0f)
            reverse = true;

        if (pulse <= 0.4f)
            reverse = false;

        pulse = Clamp(pulse, 0.0f, 1.0f);
        menuColorPulse = 255 * pulse;
    }

    static void ApplyFontChange(bool pulse) {
        plugin::patch::SetUChar(0x4CC534 + 1, pulse ? menuColorPulse : 255);
    }

    static void ApplySpriteChange(bool pulse) {
        plugin::patch::SetUChar(0x4CBD4C + 1, pulse ? menuColorPulse : 255);
    }

    static void ApplyStyleSwitch(bool frontend) {
        if (!frontend) {
            *gCurrentStyle = DefaultStyle;
            *TextureManager = DefaultTex;
        }
        else {
            DefaultStyle = *gCurrentStyle;
            *gCurrentStyle = FStyle;

            DefaultTex = *TextureManager;
            *TextureManager = FStyleTex;
        }

        CStyle::SetFontStyles((*gCurrentStyle)->m_pFontBase->fontCount);
    }

    static void SmoothRect(CRect const& rect, CRGBA const& c0, CRGBA const& c1, CRGBA const& c2, CRGBA const& c3) {
        CSprite2d::SetVertices(rect, c0, c1, c2, c3);

        RenderStateSet(D3DRENDERSTATE_TEXTUREHANDLE, NULL);

        RenderStateSet(D3DRENDERSTATE_SHADEMODE, (void*)D3DSHADE_GOURAUD);
        RenderStateSet(D3DRENDERSTATE_SHADEMODE, (void*)D3DSHADE_GOURAUD);
        RenderStateSet(D3DRENDERSTATE_ZENABLE, FALSE);
        RenderStateSet(D3DRENDERSTATE_ZWRITEENABLE, FALSE);
        RenderStateSet(D3DRENDERSTATE_VERTEXBLEND, (void*)(c0.a != 255));

        RenderPrimitive(D3DPT_TRIANGLEFAN, CSprite2d::ms_aVertices, 4);

        RenderStateSet(D3DRENDERSTATE_ZENABLE, (void*)TRUE);
        RenderStateSet(D3DRENDERSTATE_ZWRITEENABLE, (void*)TRUE);
        RenderStateSet(D3DRENDERSTATE_SHADEMODE, (void*)D3DSHADE_GOURAUD);
    }

    static CRGBA GetColFromRatio(float ratio, unsigned char alpha) {
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
        CSprite2d::DrawRect(CRect(ScaleXKeepCentered(20.0f - 2.0f), ScaleY(REDEFINE_TEXT_Y - 2.0f - 2.0f), ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 20.0f + 2.0f), ScaleY(REDEFINE_TEXT_Y + (REDEFINE_RECT_SIZE * 11) + 2.0f + 2.0f)), CRGBA(125, 125, 125, 255));
        CSprite2d::DrawRect(CRect(ScaleXKeepCentered(20.0f), ScaleY(REDEFINE_TEXT_Y - 2.0f), ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 20.0f), ScaleY(REDEFINE_TEXT_Y + (REDEFINE_RECT_SIZE * 11) + 2.0f)), CRGBA(0, 0, 32, 255));

        wchar_t* str = L"SETUP CONTROLS";//BobTheText->Get("contcur");
        //str = UpperCase(str);
        CFont::PrintString(str, ScaleXKeepCentered(40) * 16384, ScaleY(10) * 16384, _this->m_nFontStyle + 9, ScaleY(1.0f) * 16384, 8, 5, 0, 0);

        CFont::PrintString(L"ACTION", ScaleXKeepCentered(30) * 16384, ScaleY(128) * 16384, _this->m_nFontStyle, ScaleY(0.8f) * 16384, 2, 0, 0, 0);
        
        int s = (CFont::GetStringWidth(L"KEY", _this->m_nFontStyle) * (0.8f));
        CFont::PrintString(L"KEY", ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 30 - s) * 16384, ScaleY(128) * 16384, _this->m_nFontStyle, ScaleY(0.8f) * 16384, 2, 0, 0, 0);

        std::vector<const wchar_t*> controls = {
            L"FORWARD", //"forward",
            L"BACKWARD", //"back",
            L"LEFT", //"left",
            L"RIGHT", //"right",
            L"ATTACK", //"attack",
            L"ENTER/EXIT", //"enter",
            L"HANDBRAKE/JUMP", //"jump",
            L"PREVIOUS WEAPON", //"prev",
            L"NEXT WEAPON", //"next",
            L"SPECIAL", //"special",
            L"SPECIAL 2", //"spec2",
        };

        int i = 0;
        for (auto& it : controls) {
            float x = (float)_this->m_MenuPages[_this->m_nCurrentMenuPage].items[i].x;
            float y = (float)_this->m_MenuPages[_this->m_nCurrentMenuPage].items[i].y;

            //wchar_t* s = UpperCase(BobTheText->Get(it));
            CFont::PrintString(it, ScaleXKeepCentered(x) * 16384, ScaleY(y - 1.0f) * 16384, _this->m_nFontStyle, ScaleY(1.0f) * 16384, 2, 0, 0, 0);
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

    static void PopulateCustomMenu(CMenuManager* _this) {
#define MENUPAGE_12_FIRST_ENTRY_Y (160)
#define MENUPAGE_12_FIRST_SPACING_Y (32)

        // MENUPAGE_12
        //_this->m_MenuPages[MENUPAGE_12].currentMenuItem = 0;
        _this->m_MenuPages[MENUPAGE_12].initialMenuItem = 0;

        _this->m_MenuPages[MENUPAGE_12].numMenuItems = 9;
        // red controls
        _this->m_MenuPages[MENUPAGE_12].items[0].action = MENUACTION_CHANGEPAGE;
        _this->m_MenuPages[MENUPAGE_12].items[0].x = 300;
        _this->m_MenuPages[MENUPAGE_12].items[0].y = MENUPAGE_12_FIRST_ENTRY_Y;

        wchar_t* str = L"SETUP CONTROLS";//BobTheText->Get("contcur");
        str = UpperCase(str);
        wcscpy(_this->m_MenuPages[MENUPAGE_12].items[0].str, str);
        _this->m_MenuPages[MENUPAGE_12].items[0].targetPage = MENUPAGE_13;

        // res
        _this->m_MenuPages[MENUPAGE_12].items[1].action = CUSTOMMENUACTION_CHANGERES;
        _this->m_MenuPages[MENUPAGE_12].items[1].x = 300;
        _this->m_MenuPages[MENUPAGE_12].items[1].y = MENUPAGE_12_FIRST_ENTRY_Y + (MENUPAGE_12_FIRST_SPACING_Y * 1);

        wcscpy(_this->m_MenuPages[MENUPAGE_12].items[1].str, L"");
        _this->m_MenuPages[MENUPAGE_12].items[1].targetPage = MENUACTION_NONE;

        // screen type
        _this->m_MenuPages[MENUPAGE_12].items[2].action = CUSTOMMENUACTION_SCREENTYPE;
        _this->m_MenuPages[MENUPAGE_12].items[2].x = 300;
        _this->m_MenuPages[MENUPAGE_12].items[2].y = MENUPAGE_12_FIRST_ENTRY_Y + (MENUPAGE_12_FIRST_SPACING_Y * 2);

        wcscpy(_this->m_MenuPages[MENUPAGE_12].items[2].str, L"");
        _this->m_MenuPages[MENUPAGE_12].items[2].targetPage = MENUACTION_NONE;

        // sfx
        _this->m_MenuPages[MENUPAGE_12].items[3].action = CUSTOMMENUACTION_SFX;
        _this->m_MenuPages[MENUPAGE_12].items[3].x = 300;
        _this->m_MenuPages[MENUPAGE_12].items[3].y = MENUPAGE_12_FIRST_ENTRY_Y + (MENUPAGE_12_FIRST_SPACING_Y * 3);

        wcscpy(_this->m_MenuPages[MENUPAGE_12].items[3].str, L"");
        _this->m_MenuPages[MENUPAGE_12].items[3].targetPage = MENUACTION_NONE;

        // music
        _this->m_MenuPages[MENUPAGE_12].items[4].action = CUSTOMMENUACTION_MUSIC;
        _this->m_MenuPages[MENUPAGE_12].items[4].x = 300;
        _this->m_MenuPages[MENUPAGE_12].items[4].y = MENUPAGE_12_FIRST_ENTRY_Y + (MENUPAGE_12_FIRST_SPACING_Y * 4);

        wcscpy(_this->m_MenuPages[MENUPAGE_12].items[4].str, L"");
        _this->m_MenuPages[MENUPAGE_12].items[4].targetPage = MENUACTION_NONE;

        // lighting
        _this->m_MenuPages[MENUPAGE_12].items[5].action = CUSTOMMENUACTION_LIGHTING;
        _this->m_MenuPages[MENUPAGE_12].items[5].x = 300;
        _this->m_MenuPages[MENUPAGE_12].items[5].y = MENUPAGE_12_FIRST_ENTRY_Y + (MENUPAGE_12_FIRST_SPACING_Y * 5);

        wcscpy(_this->m_MenuPages[MENUPAGE_12].items[5].str, L"");
        _this->m_MenuPages[MENUPAGE_12].items[5].targetPage = MENUACTION_NONE;

        // lang
        _this->m_MenuPages[MENUPAGE_12].items[6].action = CUSTOMMENUACTION_LANGUAGE;
        _this->m_MenuPages[MENUPAGE_12].items[6].x = 300;
        _this->m_MenuPages[MENUPAGE_12].items[6].y = MENUPAGE_12_FIRST_ENTRY_Y + (MENUPAGE_12_FIRST_SPACING_Y * 6);

        wcscpy(_this->m_MenuPages[MENUPAGE_12].items[6].str, L"");
        _this->m_MenuPages[MENUPAGE_12].items[6].targetPage = MENUACTION_NONE;

        // lang
        _this->m_MenuPages[MENUPAGE_12].items[7].action = CUSTOMMENUACTION_MOUSECONTROLLEDHEADING;
        _this->m_MenuPages[MENUPAGE_12].items[7].x = 300;
        _this->m_MenuPages[MENUPAGE_12].items[7].y = MENUPAGE_12_FIRST_ENTRY_Y + (MENUPAGE_12_FIRST_SPACING_Y * 7);

        wcscpy(_this->m_MenuPages[MENUPAGE_12].items[7].str, L"MOUSE CONTROL:");
        _this->m_MenuPages[MENUPAGE_12].items[7].targetPage = MENUACTION_NONE;

        // restore default
        _this->m_MenuPages[MENUPAGE_12].items[8].action = CUSTOMMENUACTION_RESTOREDEFAULT;
        _this->m_MenuPages[MENUPAGE_12].items[8].x = 300;
        _this->m_MenuPages[MENUPAGE_12].items[8].y = MENUPAGE_12_FIRST_ENTRY_Y + (MENUPAGE_12_FIRST_SPACING_Y * 8);

        wcscpy(_this->m_MenuPages[MENUPAGE_12].items[8].str, L"RESTORE DEFAULT");
        _this->m_MenuPages[MENUPAGE_12].items[8].targetPage = MENUACTION_NONE;

        // MENUPAGE_13
        _this->m_MenuPages[MENUPAGE_13].numMenuItems = 11;
        _this->m_MenuPages[MENUPAGE_13].initialMenuItem = 0;

        // controls
        for (int i = 0; i < 11; i++) {
            _this->m_MenuPages[MENUPAGE_13].items[i].action = CUSTOMMENUACTION_REDEFINEKEY;
            _this->m_MenuPages[MENUPAGE_13].items[i].x = 30;
            _this->m_MenuPages[MENUPAGE_13].items[i].y = REDEFINE_TEXT_Y + (REDEFINE_TEXT_SPACING * i);

            wcscpy(_this->m_MenuPages[MENUPAGE_13].items[i].str, L"");
            _this->m_MenuPages[MENUPAGE_13].items[i].targetPage = MENUACTION_NONE;
        }
    }

    static void DrawCustomPages(CMenuManager* _this) {
        if (_this->m_nCurrentMenuPage < MENUPAGE_12)
            return;

        const int numItems = _this->m_MenuPages[_this->m_nCurrentMenuPage].numMenuItems;
        for (int i = 0; i < numItems; i++) {
            float x = static_cast<float>(_this->m_MenuPages[_this->m_nCurrentMenuPage].items[i].x);
            float y = static_cast<float>(_this->m_MenuPages[_this->m_nCurrentMenuPage].items[i].y);

            bool selectedItem = (i == _this->m_MenuPages[_this->m_nCurrentMenuPage].currentMenuItem) ? 1 : 0;
            int fontStyle = _this->m_nFontStyle + selectedItem;

            wchar_t* ogStr = _this->m_MenuPages[_this->m_nCurrentMenuPage].items[i].str;
            if (!ogStr)
                ogStr = L" ";
            float leftStringWidth = static_cast<float>(CFont::GetStringWidth(ogStr, fontStyle));

            ApplyFontChange(selectedItem);

#define MENUPAGE_12_ICON_W (32.0f)
#define MENUPAGE_12_ICON_H (30.0f)

#define MENUPAGE_12_ICON_OFFSET_Y (-4.0f)
            if (leftStringWidth <= 1.0f)
                leftStringWidth = 34.0f;

            leftStringWidth += 16.0f;

            float xRight = x + leftStringWidth;
            int rightStringWidth = 0;
            switch (_this->m_MenuPages[_this->m_nCurrentMenuPage].items[i].action) {
            case CUSTOMMENUACTION_SFX:
                psxSprites[PSXSPRITE_SFX].Draw(ScaleXKeepCentered(x), ScaleY(y + MENUPAGE_12_ICON_OFFSET_Y), ScaleX(MENUPAGE_12_ICON_W), ScaleY(MENUPAGE_12_ICON_H), CRGBA(255, !selectedItem ? 255 : 0, !selectedItem ? 255 : 0, selectedItem ? menuColorPulse : 255));
                DrawSlider(ScaleXKeepCentered(xRight), ScaleY(y + 1.0f), gameSettings.sfxVolume / 127.0f, selectedItem);
                rightStringWidth = 190;
                break;
            case CUSTOMMENUACTION_MUSIC:
                psxSprites[PSXSPRITE_MUSIC].Draw(ScaleXKeepCentered(x), ScaleY(y + MENUPAGE_12_ICON_OFFSET_Y), ScaleX(MENUPAGE_12_ICON_W), ScaleY(MENUPAGE_12_ICON_H), CRGBA(255, !selectedItem ? 255 : 0, !selectedItem ? 255 : 0, selectedItem ? menuColorPulse : 255));
                DrawSlider(ScaleXKeepCentered(xRight), ScaleY(y + 1.0f), gameSettings.musicVolume / 127.0f, selectedItem);
                rightStringWidth = 190;
                break;
            case CUSTOMMENUACTION_CHANGERES:  
                psxSprites[PSXSPRITE_RES].Draw(ScaleXKeepCentered(x), ScaleY(y + MENUPAGE_12_ICON_OFFSET_Y), ScaleX(MENUPAGE_12_ICON_W), ScaleY(MENUPAGE_12_ICON_H), CRGBA(255, !selectedItem ? 255 : 0, !selectedItem ? 255 : 0, selectedItem ? menuColorPulse : 255));
                CFont::PrintString(listOfRes.at(currentScreenResIndex).str, ScaleXKeepCentered(xRight) * 16384, ScaleY(y) * 16384, fontStyle, ScaleY(1.0f) * 16384, 2, 0, 0, 0);
                rightStringWidth = CFont::GetStringWidth(listOfRes.at(currentScreenResIndex).str, fontStyle);
                break;
            case CUSTOMMENUACTION_SCREENTYPE: {
                psxSprites[PSXSPRITE_SCREENTYPE].Draw(ScaleXKeepCentered(x), ScaleY(y + MENUPAGE_12_ICON_OFFSET_Y), ScaleX(MENUPAGE_12_ICON_W), ScaleY(MENUPAGE_12_ICON_H), CRGBA(255, !selectedItem ? 255 : 0, !selectedItem ? 255 : 0, selectedItem ? menuColorPulse : 255));

                wchar_t* str = nullptr;
                switch (gameSettings.screenType) {
                case 0:
                    str = L"FULLSCREEN";//BobTheText->Get("fullscr");
                    break;
                case 1:
                    str = L"WINDOWED";//BobTheText->Get("window");
                    break;
                case 2:
                    str = L"BORDERLESS WINDOWED";
                    break;
                }

                //str = UpperCase(str);

                float scale = 1.0f;
                if (CFont::GetStringWidth(str, fontStyle) > 320)
                    scale = 0.88f;

                CFont::PrintString(str, ScaleXKeepCentered(xRight) * 16384, ScaleY(y) * 16384, fontStyle, ScaleY(scale) * 16384, 2, 0, 0, 0);
                rightStringWidth = CFont::GetStringWidth(str, fontStyle) * scale;
            } break;
            case CUSTOMMENUACTION_LIGHTING: {
                psxSprites[PSXSPRITE_LGHT].Draw(ScaleXKeepCentered(x), ScaleY(y + MENUPAGE_12_ICON_OFFSET_Y), ScaleX(MENUPAGE_12_ICON_W), ScaleY(MENUPAGE_12_ICON_H), CRGBA(255, !selectedItem ? 255 : 0, !selectedItem ? 255 : 0, selectedItem ? menuColorPulse : 255));

                wchar_t* str = nullptr;
                switch (gameSettings.lightingType) {
                case 1:
                    str = L"DUSK";//BobTheText->Get("dusk");
                    break;
                default:
                    str = L"NOON";//BobTheText->Get("noon");
                    break;
                }
                //str = UpperCase(str);

                CFont::PrintString(str, ScaleXKeepCentered(xRight) * 16384, ScaleY(y) * 16384, fontStyle, ScaleY(1.0f) * 16384, selectedItem ? 2 : 8, 13, 0, 0);
                rightStringWidth = CFont::GetStringWidth(str, fontStyle);
            } break;
            case CUSTOMMENUACTION_LANGUAGE: {
                psxSprites[PSXSPRITE_LANG].Draw(ScaleXKeepCentered(x), ScaleY(y + MENUPAGE_12_ICON_OFFSET_Y), ScaleX(MENUPAGE_12_ICON_W), ScaleY(MENUPAGE_12_ICON_H), CRGBA(255, !selectedItem ? 255 : 0, !selectedItem ? 255 : 0, selectedItem ? menuColorPulse : 255));

                wchar_t* str = nullptr;
                switch (gameSettings.language) {
                case 'e':
                    str = L"ENGLISH";//BobTheText->Get("english");
                    break;
                case 'f':
                    str = L"FRENCH";//BobTheText->Get("french");
                    break;
                case 'g':
                    str = L"GERMAN";//BobTheText->Get("german");
                    break;
                case 'i':
                    str = L"ITALIAN";//BobTheText->Get("italian");
                    break;
                case 's':
                    str = L"SPANISH";//BobTheText->Get("spanish");
                    break;
                case 'j':
                    str = L"JAPANESE";//BobTheText->Get("japenes");
                    break;
                }

                //str = UpperCase(str);
                CFont::PrintString(str, ScaleXKeepCentered(xRight) * 16384, ScaleY(y) * 16384, fontStyle, ScaleY(1.0f) * 16384, 2, 0, 0, 0);
                rightStringWidth = CFont::GetStringWidth(str, fontStyle);
            } break;
            case CUSTOMMENUACTION_MOUSECONTROLLEDHEADING: {
                wchar_t* str = gameSettings.mouseControlledHeading ? L"YES" : L"NO";
                CFont::PrintString(str, ScaleXKeepCentered(xRight) * 16384, ScaleY(y) * 16384, fontStyle, ScaleY(1.0f) * 16384, 2, 0, 0, 0);
                rightStringWidth = CFont::GetStringWidth(str, fontStyle);
            } break;
            case CUSTOMMENUACTION_REDEFINEKEY: {
                const wchar_t* str = dinputKeyNames[gameSettings.controlKeys[i]];
                
                //str = UpperCase(str);
                static int timer = 0;

                if (redefineKey && selectedItem) {
                    if (_this->m_nTimeInMilliseconds & 8)
                        str = L"???";
                    else
                        str = L" ";
                }

                int fontStyle = _this->m_nFontStyle;
                if (!lstrcmpW(str, L"UNBOUND")) {
                    fontStyle += 1;
                }

                int s = (CFont::GetStringWidth(str, fontStyle) * (0.7f));
                CFont::PrintString(str, ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 30 - s) * 16384, ScaleY(y + 3) * 16384, fontStyle, ScaleY(0.7f) * 16384, 2, 0, 0, 0);
                rightStringWidth = CFont::GetStringWidth(str, fontStyle);
            } break;
            };

            if (_this->m_MenuPages[_this->m_nCurrentMenuPage].items[i].action >= CUSTOMMENUACTION_SFX && 
                _this->m_MenuPages[_this->m_nCurrentMenuPage].items[i].action <= CUSTOMMENUACTION_MOUSECONTROLLEDHEADING &&
                selectedItem) {
                const bool lmb = NewMouseState.buttons[0] && !OldMouseState.buttons[0];

                ApplySpriteChange(true);
                CSprite::DrawSprite(6, 37, (ScaleXKeepCentered(xRight - 10.0f)) * 16384, (ScaleY(y + 11.0f)) * 16384, 0, ScaleY(1.0f) * 16384, 2, 0, 0, 0, 0);
                if (ShowMouse && CheckHover(ScaleXKeepCentered(xRight - 10.0f - 8.0f), ScaleXKeepCentered(xRight - 10.0f + 8.0f), ScaleY(y + 11.0f - 8.0f), ScaleY(y + 11.0f + 8.0f)))
                    arrowClick = -1; wasMouseClick = true;

                CSprite::DrawSprite(6, 38, (ScaleXKeepCentered(xRight + rightStringWidth + 10.0f)) * 16384, (ScaleY(y + 11.0f)) * 16384, 0, ScaleY(1.0f) * 16384, 2, 0, 0, 0, 0);
                if (ShowMouse && CheckHover(ScaleXKeepCentered(xRight + rightStringWidth + 10.0f - 8.0f), ScaleXKeepCentered(xRight + rightStringWidth + 10.0f + 8.0f), ScaleY(y + 11.0f - 8.0f), ScaleY(y + 11.0f + 8.0f)))
                    arrowClick = 1; wasMouseClick = true;

                ApplySpriteChange(false);
            }
        }

        ApplyFontChange(false);

        switch (_this->m_nCurrentMenuPage) {
        case MENUPAGE_13: {
            int selectedItem = (_this->m_MenuPages[_this->m_nCurrentMenuPage].currentMenuItem);
            static float ratio = 0.0f;
            ratio += 0.0125f;

            float y = REDEFINE_TEXT_Y;
            CRGBA c0 = GetColFromRatio(ratio, (unsigned char)150);
            CRGBA c1 = c0;
            CRGBA c2 = CRGBA(255 - c0.b, 255 - c0.g, 255 - c0.r, c0.a);
            CRGBA c3 = CRGBA(c0.b, c0.g, c0.r, c0.a);

            if (ratio > 1.0f) {
                ratio = 0.0f;
            }

            SmoothRect(CRect(ScaleXKeepCentered(22.0f), ScaleY(y + (selectedItem * REDEFINE_TEXT_SPACING)), ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 22.0f), ScaleY(y + REDEFINE_RECT_SIZE + (selectedItem * REDEFINE_TEXT_SPACING))),
                c0, c1, c2, c3);

            const float helpSize = (0.8f);
            if (redefineKey) {
                const wchar_t* str = L"SELECT A NEW CONTROL";
                
                int s = (CFont::GetStringWidth(str, _this->m_nFontStyle) * helpSize) / 2;
                CFont::PrintString(str, ScaleXKeepCentered(320 - s) * 16384, ScaleY(402 * 16384), _this->m_nFontStyle, ScaleY(helpSize) * 16384, 2, 0, 0, 0);

                str = L"FOR THIS ACTION OR";
                s = (CFont::GetStringWidth(str, _this->m_nFontStyle) * helpSize) / 2;
                CFont::PrintString(str, ScaleXKeepCentered(320 - s) * 16384, ScaleY(422 * 16384), _this->m_nFontStyle, ScaleY(helpSize) * 16384, 2, 0, 0, 0);

                str = L"ESC TO CANCEL";
                s = (CFont::GetStringWidth(str, _this->m_nFontStyle) * helpSize) / 2;
                CFont::PrintString(str, ScaleXKeepCentered(320 - s) * 16384, ScaleY(442 * 16384), _this->m_nFontStyle, ScaleY(helpSize) * 16384, 2, 0, 0, 0);
            }
            else {
                const wchar_t* str = L"DELETE TO CLEAR OR";
                int s = (CFont::GetStringWidth(str, _this->m_nFontStyle) * helpSize) / 2;
                CFont::PrintString(str, ScaleXKeepCentered(320 - s) * 16384, ScaleY(402 * 16384), _this->m_nFontStyle, ScaleY(helpSize) * 16384, 2, 0, 0, 0);

                str = L"RETURN TO CHANGE";
                s = (CFont::GetStringWidth(str, _this->m_nFontStyle) * helpSize) / 2;
                CFont::PrintString(str, ScaleXKeepCentered(320 - s) * 16384, ScaleY(422 * 16384), _this->m_nFontStyle, ScaleY(helpSize) * 16384, 2, 0, 0, 0);
            }
        } break;
        };
    }

    static void PlayFrontendSound(CMenuManager* _this, int i) {
        if (frontendSoundTime < _this->m_nTimeInMilliseconds) {
            DMAudio.NewFrontendAudioObject.type = i;
            frontendSoundTime = _this->m_nTimeInMilliseconds + 100;
        }
    }

    static void ProcessCustomMenuOptions(CMenuManager* _this, int type, bool enter, bool del, int arrows, int arrows2) {
        if (_this->m_nCurrentMenuPage < MENUPAGE_12)
            return;

        switch (type) {
        case CUSTOMMENUACTION_SFX:
            gameSettings.sfxVolume += arrows2 * (128 / 32);
            gameSettings.sfxVolume = Clamp(gameSettings.sfxVolume, 0, 127);
            gameSettings.Save();

            if (arrows2 != 0 && gameSettings.sfxVolume > 0 && gameSettings.sfxVolume < 127)
                PlayFrontendSound(_this, 3);
            break;
        case CUSTOMMENUACTION_MUSIC:
            gameSettings.musicVolume += arrows2 * (128 / 32);
            gameSettings.musicVolume = Clamp(gameSettings.musicVolume, 0, 127);
            gameSettings.Save();

            if (arrows2 != 0 && gameSettings.musicVolume > 0 && gameSettings.musicVolume < 127)
                PlayFrontendSound(_this, 3);
            break;
        case CUSTOMMENUACTION_CHANGERES:
            if (arrows < 0) {
                currentScreenResIndex--;
                PlayFrontendSound(_this, 3);
            }
            else if (arrows > 0) {
                currentScreenResIndex++;
                PlayFrontendSound(_this, 3);
            }
            currentScreenResIndex = ClampInverse(currentScreenResIndex, 0, listOfRes.size() - 1);

            if (enter && previousScreenResIndex != currentScreenResIndex) {
                PlayFrontendSound(_this, 5);
                gameSettings.screenWidth = listOfRes.at(currentScreenResIndex).w;
                gameSettings.screenHeight = listOfRes.at(currentScreenResIndex).h;
                gameSettings.Save();
                ResetVideoDevice();
                previousScreenResIndex = currentScreenResIndex;
            }

            break;
        case CUSTOMMENUACTION_SCREENTYPE:
            if (arrows < 0) {
                gameSettings.screenType--;
                gameSettings.screenType = ClampInverse(gameSettings.screenType, 0, 2);
                PlayFrontendSound(_this, 3);
            }
            else if (arrows > 0) {
                gameSettings.screenType++;
                gameSettings.screenType = ClampInverse(gameSettings.screenType, 0, 2);
                PlayFrontendSound(_this, 3);
            }
            else if (enter && gameSettings.screenType != start_mode) {
                PlayFrontendSound(_this, 5);
                gameSettings.Save();
                ResetVideoDevice();
            }
            break;
        case CUSTOMMENUACTION_LIGHTING:
            if (arrows > 0 || arrows < 0) {
                gameSettings.lightingType ^= 1;
                PlayFrontendSound(_this, 3);
            }

            gameSettings.Save();
            break;
        case CUSTOMMENUACTION_LANGUAGE:
            if (arrows < 0) {
                currentLanguage--;
                PlayFrontendSound(_this, 3);

                currentLanguage = ClampInverse(currentLanguage, 0, 4);
            }
            else if (arrows > 0) {
                currentLanguage++;
                PlayFrontendSound(_this, 3);

                currentLanguage = ClampInverse(currentLanguage, 0, 4);
            }

            gameSettings.language = lang[currentLanguage];
            gameSettings.Save();

            _this->PopulateMenu();
            PopulateCustomMenu(_this);
            break;
        case CUSTOMMENUACTION_REDEFINEKEY:
            if (!redefineKey) {
                if (enter) {
                    PlayFrontendSound(_this, 5);
                    redefineKey = true;
                    previousMenuItem = _this->m_MenuPages[_this->m_nCurrentMenuPage].currentMenuItem;
                }
                else if (del) {
                    PlayFrontendSound(_this, 8);
                    gameSettings.controlKeys[_this->m_MenuPages[_this->m_nCurrentMenuPage].currentMenuItem] = 0;
                    gameSettings.Save();
                }
            }
            break;
        case CUSTOMMENUACTION_MOUSECONTROLLEDHEADING:
            if (arrows > 0 || arrows < 0) {
                gameSettings.mouseControlledHeading ^= 1;
                PlayFrontendSound(_this, 3);
            }

            gameSettings.Save();
            break;
        case CUSTOMMENUACTION_RESTOREDEFAULT: {
            if (enter) {
                gameSettings.Clear(true);
                gameSettings.Save();
            }
        } break;
        }
    }

    static void UnboundDoubleKeys(CMenuManager* _this, int i) {
        int j = 0;
        for (auto& it : gameSettings.controlKeys) {
            if (it == i && j != _this->m_MenuPages[_this->m_nCurrentMenuPage].currentMenuItem) {
                it = 0;
                break;
            }
            j++;
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
        bool lmb = NewMouseState.buttons[0] && !OldMouseState.buttons[0];

        if (ShowMouse && arrowClick != 0) {
            if (arrowClick < 0 && lmb) {
                left2 += 1;
                left += 1;
                enter = 0;
            }

            if (arrowClick > 0 && lmb) {
                right += 1;
                right2 += 1;
                enter = 0;
            }

            arrowClick = 0;
        }

        short& currentMenuPage = _this->m_nCurrentMenuPage;
        unsigned short& currentItem = _this->m_MenuPages[currentMenuPage].currentMenuItem;

        switch (currentMenuPage) {
        case MENUPAGE_12:
            _this->m_nCurrScreen = MENUSCREEN_OPTIONS;
            break;
        case MENUPAGE_13:
            _this->m_nCurrScreen = MENUSCREEN_REDBAR;

            if (redefineKey) {
                _this->m_MenuPages[_this->m_nCurrentMenuPage].currentMenuItem = previousMenuItem;

                for (unsigned int i = 0; i < dinputAllowedKeysLength; i++) {
                    int key = dinputAllowedKeys[i];
                    if (newKeys[key] && !oldKeys[key]) {
                        PlayFrontendSound(_this, 7);
                        gameSettings.controlKeys[_this->m_MenuPages[_this->m_nCurrentMenuPage].currentMenuItem] = key;
                        gameSettings.Save();

                        UnboundDoubleKeys(_this, key);
                        redefineKey = false;
                        return;
                    }
                }

                // Mouse support
                if (!wasMouseClick) {
                    bool lmb = NewMouseState.buttons[0] && !OldMouseState.buttons[0];
                    bool rmb = NewMouseState.buttons[1] && !OldMouseState.buttons[1];
                    bool mmb = NewMouseState.buttons[2] && !OldMouseState.buttons[2];
                    bool wheelUp = NewMouseState.buttons[3];
                    bool wheelDown = NewMouseState.buttons[4];

                    if (lmb) {
                        PlayFrontendSound(_this, 7);
                        gameSettings.controlKeys[_this->m_MenuPages[_this->m_nCurrentMenuPage].currentMenuItem] = DIK_LMB;
                        gameSettings.Save();

                        UnboundDoubleKeys(_this, DIK_LMB);
                        redefineKey = false;
                    }
                    else if (rmb) {
                        PlayFrontendSound(_this, 7);
                        gameSettings.controlKeys[_this->m_MenuPages[_this->m_nCurrentMenuPage].currentMenuItem] = DIK_RMB;
                        gameSettings.Save();

                        UnboundDoubleKeys(_this, DIK_RMB);
                        redefineKey = false;
                    }
                    else if (mmb) {
                        PlayFrontendSound(_this, 7);
                        gameSettings.controlKeys[_this->m_MenuPages[_this->m_nCurrentMenuPage].currentMenuItem] = DIK_MMB;
                        gameSettings.Save();

                        UnboundDoubleKeys(_this, DIK_MMB);
                        redefineKey = false;
                    }
                    else if (wheelUp) {
                        PlayFrontendSound(_this, 7);
                        gameSettings.controlKeys[_this->m_MenuPages[_this->m_nCurrentMenuPage].currentMenuItem] = DIK_WHEELUP;
                        gameSettings.Save();

                        UnboundDoubleKeys(_this, DIK_WHEELUP);
                        redefineKey = false;
                    }
                    else if (wheelDown) {
                        PlayFrontendSound(_this, 7);
                        gameSettings.controlKeys[_this->m_MenuPages[_this->m_nCurrentMenuPage].currentMenuItem] = DIK_WHEELDN;
                        gameSettings.Save();

                        UnboundDoubleKeys(_this, DIK_WHEELDN);
                        redefineKey = false;
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
            int a = _this->m_MenuPages[currentMenuPage].items[currentItem].action;
            ProcessCustomMenuOptions(_this, a, enter, del,
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

    static void DrawLoadingBar(float x, float y, float w, float h, float progress) {
        float border = ScaleY(2.0f);

        CSprite2d::DrawRect(CRect(x - border, y - border, x + w + border, y + h + border), CRGBA(255, 255, 255, 255));
        CSprite2d::DrawRect(CRect(x, y, x + w, y + h), CRGBA(255, 125, 0, 255));

        progress = Clamp(progress, 0.0f, 1.0f);
        if (progress > 0.0f)
            CSprite2d::DrawRect(CRect(x, y, x + (w * progress), y + h), CRGBA(0, 0, 255, 255));
    }

    static void DrawMouse(CMenuManager* _this) {
        if (!ShowMouse &&
            (PrevMousePosX != MousePosX || PrevMousePosY != MousePosY)) {
            ShowMouse = true;
        }

        if (!wasMouseClick) {
            if (_this &&
                (_this->NewKeyState.left ||
                    _this->NewKeyState.right ||
                    _this->NewKeyState.up ||
                    _this->NewKeyState.down ||
                    _this->NewKeyState.enter ||
                    _this->NewKeyState.esc ||
                    _this->NewKeyState.del)) {
                ShowMouse = false;
            }
        }

        if (ShowMouse) {
            psxSprites[PSXSPRITE_MOUSE].Draw(MousePosX + ScaleX(8.0f), MousePosY + ScaleY(4.0f), ScaleX(64.0f), ScaleY(64.0f), CRGBA(100, 100, 100, 50));
            psxSprites[PSXSPRITE_MOUSE].Draw(MousePosX, MousePosY, ScaleX(64.0f), ScaleY(64.0f), CRGBA(255, 255, 255, 255));
        }
    }

    static void DrawBackground(CMenuManager* _this) {
        unsigned char left;
        unsigned char right;
        CMenuManager::FindBackground(_this->m_nCurrScreen, &left, &right);
        //RenderStateSet(D3DRENDERSTATE_VERTEXBLEND, (void*)TRUE);

        bool full = _this->m_nCurrScreen == MENUSCREEN_GAMEOVER ||
            _this->m_nCurrScreen == MENUSCREEN_REDBAR ||
            _this->m_nCurrScreen == MENUSCREEN_BLUEBAR ||
            _this->m_nCurrScreen == MENUSCREEN_LOADING ||
            _this->m_nCurrScreen == MENUSCREEN_HISCORES ||
            _this->m_nCurrScreen == MENUSCREEN_CREDITS;

#if HD_FRONTEND
        frontendSprites.at(left)->Draw(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, CRGBA(255, 255, 255, 255));

        float x = SCREEN_WIDTH / 2;
        float y = ScaleY(98.0f);
        float w = ScaleX(216.0f);
        float h = ScaleY(216.0f);
        logo.Draw(CRect(x - w, y - h, x + w, y + h), CRGBA(255, 255, 255, 255));
#else
        if (full) {
            if (left == 16) {
                if (randomLoadScreen == -1) {
                    randomLoadScreen = plugin::Random((unsigned int)0, 3);
                }
                left = 25 + randomLoadScreen;
            }
            frontendSprites.at(left)->Draw(SCREEN_SCALE_X(0.0f), SCREEN_SCALE_Y(0.0f), SCREEN_SCALE_W(MENU_BACKGROUND_WIDTH), SCREEN_SCALE_H(MENU_BACKGROUND_HEIGHT), CRGBA(255, 255, 255, 255));
        }
        else {
            frontendSprites.at(left)->Draw(SCREEN_SCALE_X(0.0f), SCREEN_SCALE_Y(0.0f), SCREEN_SCALE_W(MENU_BACKGROUND_LEFT_WIDTH), SCREEN_SCALE_H(MENU_BACKGROUND_HEIGHT), CRGBA(255, 255, 255, 255));
            frontendSprites.at(right)->Draw(SCREEN_SCALE_X(MENU_BACKGROUND_LEFT_WIDTH), SCREEN_SCALE_Y(0.0f), SCREEN_SCALE_W(MENU_BACKGROUND_RIGHT_WIDTH), SCREEN_SCALE_H(MENU_BACKGROUND_HEIGHT), CRGBA(255, 255, 255, 255));
        }
#endif
    }

    static void DrawSlider(float x, float y, float progress, bool selected, float scale = 1.0f) {
        float _x = (x);
        float _y = (y);
        float _w = ScaleX(4.0f * scale);
        float _h = ScaleY(20.0f);
        float spacing = ScaleX(2.0f * scale);

        progress = Clamp(progress, 0.0f, 1.0f);

        float alpha = 0.5f;

        if (selected)
            alpha = 1.0f;

        for (int i = 0; i < 32; i++) {
            CSprite2d::DrawRect(CRect(_x, _y, _x + _w, _y + _h), CRGBA(35, 35, 35, 255));
                 
            if (i / 32.0f + 1 / 64.0f < progress)
                CSprite2d::DrawRect(CRect(_x, _y, _x + _w, _y + _h), CRGBA(((i / 32.0f) * 255) * alpha, ((1.0f - (i / 32.0f)) * 255) * alpha, 0, 255));

            _x += spacing + _w;
        }
    }

    static void DrawWindow(float x, float y, float w, float h, bool blend) {
        CRect rect;

        rect.left = x - w;
        rect.top = y - h;
        rect.right = x + w;
        rect.bottom = y + h;

        if (blend) {
            RenderStateSet(D3DRENDERSTATE_VERTEXBLEND, (void*)TRUE);
            RenderStateSet(D3DRENDERSTATE_SRCBLEND, (void*)D3DBLEND_SRCALPHA);
            RenderStateSet(D3DRENDERSTATE_DESTBLEND, (void*)D3DBLEND_DESTCOLOR);
            CSprite2d::DrawRect(rect, CRGBA(5, 45, 0, 255));
            CSprite2d::DrawRect(rect, CRGBA(0, 0, 0, 255));
        }
        else
            CSprite2d::DrawRect(rect, CRGBA(33, 66, 16, 255));//CRGBA(5, 45, 0, 255));

        RenderStateSet(D3DRENDERSTATE_SRCBLEND, (void*)D3DBLEND_SRCALPHA);
        RenderStateSet(D3DRENDERSTATE_DESTBLEND, (void*)D3DBLEND_INVSRCALPHA);

        float b = ScaleY(2.0f);
        CRect border = rect;
        border.left -= b;
        border.top -= b;
        border.right += b;
        border.bottom = rect.top;
        CSprite2d::DrawRect(border, CRGBA(115, 115, 115, 255));

        border = rect;
        border.left -= b;
        border.top = rect.bottom;
        border.right += b;
        border.bottom = border.top + b;
        CSprite2d::DrawRect(border, CRGBA(115, 115, 115, 255));

        border = rect;
        border.left -= b;
        border.top -= b;
        border.right = border.left + b;
        border.bottom += b;
        CSprite2d::DrawRect(border, CRGBA(115, 115, 115, 255));

        border = rect;
        border.left = rect.right;
        border.top -= b;
        border.right += b;
        border.bottom += b;
        CSprite2d::DrawRect(border, CRGBA(115, 115, 115, 255));
    }

    static void DrawPauseBackground() {
        DrawWindow((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2) + ScaleY(-10.0f), ScaleX(240.0f), ScaleY(190.0f), true);
    }

    static void OpenClosePauseMenu() {
        for (int i = 0; i < 12; i++) {
            gReplay->ClearButton(i);
        }

        NewMouseState.Clear();
        OldMouseState.Clear();

        if (!showPauseGameQuit) {
            GetGame()->SwitchUserPause();
            showPauseMenu = GetGame()->m_eGameStatus == 2;
            pauseMenuSelectedItem = 0;
        }

        if (!showPauseMenu) {
            SampleManager.SetStreamVolume(0, gameSettings.sfxVolume);
            SampleManager.SetStreamVolume(1, gameSettings.musicVolume);
        }
        else {
            SampleManager.SetStreamVolume(0, 0);
            SampleManager.SetStreamVolume(1, 0);
        }

        CentreCursor();
    }

    static bool CheckHover(float x1, float x2, float y1, float y2) {
        return (MousePosX > x1 && MousePosX < x2) && (MousePosY > y1 && MousePosY < y2);
    }

    static void DrawCrosshair(float x, float y, float s) {
        CRect rect = {};
        CRGBA col = { 255, 255, 255, 255 };

        float w = ScaleY(0.5f);
        float h = ScaleY(4.0f);
        float _x = x;
        float _y = y;

        y -= s;
        rect = { x - w, y, x + w, y - h };
        CSprite2d::DrawRect(rect, col);

        y = _y;
        y += s;
        rect = { x - w, y, x + w, y + h };
        CSprite2d::DrawRect(rect, col);
        y = _y;

        x -= s;
        rect = { x - h, y - w, x, y + w };
        CSprite2d::DrawRect(rect, col);

        x = _x;
        x += s;
        rect = { x, y - w, x + h, y + w };
        CSprite2d::DrawRect(rect, col);
    }

    bool init;

    FrontendFixII() {
        settings.Read();
        init = false;
#ifdef _DEBUG
        //AllocConsole();
        //freopen("conin$", "r", stdin);
        //freopen("conout$", "w", stdout);
        //freopen("conout$", "w", stderr);
        //std::setvbuf(stdout, NULL, _IONBF, 0);
#endif
        plugin::patch::Nop(0x456D0E, 5); // Don't load broken textures
        plugin::Events::initEngineEvent += [&]() {
            if (init)
                return;

            gameSettings.Load();

            for (auto& it : frontendTexFileNames) {
                CSprite2d* sprite = new CSprite2d();

                unsigned short w, h;
                unsigned char* pixels;

                plugin::LoadTGAFromFile(it.c_str(), &w, &h, &pixels);

                if (!sprite->SetTexture(it.c_str(), w, h, 32, pixels))
                    continue;

                delete[] pixels;

                frontendSprites.push_back(sprite);
            }

            //skip_quit_confirm = true;

#if HD_FRONTEND
            logo.SetTexture("data\\frontend\\gtalogo.dds");
#endif

            ShowCursor(FALSE);
            SetMousePosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
            SetResolutionsList();

            psxSprites[0].SetTexture("sfx", 32, 32, 32, (unsigned char*)sfxSprite);
            psxSprites[1].SetTexture("music", 32, 32, 32, (unsigned char*)cdVolSprite);
            psxSprites[2].SetTexture("res", 32, 32, 32, (unsigned char*)resSprite);
            psxSprites[3].SetTexture("screentype", 32, 32, 32, (unsigned char*)screenTypeSprite);
            psxSprites[4].SetTexture("lang", 32, 32, 32, (unsigned char*)langSprite);
            psxSprites[5].SetTexture("lighting", 32, 32, 32, (unsigned char*)lightingSprite);
            psxSprites[6].SetTexture("mouse", 64, 64, 32, (unsigned char*)mouseSprite);

            //BobTheText = new CText(gameSettings.language);

            //CText__Load(BobTheText, true);

            FStyle = new CStyle();
            FStyle->Load("data\\fstyle.sty");

            DefaultStyle = *gCurrentStyle;
            *gCurrentStyle = FStyle;

            FStyleTex = new CTextureManager();
            FStyleTex->Load();

            *gCurrentStyle = DefaultStyle;

            // Mouse input
            InitMouse();
            CentreCursor();

            init = true;
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

        //plugin::StdcallEvent <plugin::AddressList<0x44C2DD, plugin::H_CALL>, plugin::PRIORITY_BEFORE, plugin::ArgPickNone, void(int)> onKeyPressReplay;
        plugin::Events::drawHudEvent += []() {
            if (gameSettings.mouseControlledHeading && !UsingGamePad) {
                if (GetGame()->GetIsUserPaused() || gReplay->IsPlayingBack() || showPauseMenu)
                    return;

                CPlayerPed* playa = GetGame()->m_pCurrentPlayer;

                if (!playa || !playa->GetPed() || !playa->GetPed()->m_pObject || playa->GetPed()->GetCar())
                    return;

                float minRadius = 4.0f;
                const float maxRadius = 24.0f;
                static float s = minRadius;

                CWeapon* weapon = playa->GetPed()->m_pSelectedWeapon;
                if (weapon &&
                    weapon->m_eType != eWeaponType::WEAPONTYPE_GRENADE &&
                    weapon->m_eType != eWeaponType::WEAPONTYPE_MOLOTOV) {
                    const bool attack = playa->m_bButtonAttack;
                    minRadius -= (weapon->m_nRange / 100.0f);

                    if (attack && !weapon->m_nTimer)
                        s += 1.0f;
                }

                float scale = ScaleY(s);
                DrawCrosshair(MousePosX, MousePosY, scale);
                Interp(s, minRadius, plugin::GetTimeStepFix() * 0.2f);
            }
        };

        plugin::Events::shutdownEngineEvent += [&]() {
            if (!init)
                return;

            gameSettings.Save();

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

            //if (BobTheText) {
            //    BobTheText->Unload();
            //    delete BobTheText;
            //}

            if (FStyleTex) {
                //FStyleTex->Unload();
                delete FStyleTex;
            }

            if (FStyle) {
                FStyle->Unload();
                delete FStyle;
            }

            ShutdownMouse();

            init = false;
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

        // No credits on quit
        //plugin::patch::SetUShort(0x453EFB + 7, 258);

        // No credits on esc press
        plugin::patch::Nop(0x459AD6, 9);

        // No demo
        //plugin::patch::Set<BYTE>(0x45A349, 0xEB);
        //plugin::patch::Set<BYTE>(0x45A36C + 3, 0);

        // No sampman delay
        //plugin::patch::Nop(0x4B6823, 8);
        //plugin::patch::Set<BYTE>(0x4B700D, 0xEB);
        //plugin::patch::Set<BYTE>(0x4B6802, 0x75);

        // Fix frontend
        //auto menuPrintString = [](const wchar_t* str, int x, int y, int style, int scale, int* mode, int palette, int enableAlpha, int alpha) {
        //    float _x = SCREEN_SCALE_X(x / 16384.0f);
        //    float _y = SCREEN_SCALE_Y(y / 16384.0f);
        //    float _scale = SCREEN_SCALE_Y(scale / 16384.0f);
        //
        //    x = (int)(_x * 16384);
        //    y = (int)(_y * 16384);
        //    scale = (int)(_scale * 16384);
        //
        //    hbMenuPrintString.fun(str, x, y, style, scale, mode, palette, enableAlpha, alpha);
        //};

        //hbMenuPrintString.fun = injector::MakeCALL(0x453A1D, LAMBDA(void, __stdcall, menuPrintString, const wchar_t*, int, int, int, int, int*, int, int, int)).get();
        //plugin::patch::RedirectCall(0x453A1D, LAMBDA(void, __stdcall, menuPrintString, const wchar_t*, int, int, int, int, int*, int, int, int));

        //hbMenuPrintString.fun = injector::MakeCALL(0x453799, LAMBDA(void, __stdcall, menuPrintString, const wchar_t*, int, int, int, int, int*, int, int, int)).get();

        //plugin::patch::RedirectCall(0x453799, LAMBDA(void, __stdcall, menuPrintString, const wchar_t*, int, int, int, int, int*, int, int, int));
        //plugin::patch::RedirectCall(0x4567DC, LAMBDA(void, __stdcall, menuPrintString, const wchar_t*, int, int, int, int, int*, int, int, int));
        //plugin::patch::RedirectCall(0x456A17, LAMBDA(void, __stdcall, menuPrintString, const wchar_t*, int, int, int, int, int*, int, int, int));
        //plugin::patch::RedirectCall(0x4570A7, LAMBDA(void, __stdcall, menuPrintString, const wchar_t*, int, int, int, int, int*, int, int, int));
        //plugin::patch::RedirectCall(0x4580C1, LAMBDA(void, __stdcall, menuPrintString, const wchar_t*, int, int, int, int, int*, int, int, int));
        //plugin::patch::RedirectCall(0x458421, LAMBDA(void, __stdcall, menuPrintString, const wchar_t*, int, int, int, int, int*, int, int, int));

        plugin::StdcallEvent <plugin::AddressList<0x457DB2, plugin::H_CALL>, plugin::PRIORITY_AFTER, plugin::ArgPickNone, void(const wchar_t* str, int x, int y, int style, int scale)> onFrontendTextDraw;
        onFrontendTextDraw.before += [] {
            ApplyFontChange(true);
        };
        onFrontendTextDraw.after += [] {
            ApplyFontChange(false);
        };

        //plugin::StdcallEvent <plugin::AddressList<0x4582EE, plugin::H_CALL>, plugin::PRIORITY_BEFORE, plugin::ArgPickNone, void(int id1, int id2, char x, char y, char angle, signed int a6, int styleId, int a8, int a9, char a10, char a11)> onFrontendSpriteDraw;
        //onFrontendSpriteDraw.before += [] {
        //    ApplySpriteChange(true);
        //};
        //
        //onFrontendSpriteDraw.after += [] {
        //    ApplySpriteChange(false);
        //};

        auto drawSprite = [](int id1, int id2, int x, int y, int a5, int scale, int a7, int a8, int a9, char a10, char a11) {
            //if (!GetGame()) {
                //float _x = SCREEN_SCALE_X(x / 16384.0f);
                //float _y = SCREEN_SCALE_Y(y / 16384.0f);
                //float _scale = SCREEN_SCALE_Y(scale / 16384.0f);
                //
                //x = (int)(_x * 16384);
                //y = (int)(_y * 16384);
                //scale = (int)(_scale * 16384);

            if (id2 == 38 || id2 == 37)
                ApplySpriteChange(true);
            // }

            hbDrawSprite.fun(id1, id2, x, y, a5, scale, a7, a8, a9, a10, a11);
            ApplySpriteChange(false);
        };
        hbDrawSprite.fun = injector::MakeCALL(0x453705, LAMBDA(void, __stdcall, drawSprite, int id1, int id2, int x, int y, int a5, int a6, int a7, int a8, int a9, char a10, char a11)).get();
        plugin::patch::RedirectCall(0x453705, LAMBDA(void, __stdcall, drawSprite, int id1, int id2, int x, int y, int a5, int a6, int a7, int a8, int a9, char a10, char a11));

        hbDrawSprite.fun = injector::MakeCALL(0x453753, LAMBDA(void, __stdcall, drawSprite, int id1, int id2, int x, int y, int a5, int a6, int a7, int a8, int a9, char a10, char a11)).get();
        plugin::patch::RedirectCall(0x453753, LAMBDA(void, __stdcall, drawSprite, int id1, int id2, int x, int y, int a5, int a6, int a7, int a8, int a9, char a10, char a11));

        hbDrawSprite.fun = injector::MakeCALL(0x4582EE, LAMBDA(void, __stdcall, drawSprite, int id1, int id2, int x, int y, int a5, int a6, int a7, int a8, int a9, char a10, char a11)).get();
        plugin::patch::RedirectCall(0x4582EE, LAMBDA(void, __stdcall, drawSprite, int id1, int id2, int x, int y, int a5, int a6, int a7, int a8, int a9, char a10, char a11));

        plugin::ThiscallEvent <plugin::AddressList<0x45A2B1, plugin::H_CALL>, plugin::PRIORITY_BEFORE, plugin::ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onProcessButtonPresses;
        onProcessButtonPresses.before += [](CMenuManager* _this) {
            memcpy(oldKeys, newKeys, 256);
            memcpy(newKeys, _this->m_nKeys, 256);
            UpdateMouse();

            // Mouse support for each page 
            hoverMenuItem = -1;
            wasMouseClick = false;

            if (ShowMouse && !redefineKey) {
                short currentPage = _this->m_nCurrentMenuPage;
                short numItems = _this->m_MenuPages[_this->m_nCurrentMenuPage].numMenuItems;

                for (int i = 0; i < numItems; i++) {
                    if (PrevMousePosX == MousePosX || PrevMousePosY == MousePosY)
                        break;

                    const float x = ScaleXKeepCentered(_this->m_MenuPages[currentPage].items[i].x);
                    const float y = ScaleY(_this->m_MenuPages[currentPage].items[i].y);
                    float length = ScaleX(320.0f);
                    const float height = ScaleY(16.0f);

                    if (currentPage == MENUPAGE_13)
                        length = ScaleX(DEFAULT_SCREEN_WIDTH - 60.0f);

                    const bool hover = CheckHover(x, x + length, y, y + height);

                    if (hover) {
                        hoverMenuItem = i;
                        break;
                    }
                }

                if (hoverMenuItem != -1 && _this->m_MenuPages[currentPage].currentMenuItem != hoverMenuItem) {
                    _this->m_MenuPages[currentPage].currentMenuItem = hoverMenuItem;
                    PlayFrontendSound(_this, 2);
                }

                if (NewMouseState.buttons[0] && !OldMouseState.buttons[0]) {
                    _this->NewKeyState.enter = true;
                    wasMouseClick = true;
                }

                if (NewMouseState.buttons[4] && !OldMouseState.buttons[4]) {
                    _this->NewKeyState.left = true;
                    _this->OldKeyState.left = true;
                    wasMouseClick = true;
                }
                else if (NewMouseState.buttons[3] && !OldMouseState.buttons[3]) {
                    _this->NewKeyState.right = true;
                    _this->OldKeyState.right = true;
                    wasMouseClick = true;
                }
            }
        };

        onProcessButtonPresses.after += [](CMenuManager* _this) {
            ProcessCustomButtonPresses(_this);
        };

        plugin::ThiscallEvent <plugin::AddressList<0x456C70, plugin::H_CALL>, plugin::PRIORITY_AFTER, plugin::ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onPopulateFrontend;
        onPopulateFrontend += [](CMenuManager* _this) {
            PopulateCustomMenu(_this);
        };

        plugin::ThiscallEvent <plugin::AddressList<0x4585B9, plugin::H_JUMP>, plugin::PRIORITY_AFTER, plugin::ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onFrontendDraw;

        onFrontendDraw.before += [](CMenuManager* _this) {
            DoColorPulse();
            switch (_this->m_nCurrentMenuPage) {
            case MENUPAGE_13:
                DrawRedefineCtrlPage(_this);
                break;
            }
        };

        onFrontendDraw.after += [](CMenuManager* _this) {
            DrawCustomPages(_this);
        };

        plugin::ThiscallEvent <plugin::AddressList<0x4590D2, plugin::H_CALL>, plugin::PRIORITY_AFTER, plugin::ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onSetNewMenuBackground;
        onSetNewMenuBackground += [](CMenuManager* _this) {
            _this->NewKeyState.Clear();
            memset(_this->m_nKeys, 0, 256);
        };

        plugin::Events::menuDrawingEvent += []() {
            DrawMouse(GetFrontendMenuManager());
        };

        // No quit on esc press
        plugin::patch::Nop(0x459AF7, 10);

        // Skip intro
        plugin::patch::Set<BYTE>(0x4D145C + 2, 0);

        // 30 fps
        //plugin::patch::Set<BYTE>(0x45A472 + 2, 30);

        // Loading screen
        if (settings.EnableLoadingScreen) {
            plugin::patch::Set<BYTE>(0x459639 + 6, MENUSCREEN_LOADING);
            plugin::patch::Set<BYTE>(0x459A11 + 6, MENUSCREEN_LOADING);
            plugin::patch::Set<BYTE>(0x4598B4 + 6, MENUSCREEN_LOADING);
            plugin::patch::Set(0x453BB6 + 1, "");
            //plugin::patch::Set(0x453B9D + 1, 260);

            plugin::StdcallEvent <plugin::AddressList<0x453BC1, plugin::H_CALL>, plugin::PRIORITY_AFTER, plugin::ArgPickNone, void(const wchar_t*, int, int, int, int)> drawLoadingBar;
            drawLoadingBar += [] {
                gbh_EndScene();
                Vid_FreeSurface(GetVideoSystem());
                Vid_FlipBuffers(GetVideoSystem());
                Vid_ClearScreen(GetVideoSystem(), 0, 0, 0, 0, 0, GetVideoSystem()->width, GetVideoSystem()->height);

                SampleManager.SetStreamVolume(0, 0);
                SampleManager.SetStreamVolume(0, 1);

                static float progress = 0.0f;
                while (progress < 150.0f) {
                    {
                        plugin::Call<0x4CAEC0>();
                        gbh_BeginScene();

                        DrawBackground(GetFrontendMenuManager());
                        DrawLoadingBar(ScaleXKeepCentered(70.0f), SCREEN_HEIGHT - ScaleY(70.0f), ScaleX(504.0f), ScaleY(22.0f), progress / 100.0f);

                        const wchar_t* str = L"LOADING";
                        int s = (CFont::GetStringWidth(str, GetFrontendMenuManager()->m_nFontStyle) * (1.2f)) / 2;
                        CFont::PrintString(str, ScaleXKeepCentered(320 - s) * 16384, ScaleY(143 * 16384), GetFrontendMenuManager()->m_nFontStyle, ScaleY(1.2f) * 16384, 2, 0, 0, 0);
                        progress += 1.0f;
                        gbh_EndScene();
                        Vid_FreeSurface(GetVideoSystem());
                        Vid_FlipBuffers(GetVideoSystem());
                        Vid_ClearScreen(GetVideoSystem(), 0, 0, 0, 0, 0, GetVideoSystem()->field_48, GetVideoSystem()->field_4C);
                    }
                }

                progress = 0.0f;
                randomLoadScreen = -1;
            };
        }

        if (settings.EnablePauseMenu) {
            // no f3/f4 game vol
            plugin::patch::Nop(0x4A7145, 42);

            // No f6 game pause
            plugin::patch::Nop(0x4A71A7, 11);

            // No esc game quit
            plugin::patch::Nop(0x4A7080, 18);

            // No f8 gta manager
            plugin::patch::Nop(0x4A71ED, 5);

            // Pause menu key 
            plugin::patch::Set<BYTE>(0x4A705A, 0xEB);

            //auto openClosePauseMenu = [](void* _this) {
            //    OpenClosePauseMenu();
            //};
            //
            //auto openClosePauseMenu2 = [](void* _this, int) {
            //    OpenClosePauseMenu();
            //};
            //
            //plugin::patch::RedirectCall(0x4A708D, LAMBDA(void, __stdcall, openClosePauseMenu, void*));
            //plugin::patch::RedirectCall(0x4A71AD, LAMBDA(void, __fastcall, openClosePauseMenu2, void*, int));

            // No freeze
            plugin::patch::Set<BYTE>(0x418079, 0xEB);

            // Pause menu
            auto drawGamePaused = [](int id1, int id2, char x, char y, char style, int* a6, int a7, int a8, char a9) {
                ApplyStyleSwitch(true);
                DrawPauseBackground();
                const int fontStyle = 10;
                const float fontScale = (1.2f);
                wchar_t* str = GetTheText()->Get("psxeff");
                const int mode = 8;
                const int pal = 8;

                int s = (CFont::GetStringWidth(str, fontStyle) * (fontScale)) / 2;
                CFont::PrintString(str, ScaleXKeepCentered(320 - s) * 16384, ScaleY(140.0f) * 16384, fontStyle, ScaleY(fontScale) * 16384, pauseMenuSelectedItem == 0 ? 2 : 8, pal, false, 0);
                if (!showPauseGameQuit && ShowMouse && CheckHover(ScaleXKeepCentered(320 - s), ScaleXKeepCentered(320 + s), ScaleY(140.0f), ScaleY(140.0f + 50.0f))) pauseMenuSelectedItem = 0;

                DrawSlider(ScaleXKeepCentered(320.0f - 128.0f), ScaleY(168.0f), gameSettings.sfxVolume / 127.0f, pauseMenuSelectedItem == 0, 1.32f);

                str = GetTheText()->Get("psxmus");
                s = (CFont::GetStringWidth(str, fontStyle) * (fontScale)) / 2;
                CFont::PrintString(str, ScaleXKeepCentered(320 - s) * 16384, ScaleY(200.0f) * 16384, fontStyle, ScaleY(fontScale) * 16384, pauseMenuSelectedItem == 1 ? 2 : 8, pal, false, 0);
                if (!showPauseGameQuit && ShowMouse && CheckHover(ScaleXKeepCentered(320 - s), ScaleXKeepCentered(320 + s), ScaleY(200.0f), ScaleY(200.0f + 50.0f))) pauseMenuSelectedItem = 1;

                DrawSlider(ScaleXKeepCentered(320.0f - 128.0f), ScaleY(228.0f), gameSettings.musicVolume / 127.0f, pauseMenuSelectedItem == 1, 1.32f);

                RenderStateSet(D3DRENDERSTATE_VERTEXBLEND, (void*)TRUE);
                RenderStateSet(D3DRENDERSTATE_SRCBLEND, (void*)D3DBLEND_ONE);
                RenderStateSet(D3DRENDERSTATE_DESTBLEND, (void*)D3DBLEND_ONE);

                psxSprites[0].Draw(ScaleXKeepCentered(158.0f), ScaleY(168.0f), ScaleX(30.0f), ScaleY(20.0f), CRGBA(255, 255, 255, 255));
                psxSprites[1].Draw(ScaleXKeepCentered(158.0f), ScaleY(228.0f), ScaleX(30.0f), ScaleY(20.0f), CRGBA(255, 255, 255, 255));

                RenderStateSet(D3DRENDERSTATE_SRCBLEND, (void*)D3DBLEND_SRCALPHA);
                RenderStateSet(D3DRENDERSTATE_DESTBLEND, (void*)D3DBLEND_INVSRCALPHA);

                str = nullptr;
                const int textSpeed = GetHud()->m_nTextSpeed;
                switch (textSpeed) {
                case 0:
                case 1:
                    str = GetTheText()->Get("psxslow");
                    break;
                case 2:
                case 3:
                    str = GetTheText()->Get("psxnorm");
                    break;
                case 4:
                case 5:
                    str = GetTheText()->Get("psxfast");
                    break;
                };

                wsprintfW(gUString, L"%s %s", GetTheText()->Get("psxtext"), str);
                str = gUString;

                s = (CFont::GetStringWidth(str, fontStyle) * (fontScale)) / 2;
                CFont::PrintString(str, ScaleXKeepCentered(320 - s) * 16384, ScaleY(260.0f) * 16384, fontStyle, ScaleY(fontScale) * 16384, pauseMenuSelectedItem == 2 ? 2 : 8, pal, false, 0);
                if (!showPauseGameQuit && ShowMouse && CheckHover(ScaleXKeepCentered(320 - s), ScaleXKeepCentered(320 + s), ScaleY(260.0f), ScaleY(260.0f + 18.0f))) pauseMenuSelectedItem = 2;

                str = GetTheText()->Get("psxret");
                s = (CFont::GetStringWidth(str, fontStyle) * (fontScale)) / 2;
                CFont::PrintString(str, ScaleXKeepCentered(320 - s) * 16384, ScaleY(290.0f) * 16384, fontStyle, ScaleY(fontScale) * 16384, pauseMenuSelectedItem == 3 ? 2 : 8, pal, false, 0);
                if (!showPauseGameQuit && ShowMouse && CheckHover(ScaleXKeepCentered(320 - s), ScaleXKeepCentered(320 + s), ScaleY(290.0f), ScaleY(290.0f + 18.0f))) pauseMenuSelectedItem = 3;

                str = GetTheText()->Get("psxquit");
                s = (CFont::GetStringWidth(str, fontStyle) * (fontScale)) / 2;
                CFont::PrintString(str, ScaleXKeepCentered(320 - s) * 16384, ScaleY(320.0f) * 16384, fontStyle, ScaleY(fontScale) * 16384, pauseMenuSelectedItem == 4 ? 2 : 8, pal, false, 0);
                if (!showPauseGameQuit && ShowMouse && CheckHover(ScaleXKeepCentered(320 - s), ScaleXKeepCentered(320 + s), ScaleY(320.0f), ScaleY(320.0f + 18.0f))) pauseMenuSelectedItem = 4;

                // Help text
                str = nullptr;
                switch (pauseMenuSelectedItem) {
                case 0:
                case 1:
                    str = GetTheText()->Get("psxchng");
                    break;
                case 2:
                case 3:
                case 4:
                    str = GetTheText()->Get("psxsel");
                    break;
                }

                if (str) {
                    s = (CFont::GetStringWidth(str, fontStyle) * (fontScale)) / 2;
                    CFont::PrintString(str, ScaleXKeepCentered(320 - s) * 16384, ScaleY(380.0f) * 16384, fontStyle, ScaleY(fontScale) * 16384, 2, 0, false, 0);
                }

                if (showPauseGameQuit) {
                    DrawWindow(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, ScaleX(230.0f), ScaleY(90.0f), false);

                    str = GetTheText()->Get("psxsq0");
                    s = (CFont::GetStringWidth(str, fontStyle) * (fontScale)) / 2;
                    CFont::PrintString(str, ScaleXKeepCentered(320 - s) * 16384, ((SCREEN_HEIGHT / 2) + ScaleY(-80.0f)) * 16384, fontStyle, ScaleY(fontScale) * 16384, 2, 0, false, 0);

                    str = GetTheText()->Get("psxsq1");
                    s = (CFont::GetStringWidth(str, fontStyle) * (fontScale)) / 2;
                    CFont::PrintString(str, ScaleXKeepCentered(320 - s) * 16384, ((SCREEN_HEIGHT / 2) + ScaleY(-60.0f)) * 16384, fontStyle, ScaleY(fontScale) * 16384, 2, 0, false, 0);

                    str = GetTheText()->Get("psxconf");
                    s = (CFont::GetStringWidth(str, fontStyle) * (fontScale)) / 2;
                    CFont::PrintString(str, ScaleXKeepCentered(320 - s - 98.0f) * 16384, ((SCREEN_HEIGHT / 2) + ScaleY(10.0f)) * 16384, fontStyle, ScaleY(fontScale) * 16384, 2, 0, false, 0);

                    str = GetTheText()->Get("psxcanc");
                    s = (CFont::GetStringWidth(str, fontStyle) * (fontScale)) / 2;
                    CFont::PrintString(str, ScaleXKeepCentered(320 - s + 98.0f) * 16384, ((SCREEN_HEIGHT / 2) + ScaleY(10.0f)) * 16384, fontStyle, ScaleY(fontScale) * 16384, 2, 0, false, 0);
                }

                ApplyStyleSwitch(false);

                DrawMouse(nullptr);
            };

            plugin::patch::RedirectCall(0x4CA080, LAMBDA(void, __stdcall, drawGamePaused, int id1, int id2, char x, char y, char style, int* a6, int a7, int a8, char a9));

            auto drawGamePausedText = [](wchar_t* str, char x, char y, int style, signed int* a5, signed int* a6, signed int* a7, int a8) {
                ApplyStyleSwitch(true);
                int s = (CFont::GetStringWidth(str, 1) * (1.0f)) / 2;

                CFont::PrintString(str, ScaleXKeepCentered(320 - s) * 16384, ScaleY(88.0f) * 16384, 10, ScaleY(1.2f) * 16384, 2, 0, 0, 0);
                ApplyStyleSwitch(false);
            };
            plugin::patch::RedirectCall(0x4CA0DD, LAMBDA(void, __stdcall, drawGamePausedText, wchar_t* str, char x, char y, int style, signed int* a5, signed int* a6, signed int* a7, int a8));

            plugin::patch::RedirectJump(0x4CA00D, (void*)0x4CA080);

            // Pause menu stats
            plugin::patch::SetInt(0x4CA3F6 + 1, 52); // text y
            plugin::patch::SetInt(0x4CA1A3 + 1, 52 + 12); // sprite y

            plugin::ThiscallEvent <plugin::AddressList<0x4A7683, plugin::H_CALL>, plugin::PRIORITY_AFTER, plugin::ArgPickN<CPlayerPed*, 0>, void(CPlayerPed*)> onProcessPlayerKeys;

            onProcessPlayerKeys += [&](CPlayerPed* _this) {
                if (gReplay->IsPlayingBack()) {
                    return;
                }

                if (_this == GetGame()->m_pCurrentPlayer) {
                    // Mouse support
                    UpdateMouse();

                    CPad* pad = CPad::GetPad(0);

                    if (pad && pad->IsConnected() && pad->NewState.CheckForInput()) {
                        if (pad->NewState.CheckForInput()) {
                            UsingGamePad = true;
                        }
                    }

                    if ((_this->m_nKeyboardKey >> 12) || abs(NewMouseState.x) || abs(NewMouseState.y)) {
                        UsingGamePad = false;
                    }

                    wasMouseClick = false;

                    // Do mouse controls for player in focus
                    // Button presses
                    for (int i = 0; i < 12; i++) {
                        int ctrl = ControlKeys[i];

                        if (ctrl >= DIK_LMB && ctrl <= DIK_WHEELDN) {
                            if (NewMouseState.buttons[ctrl - DIK_LMB])
                                gReplay->SetButton(i);
                            else
                                gReplay->ClearButton(i);
                        }
                    }
                    // Player rotation
                    if (gameSettings.mouseControlledHeading && !UsingGamePad) {
                        if (_this->GetPed() &&
                            _this->GetPed()->m_pObject &&
                            _this->GetPed()->m_nPedState != PEDSTATE_FALL &&
                            _this->GetPed()->m_nPedState != PEDSTATE_DEAD &&
                            !_this->GetPed()->m_nPedFlags.bIsArrested) {
                            CEncodedVector in = _this->GetPed()->GetPosition();
                            CEncodedVector2D out;
                            _this->GetAuxCamera()->WorldToScreen2D(in, &out);

                            float x = MousePosX - out.FromInt16().x;
                            float y = MousePosY - out.FromInt16().y;
                            float rotDest = atan2f(x, y);
                            float rotCur = DegToRad(_this->GetPed()->m_pObject->m_nRotation / 4.0f);
                            float angle = LimitRadianAngle(rotDest);

                            if (rotCur - M_PI > rotDest) {
                                angle += 2 * M_PI;
                            }
                            else if (M_PI + rotCur < rotDest) {
                                angle -= 2 * M_PI;
                            }

                            short finalRot = static_cast<short>(RadToDeg((angle - rotCur) * plugin::GetTimeStepFix()));
                            _this->GetPed()->m_pObject->m_nRotation += finalRot;

                            _this->m_bButtonLeft = false;
                            _this->m_bButtonRight = false;
                            gReplay->ClearButton(CONTROLKEY_LEFT);
                            gReplay->ClearButton(CONTROLKEY_RIGHT);
                        }
                    }

                    bool up = _this->IsKeyJustDown(DIK_UP);
                    bool down = _this->IsKeyJustDown(DIK_DOWN);
                    bool left = _this->IsKeyJustDown(DIK_LEFT);
                    bool right = _this->IsKeyJustDown(DIK_RIGHT);
                    bool enter = _this->IsKeyJustDown(DIK_RETURN);
                    bool esc = _this->IsKeyJustDown(DIK_ESCAPE);

                    if (pad) {
                        up |= (pad->NewState.DPadUp && !pad->OldState.DPadUp) ? 1 : 0;
                        down |= (pad->NewState.DPadDown && !pad->OldState.DPadDown) ? 1 : 0;
                        left |= (pad->NewState.DPadLeft && !pad->OldState.DPadLeft) ? 1 : 0;
                        right |= (pad->NewState.DPadRight && !pad->OldState.DPadRight) ? 1 : 0;
                        enter |= (pad->NewState.Cross && !pad->OldState.Cross) ? 1 : 0;

                        if (showPauseGameQuit)
                            esc |= (pad->NewState.Triangle && !pad->OldState.Triangle) ? 1 : 0;
                        else
                            esc |= (pad->NewState.Start && !pad->OldState.Start) ? 1 : 0;
                    }

                    if (up || down || left || right || enter || esc)
                        ShowMouse = false;

                    if (esc)
                        OpenClosePauseMenu();

                    if (!showPauseMenu)
                        return;

                    if (showPauseGameQuit) {
                        if (ShowMouse) {
                            if (CheckHover(ScaleXKeepCentered(320.0f - 98.0f - 42.0f), ScaleXKeepCentered(320.0f - 98.0f + 42.0f), ((SCREEN_HEIGHT / 2) + ScaleY(10.0f)), ((SCREEN_HEIGHT / 2) + ScaleY(10.0f + 18.0f))))
                                enter |= NewMouseState.buttons[0] && !OldMouseState.buttons[0];
                            else if (CheckHover(ScaleXKeepCentered(320.0f + 98.0f - 42.0f), ScaleXKeepCentered(320.0f + 98.0f + 42.0f), ((SCREEN_HEIGHT / 2) + ScaleY(10.0f)), ((SCREEN_HEIGHT / 2) + ScaleY(10.0f + 18.0f))))
                                esc |= NewMouseState.buttons[0] && !OldMouseState.buttons[0];
                        }

                        if (enter) {
                            GetGame()->SetState(1, 2);
                            showPauseGameQuit = false;
                            showPauseMenu = false;
                        }
                        else if (esc) {
                            showPauseGameQuit = false;
                        }
                    }
                    else {
                        if (ShowMouse) {
                            left |= NewMouseState.buttons[4] && !OldMouseState.buttons[4];
                            right |= NewMouseState.buttons[3] && !OldMouseState.buttons[3];
                            enter |= NewMouseState.buttons[0] && !OldMouseState.buttons[0];
                        }

                        int arrows = left ? -1 : right ? 1 : 0;

                        if (up) {
                            pauseMenuSelectedItem--;
                            pauseMenuSelectedItem = ClampInverse(pauseMenuSelectedItem, 0, 4);
                        }
                        else if (down) {
                            pauseMenuSelectedItem++;
                            pauseMenuSelectedItem = ClampInverse(pauseMenuSelectedItem, 0, 4);
                        }

                        if (arrows != 0 || enter) {
                            switch (pauseMenuSelectedItem) {
                            case 0:
                                gameSettings.sfxVolume += arrows * (128 / 32);
                                gameSettings.sfxVolume = Clamp(gameSettings.sfxVolume, 0, 127);
                                gameSettings.Save();
                                break;
                            case 1:
                                gameSettings.musicVolume += arrows * (128 / 32);
                                gameSettings.musicVolume = Clamp(gameSettings.musicVolume, 0, 127);
                                gameSettings.Save();
                                break;
                            case 2: {
                                int& textSpeed = GetHud()->m_nTextSpeed;
                                if (left) {
                                    switch (textSpeed) {
                                    case 0:
                                    case 1:
                                        textSpeed = 5;
                                        break;
                                    case 2:
                                    case 3:
                                        textSpeed = 1;
                                        break;
                                    case 4:
                                    case 5:
                                        textSpeed = 3;
                                        break;
                                    };
                                }
                                else if (right) {
                                    switch (textSpeed) {
                                    case 0:
                                    case 1:
                                        textSpeed = 3;
                                        break;
                                    case 2:
                                    case 3:
                                        textSpeed = 5;
                                        break;
                                    case 4:
                                    case 5:
                                        textSpeed = 1;
                                        break;
                                    };
                                }
                            } break;
                            case 3:
                                if (enter)
                                    OpenClosePauseMenu();
                                break;
                            case 4:
                                if (enter)
                                    showPauseGameQuit = true;
                                break;
                            }
                        }
                    }
                }
            };

            // Fix enter key
            plugin::patch::Set<BYTE>(0x44C2C7, 0xEB);

            auto setInitialTextSpeed = [](CHud* _this, int) {
                _this->m_nTextSpeed = 3;
            };
            plugin::patch::RedirectJump(0x4C6DC0, LAMBDA(void, __fastcall, setInitialTextSpeed, CHud*, int));

            auto stateTwoKeys = [](int key) {
                //if (GetGame()->m_pCurrentPlayer->m_bShowGameQuitText)
                //    return GetGame()->m_pCurrentPlayer->m_bShowGameQuitText && (key == DIK_RETURN || key == DIK_ESCAPE);
                if (showPauseMenu) {
                    return key == DIK_RETURN || key == DIK_UP || key == DIK_DOWN || key == DIK_LEFT || key == DIK_RIGHT || key == DIK_ESCAPE;
                }

                return false;
            };
            plugin::patch::RedirectJump(0x4C6E20, LAMBDA(bool, __stdcall, stateTwoKeys, int));
        }

        auto resetVideoDevice = []() {
            plugin::Call<0x4CADA0>();
            Vid_CloseScreen(GetVideoSystem());

            GetVideoSystem()->flags |= 0x10;

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

            int mode = -2;

            if (start_mode == 0) {
                mode = Vid_CheckMode(GetVideoSystem(), full_width, full_height, 32);
                SetWindowLongPtr(GetHWnd(), GWL_STYLE, WS_VISIBLE | WS_POPUP);
                SetWindowPos(GetHWnd(), HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, SWP_FRAMECHANGED);
            }
            else if (start_mode == 1) {
                int extraWidth = GetSystemMetrics(SM_CXSIZEFRAME);
                int extraHeight = GetSystemMetrics(SM_CYSIZE) + GetSystemMetrics(SM_CYSIZEFRAME);
                rect.left -= extraWidth / 2;
                rect.top -= extraHeight / 2;
                rect.right += extraWidth;
                rect.bottom += extraHeight;
                SetWindowLong(GetHWnd(), GWL_STYLE, WS_VISIBLE | (WS_OVERLAPPEDWINDOW & ~WS_SIZEBOX));
                SetWindowPos(GetHWnd(), HWND_NOTOPMOST, rect.left, rect.top, rect.right, rect.bottom, 0);
            }
            else if (start_mode == 2) {
                SetWindowLongPtr(GetHWnd(), GWL_STYLE, WS_VISIBLE | WS_POPUP);
                SetWindowPos(GetHWnd(), HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, SWP_FRAMECHANGED);
            }
            else {
                assert(start_mode < 2);
            }

            gbh_SetMode(GetVideoSystem(), GetHWnd(), mode);

            plugin::Call<0x4CC580>();
            plugin::Call<0x4CB730>();

            if (GetGame()) {
                plugin::Call<0x4CB880>();
            }

            Vid_ClearScreen(GetVideoSystem(), 0, 0, 0, window_x, window_y, window_width, window_height);
            Vid_FlipBuffers(GetVideoSystem());
            Vid_ClearScreen(GetVideoSystem(), 0, 0, 0, window_x, window_y, window_width, window_height);
        };
        plugin::patch::RedirectJump(0x4CC5C0, LAMBDA(void, __cdecl, resetVideoDevice));

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
} frontendFixII;
