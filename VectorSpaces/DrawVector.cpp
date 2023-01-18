#include "DrawVector.h"
#include "raymath.h"

int DrawVector::vectorCount = 0;

DrawVector::DrawVector(float x, float y, float z) : vector({x, y, z}), origin(Vector3Zero())
{
    vectorIndex = vectorCount;
    vectorCount++;

    color = GetRandomColor();
}

Color DrawVector::GetRandomColor()
{
    float offset = GetRandomValue(0, 180);
    float h = (int)(offset + vectorIndex * 137.508f) % 360; // use golden angle approximation
    float s = GetRandomValue(60, 80) / 100.0f;
    const float v = 1;

    if (h < 220 || h > 280) {
        return ColorFromHSV(h, s, v);
    }

    s = 0.4f;
    return ColorFromHSV(h, s, v);
}

float DrawVector::X() const
{
    return vector.x;
}

float DrawVector::Y() const
{
    return vector.y;
}

float DrawVector::Z() const
{
    return vector.z;
}