#ifndef VECVIZ_DRAWING_H
#define VECVIZ_DRAWING_H

#include <functional>
#include "raylib.h"

namespace Drawing {
    void DrawToOtherRt(Camera2D camera, RenderTexture rt, RenderTexture otherRt, const std::function<void()>& drawFunc);

    void DrawArrow2D(Vector2 pos, Vector2 origin, float arrowLength, Color color);

    void DrawText(const std::string& text, const float& posX, const float& posY, const Color& color, const float& size);

    void DrawText(const std::string& text, Vector2 pos, const Color& color, const float& size);

    Vector2 MeasureText(const std::string& text, float fontSize);

    /*static void DrawMathText(const std::string& text, const float& posX, const float& posY, const Color& color, const float& size);*/
}

#endif //VECVIZ_DRAWING_H
