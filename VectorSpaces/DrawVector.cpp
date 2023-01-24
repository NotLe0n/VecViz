#include "DrawVector.h"
#include "raymath.h"
#include "../utils.h"

int DrawVector::vectorCount = 0;

DrawVector::DrawVector(float x, float y, float z) : vector({x, y, z}), origin(Vector3Zero()), selected(false)
{
    vectorIndex = vectorCount;
    vectorCount++;

    color = ColorToImVec4(GetRandomColor());
}

Color DrawVector::GetRandomColor() const
{
    float offset = GetRandomValue(0, 180);
    float h = (int)(offset + vectorIndex * 137.508f) % 360; // use golden angle approximation
    float s = GetRandomValue(60, 80) / 100.0f;
    const float v = 1;

    // if color isn't inside the dark range, return
    if (h < 220 || h > 280) {
        return ColorFromHSV(h, s, v);
    }

    // if it is a dark color, reduce saturation and return
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

Color DrawVector::GetColor() const
{
    return {(unsigned char)(color.x * 255), (unsigned char)(color.y * 255), (unsigned char)(color.z * 255), 255};
}

