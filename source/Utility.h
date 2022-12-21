#pragma once
#include "common.h"
#include "CVector.h"
#include "CSprite2d.h"

class CRGBA;
class CRect;

#define DEFAULT_SCREEN_WIDTH 640.0f
#define DEFAULT_SCREEN_HEIGHT 480.0f
#define DEFAULT_SCREEN_ASPECT_RATIO (DEFAULT_SCREEN_WIDTH / DEFAULT_SCREEN_HEIGHT)
#define SCREEN_ASPECT_RATIO (SCREEN_WIDTH / SCREEN_HEIGHT)

static float ScaleX(float x) {
    float f = ((x) * (float)SCREEN_WIDTH / DEFAULT_SCREEN_WIDTH) * DEFAULT_SCREEN_ASPECT_RATIO / SCREEN_ASPECT_RATIO;
    return f;
}

static float ScaleXKeepCentered(float x) {
    float f = ((SCREEN_WIDTH == DEFAULT_SCREEN_WIDTH) ? (x) : (SCREEN_WIDTH - ScaleX(DEFAULT_SCREEN_WIDTH)) / 2 + ScaleX((x)));
    return f;
}

static float ScaleY(float y) {
    float f = ((y) * (float)SCREEN_HEIGHT / DEFAULT_SCREEN_HEIGHT);
    return f;
}

static float ScaleW(float w) {
    float f = ((w) * (float)SCREEN_WIDTH / DEFAULT_SCREEN_WIDTH) * DEFAULT_SCREEN_ASPECT_RATIO / SCREEN_ASPECT_RATIO;
    return f;
}

static float ScaleH(float h) {
    float f = ((h) * (float)SCREEN_HEIGHT / DEFAULT_SCREEN_HEIGHT);
    return f;
}

#define SCREEN_SCALE_X(x) (ScaleXKeepCentered(x))
#define SCREEN_SCALE_Y(y) (ScaleY(y))

#define SCREEN_SCALE_W(w) (ScaleW(w))
#define SCREEN_SCALE_H(h) (ScaleH(h))

template<typename T, typename T2, typename T3>
static T Clamp(T v, T2 low, T3 high) {
    return ((v) < (low) ? (low) : (v) > (high) ? (high) : (v));
}

template<typename T, typename T2, typename T3>
static T ClampInverse(T v, T2 low, T3 high) {
    return ((v) < (low) ? (high) : (v) > (high) ? (low) : (v));
}

template<typename T>
static T DegToRad(T x) {
    return (x * M_PI / (T)180);
}

template<typename T>
static T RadToDeg(T x) {
    return (x * (T)180 / M_PI);
}

template<typename T>
static void Interp(T& a, T b, T f) {
    a = a + (f) * (b - a);
}

template<typename T>
static bool IsNearlyEqual(T a, T b, T t) {
    return (fabs(a - b) <= t);
}

static void RotateVertices(CVector2D* rect, float x, float y, float angle) {
    float xold, yold;
    //angle /= 57.2957795;
    float _cos = cosf(angle);
    float _sin = sinf(angle);
    for (unsigned int i = 0; i < 4; i++) {
        xold = rect[i].x;
        yold = rect[i].y;
        rect[i].x = x + (xold - x) * _cos + (yold - y) * _sin;
        rect[i].y = y - (xold - x) * _sin + (yold - y) * _cos;
    }
}

static void DrawTriangle(float x, float y, float w, float h, CVector2D center, float angle, CRGBA const& col) {
    CVector2D posn[4];
    posn[1].x = x - (w * 0.5f); posn[1].y = y - (h * 0.5f); posn[0].x = x + (w * 0.5f); posn[0].y = y - (h * 0.5f);
    posn[3].x = x; posn[3].y = y + (h * 0.5f);	posn[2].x = x; posn[2].y = y + (h * 0.5f);

    RotateVertices(posn, x + (w * center.x), y + (h * center.y), angle);
    CSprite2d::Draw2DPolygon(posn[0].x, posn[0].y, posn[1].x, posn[1].y, posn[2].x, posn[2].y, posn[3].x, posn[3].y, CRGBA(col));
}

static void DrawSATarget(float x, float y, float dist, float rotationMult, CRGBA const& col) {
    dist = Clamp(dist, 0.6f, 1.0f);

    static float angle = 0.0f;
    angle += ((0.2f * rotationMult) * 0.2f);

    float b = ScaleY(10.0f * dist);
    float s = ScaleY(9.8f * dist);
    DrawTriangle(x, y, b * 0.8f, b, CVector2D(0.0f, dist + 1.0f), DegToRad(60.0f) - angle, CRGBA(0, 0, 0, col.a));
    DrawTriangle(x, y, s * 0.8f, s, CVector2D(0.0f, dist + 1.0f), DegToRad(60.0f) - angle, col);
    DrawTriangle(x, y, b * 0.8f, b, CVector2D(0.0f, dist + 1.0f), DegToRad(-60.0f) - angle, CRGBA(0, 0, 0, col.a));
    DrawTriangle(x, y, s * 0.8f, s, CVector2D(0.0f, dist + 1.0f), DegToRad(-60.0f) - angle, col);
    DrawTriangle(x, y, b * 0.8f, b, CVector2D(0.0f, dist + 1.0f), DegToRad(180.0f) - angle, CRGBA(0, 0, 0, col.a));
    DrawTriangle(x, y, s * 0.8f, s, CVector2D(0.0f, dist + 1.0f), DegToRad(180.0f) - angle, col);
}
