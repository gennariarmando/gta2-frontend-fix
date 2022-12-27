#pragma once
#define REDEFINE_TEXT_Y (154)
#define REDEFINE_TEXT_SPACING (22)
#define REDEFINE_RECT_SIZE (22)
#include <locale>
#include <iostream>
#include <ctype.h>

class CText;

extern CText* BobTheText;

extern void CText__Load(CText* text, bool bob = false);
