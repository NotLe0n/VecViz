#ifndef VECVIZ_DRAWING_H
#define VECVIZ_DRAWING_H

#include "functional"
#include "raylib.h"
#include "raymath.h"
#include "FontManager.h"

class Drawing {
public:
    static void DrawOutsideOfRt(Camera2D camera, RenderTexture rt, const std::function<void()>& drawFunc){
        BeginScissorMode(300, 0, rt.texture.width, rt.texture.height);
        EndMode2D();
        EndTextureMode();

        drawFunc();

        BeginTextureMode(rt);
        BeginMode2D(camera);
        EndScissorMode();
    }

    static void DrawArrow2D(Vector2 pos, Vector2 origin, float arrowLength, Color color) {
        // calculate arrow triangle points
        float dx = pos.x - origin.x;
        float dy = pos.y - origin.y;

        const float angle = 35 * DEG2RAD;
        float theta = atan2f(dy, dx);

        Vector2 leftPoint = {
                pos.x - arrowLength * cosf(theta + angle),
                pos.y - arrowLength * sinf(theta + angle)
        };

        Vector2 rightPoint = {
                pos.x - arrowLength * cosf(theta - angle),
                pos.y - arrowLength * sinf(theta - angle)
        };

        // draw arrow
        DrawTriangle(leftPoint, rightPoint, pos, color);
        DrawLineV(origin, pos, color);
    }

    static void DrawText(const std::string& text, const float& posX, const float& posY, const Color& color, const float& size) {
        DrawTextEx(FontManager::font, text.c_str(), {posX, posY}, size, 0, color);
    }

    static void DrawText(const std::string& text, Vector2 pos, const Color& color, const float& size) {
        DrawTextEx(FontManager::font, text.c_str(), pos, size, 0, color);
    }

    static void DrawMathText(const std::string& text, const float& posX, const float& posY, const Color& color, const float& size) {
        DrawTextEx(FontManager::mathFont, text.c_str(), {posX, posY}, size, 0, color);
    }

    /*void FontManager::DrawUnicodeMathText(const int *text, int count, const float &posX, const float &posY, const Color &color, const float &size) {
        DrawTextCodepoints(mathFont, text, count, {posX, posY}, size, 0, color);
    }*/

    static void DrawUnicodeMathChar(const int& text, const float &posX, const float &posY, const Color &color, const float &size) {
        DrawTextCodepoint(FontManager::mathFont, text, {posX, posY}, size, color);
    }
};


#endif //VECVIZ_DRAWING_H
